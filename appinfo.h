#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>
#include <QSettings>
#include <QIcon>

#include "androidutils.h"

class AppInfo : public QObject
{
    Q_OBJECT
public:
    enum Type { Url, App };

    explicit AppInfo(QString name, QString packageName, Type type=Type::App, bool systemApp = false, QObject *parent = nullptr);
    ~AppInfo();
    int numUses() const;
    QString iconPath() const;
    void setIconPath(const QString &iconPath);
    QString name() const;
    void setName(const QString &name);
    QString packageName() const;
    bool isSystemApp() const;
    void setSystemApp(bool system=true);

    bool visible() const;
    void setVisible(bool visible);
    void hide();
    void show();

    bool hasQuickAccess() const;
    void setQuickAccess(bool quickAccess);

    Type type() const;
    void setType(const Type &type);

    QString androidLabel() const;

public slots:
    void launch();
    void uninstall();
    void removeFromLauncher();

signals:
    void iconChanged(QString);
    void visibilityChanged(bool);
    void quickAccessChanged(bool);
    void launched();
    void uninstalled();
    void urlRemoved(QString url);
    void nameChanged(QString name);

private:
    int _numUses;
    QString _iconPath;
    QString _name;
    QString _androidLabel;
    QString _packageName;
    QSettings settings;
    bool _systemApp;
    bool _visible;
    bool _quickAccess;
    Type _type;
};

#endif // APPINFO_H
