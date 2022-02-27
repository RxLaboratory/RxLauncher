#include "androidmanager.h"

AndroidManager* AndroidManager::_instance = nullptr;

AndroidManager::AndroidManager(QObject *parent) : QObject(parent)
{
    _instance = this;
    _androidWorker = new AndroidUtils;
    connect(_androidWorker, &AndroidUtils::gotApplications, this, &AndroidManager::updateApplications);

    registerNativeMethods();

    registerServices();
}

AndroidManager *AndroidManager::instance()
{
    if (!_instance) new AndroidManager();
    return _instance;
}

void AndroidManager::registerNativeMethods() {
    JNINativeMethod methods[] {
        {"appUninstalled", "(Lorg/rxlab/launcher/AppInfo;)V", reinterpret_cast<void *>(&AndroidManager::androidAppUninstalled)},
        {"appInstalled", "(Lorg/rxlab/launcher/AppInfo;)V", reinterpret_cast<void *>(&AndroidManager::androidAppInstalled)}
    };

    QAndroidJniObject javaClass("org/rxlab/launcher/AppManager");
    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
    env->RegisterNatives(objectClass,
                         methods,
                         sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);
}

void AndroidManager::registerServices()
{
    QAndroidJniObject::callStaticMethod<void>("org.rxlab.launcher.AppManager",
                                              "initAppListener",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidContext().object());
}

AppInfo *AndroidManager::toApp(QAndroidJniObject obj)
{
    const bool isSystem = obj.getField<jboolean>("system");
    const QString packageName = obj.getObjectField<jstring>("packageName").toString();
    const QString label = obj.getObjectField<jstring>("label").toString();
    AppInfo *app = new AppInfo(label, packageName, AppInfo::App, isSystem, AndroidManager::instance());
    return app;
}

AppInfo *AndroidManager::toApp(jobject obj)
{
    return toApp(QAndroidJniObject(obj));
}

QList<AppInfo *> AndroidManager::apps() const
{
    return _apps;
}

QList<AppInfo *> AndroidManager::systemApps()
{
    QList<AppInfo *> apps;
    foreach(AppInfo *app, _apps)
    {
        if (app->isSystemApp())
        {
            apps << app;
        }
    }
    return apps;
}

QList<AppInfo *> AndroidManager::userApps()
{
    QList<AppInfo *> apps;
    foreach(AppInfo *app, _apps)
    {
        if (!app->isSystemApp())
        {
            apps << app;
        }
    }
    return apps;
}

void AndroidManager::getApplications()
{
    _androidWorker->setTask(AndroidUtils::GetApplications);
    emit gettingApplications();
    _androidWorker->start();
}

void AndroidManager::addApp(AppInfo *app)
{
    app->setParent(this);
    _apps << app;
    connect(app, &AppInfo::urlRemoved, this, &AndroidManager::urlUninstalled);
}

AppInfo* AndroidManager::createUrl(QString url, QString name)
{
    if (name == "")
    {
        QRegularExpression urlRe("(?:([aa-zA-Z0-9]+):\\/\\/)?((?:[aa-zA-Z0-9]+\\.[aa-zA-Z0-9]+)(?:\\.[aa-zA-Z0-9]+)*)");
        QRegularExpressionMatch match = urlRe.match(url);
        if (match.hasMatch()) {
            name = match.captured(2); // domain
        }
    }
    AppInfo *appUrl = new AppInfo(name, url, AppInfo::Url);
    int n = _settings.beginReadArray("urls");
    _settings.endArray();
    _settings.beginWriteArray("urls");
    _settings.setArrayIndex(n);
    _settings.setValue("name", name);
    _settings.setValue("url", url);
    _settings.endArray();
    _settings.sync();

    addApp(appUrl);
    appInstalled(appUrl);
    return appUrl;
}

AppInfo *AndroidManager::takeApp(AppInfo *app)
{
    return takeApp(app->packageName());
}

AppInfo *AndroidManager::takeApp(QString packageName)
{
    for (int i = 0; i < _apps.count(); i++)
    {
        if (packageName == _apps[i]->packageName())
        {
            return _apps.takeAt(i);
        }
    }
    return nullptr;
}

void AndroidManager::showKeyboard()
{
    QInputMethod *keyboard = QGuiApplication::inputMethod();
    keyboard->show();
}

void AndroidManager::updateApplications(QAndroidJniObject *apps)
{
    //remove all
    qDeleteAll(_apps);
    _apps.clear();

    //add to list
    for (int i = 0; i < apps->callMethod<jint>("size"); ++i) {
        QAndroidJniObject a = apps->callObjectMethod("get", "(I)Ljava/lang/Object;", i);
        AppInfo *app = toApp(a);
        addApp(app);
    }
    //add urls
    int n = _settings.beginReadArray("urls");
    for (int i = 0; i < n; i++)
    {
        _settings.setArrayIndex(i);
        AppInfo *url = new AppInfo(
                    _settings.value("name", "WebApp").toString(),
                    _settings.value("url", "").toString(),
                    AppInfo::Url);
        addApp(url);
    }
    _settings.endArray();

    delete apps;
    emit gotApplications();
}

void AndroidManager::urlUninstalled(QString url)
{
    AppInfo *app = takeApp(url);
    if (app)
    {
        qDebug() << "Uninstalled " + app->packageName();

        QList<AppInfo*> urls;
        int n = _settings.beginReadArray("urls");
        for (int i = 0; i < n; i++)
        {
            _settings.setArrayIndex(i);
            QString urlStr = _settings.value("url", "").toString();
            if (url != urlStr)
            {
                AppInfo *url = new AppInfo(
                            _settings.value("name", "WebApp").toString(),
                            urlStr,
                            AppInfo::Url
                            );
                urls << url;
            }
        }
        _settings.endArray();

        _settings.remove("urls");
        _settings.beginWriteArray("urls");
        int i = 0;
        foreach(AppInfo *url, urls)
        {
             _settings.setArrayIndex(i);
             _settings.setValue("name", url->name());
             _settings.setValue("url", url->packageName());
        }
        _settings.endArray();
        appUninstalled(app);
        app->deleteLater();
    }
}

void AndroidManager::androidAppUninstalled(JNIEnv* , jobject, jobject value)
{
    AndroidManager *android = AndroidManager::instance();
    QAndroidJniObject obj(value);
    const QString packageName = obj.getObjectField<jstring>("packageName").toString();
    AppInfo *app = android->takeApp(packageName);
    if (app)
    {
        qDebug() << "Uninstalled " + app->packageName();
        android->appUninstalled(app);
        app->deleteLater();
    }

}

void AndroidManager::androidAppInstalled(JNIEnv*, jobject, jobject value)
{
    AndroidManager *android = AndroidManager::instance();
    AppInfo *app = toApp(value);
    qDebug() << "Installed " + app->packageName();
    android->addApp(app);
    android->appInstalled(app);
}
