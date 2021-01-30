#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <QAndroidJniObject>
#include <QtAndroid>
#include <QString>
#include <QThread>
#include <QtDebug>
#include <QDesktopServices>
#include <QUrl>

class AndroidUtils : public QThread
{
    Q_OBJECT

public:
    AndroidUtils();

    enum Task { GetApplications };
    Q_ENUM(Task)

    static void launchApplication(QString packageName);
    static void launchUrl(QString url);
    static void uninstallApplication(QString packageName);
    static void setAndroidBGColor(QString color);

    Task getTask() const;
    void setTask(const Task &task);

signals:
    void gotApplications(QAndroidJniObject *);

private:
    void run();
    void getApplications();
    Task _task;
};

#endif // ANDROIDUTILS_H
