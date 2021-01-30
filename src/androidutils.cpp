#include "androidutils.h"

AndroidUtils::AndroidUtils()
{
    _task = GetApplications;
}

void AndroidUtils::launchApplication(QString packageName)
{
    QAndroidJniObject packageNameString = QAndroidJniObject::fromString(packageName);
#ifdef QT_DEBUG
    qDebug() << packageNameString.toString();
#endif
    QAndroidJniObject::callStaticMethod<void>("org.rxlab.launcher.AppManager",
                                              "launchApp",
                                              "(Ljava/lang/String;Landroid/content/Context;)V",
                                              packageNameString.object<jstring>(),
                                              QtAndroid::androidContext().object());

}

void AndroidUtils::launchUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void AndroidUtils::uninstallApplication(QString packageName)
{
    QAndroidJniObject packageNameString = QAndroidJniObject::fromString(packageName);
#ifdef QT_DEBUG
    qDebug() << packageNameString.toString();
#endif
    QAndroidJniObject::callStaticMethod<void>("org.rxlab.launcher.AppManager",
                                              "deleteApp",
                                              "(Ljava/lang/String;Landroid/content/Context;)V",
                                              packageNameString.object<jstring>(),
                                              QtAndroid::androidContext().object());

}

void AndroidUtils::setAndroidBGColor(QString color)
{
    QtAndroid::runOnAndroidThread([=]()
    {
        QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
        window.callMethod<void>("addFlags", "(I)V", 0x80000000);
        window.callMethod<void>("clearFlags", "(I)V", 0x04000000);
        window.callMethod<void>("setStatusBarColor", "(I)V", 0xff1c1c1c); // Desired statusbar color
        window.callMethod<void>("setNavigationBarColor", "(I)V", 0xff1c1c1c); // Desired navigationbar color
        QAndroidJniObject decorView = window.callObjectMethod("getDecorView", "()Landroid/view/View;");
        decorView.callMethod<void>("setSystemUiVisibility", "(I)V", 0x00000000);
    });

}

void AndroidUtils::run()
{
    if (_task == GetApplications) getApplications();
}

void AndroidUtils::getApplications()
{
    QAndroidJniObject *apps = new QAndroidJniObject(
        QAndroidJniObject::callStaticObjectMethod("org.rxlab.launcher.AppManager",
                                                 "getAllApps",
                                                 "(Landroid/content/Context;)Ljava/util/ArrayList;",
                                                  QtAndroid::androidContext().object()) );
    emit gotApplications(apps);
}

AndroidUtils::Task AndroidUtils::getTask() const
{
    return _task;
}

void AndroidUtils::setTask(const Task &task)
{
    _task = task;
}
