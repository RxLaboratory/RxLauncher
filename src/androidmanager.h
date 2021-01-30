#ifndef ANDROIDMANAGER_H
#define ANDROIDMANAGER_H

#include <QObject>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QThread>
#include <QAndroidJniEnvironment>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "appinfo.h"
#include "androidutils.h"

class AndroidManager : public QObject
{
    Q_OBJECT
public:
    explicit AndroidManager(QObject *parent = nullptr);
    static AndroidManager *instance();

    QList<AppInfo *> apps() const;
    QList<AppInfo *> systemApps();
    QList<AppInfo *> userApps();

public slots:
    void getApplications();
    void addApp(AppInfo *app);
    AppInfo *createUrl(QString url, QString name = "");
    AppInfo *takeApp(AppInfo *app);
    AppInfo *takeApp(QString packageName);

signals:
    void gettingApplications();
    void gotApplications();
    void appInstalled(AppInfo *);
    void appUninstalled(AppInfo *);

private:
    QSettings _settings;
    QList<AppInfo *> _apps;
    AndroidUtils *_androidWorker;
    void registerNativeMethods();
    void registerServices();

private slots:
    void updateApplications(QAndroidJniObject *apps);
    void urlUninstalled(QString url);

protected:
    static AndroidManager *_instance;
    static void androidAppUninstalled(JNIEnv* /*env*/, jobject /*thiz*/, jobject obj);
    static void androidAppInstalled(JNIEnv* /*env*/, jobject /*thiz*/, jobject value);
    static AppInfo * toApp(QAndroidJniObject obj);
    static AppInfo * toApp(jobject obj);
};

#endif // ANDROIDMANAGER_H
