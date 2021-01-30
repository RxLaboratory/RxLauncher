#include "appinfo.h"

AppInfo::AppInfo(QString name, QString packageName, Type type, bool systemApp, QObject *parent) : QObject(parent)
{
    _type = type;
    _packageName = packageName;
    settings.beginGroup("apps/" + _packageName);
    _numUses = settings.value("numUses", 0).toInt();
    _iconPath = settings.value("iconPath", ":/icons/app").toString();
    _visible = settings.value("visible", true).toBool();
    _systemApp = settings.value("system", systemApp).toBool();
    _quickAccess = settings.value("quickAccess", false).toBool();
    _name = settings.value("name", name).toString();
    if(_name == "") _name = name;
    _androidLabel = name;
}

AppInfo::~AppInfo()
{

}

int AppInfo::numUses() const
{
    return _numUses;
}

QString AppInfo::iconPath() const
{
    return _iconPath;
}

void AppInfo::setIconPath(const QString &iconPath)
{
    _iconPath = iconPath;
    settings.setValue("iconPath", iconPath);
    emit iconChanged(_iconPath);
}

void AppInfo::launch()
{
    if (_type == Type::App) AndroidUtils::launchApplication(_packageName);
    else AndroidUtils::launchUrl(_packageName);
    _numUses++;
    settings.setValue("numUses", _numUses);
    emit launched();
    qDebug() << "Launched " + _name + " for the " + QString::number(_numUses) + "th time.";
}

void AppInfo::uninstall()
{
    if (_type == Type::App) AndroidUtils::uninstallApplication(_packageName);
    else emit urlRemoved(_packageName);
}

void AppInfo::removeFromLauncher()
{
    emit uninstalled();
    this->deleteLater();
}

QString AppInfo::androidLabel() const
{
    return _androidLabel;
}

void AppInfo::setName(const QString &name)
{
    _name = name;
    if (name == "") _name = _androidLabel;
    settings.setValue("name", name);
    emit nameChanged( _name );
}

AppInfo::Type AppInfo::type() const
{
    return _type;
}

void AppInfo::setType(const Type &type)
{
    _type = type;
}

bool AppInfo::hasQuickAccess() const
{
    return _quickAccess;
}

void AppInfo::setQuickAccess(bool quickAccess)
{
    _quickAccess = quickAccess;
    settings.setValue("quickAccess", quickAccess);
    emit quickAccessChanged(quickAccess);
}

bool AppInfo::visible() const
{
    return _visible;
}

void AppInfo::setVisible(bool visible)
{
    _visible = visible;
    settings.setValue("visible", _visible);
    emit visibilityChanged(_visible);
}

void AppInfo::hide()
{
    _visible = false;
    settings.setValue("visible", _visible);
    emit visibilityChanged(_visible);
}

void AppInfo::show()
{
    _visible = true;
    settings.setValue("visible", _visible);
    emit visibilityChanged(_visible);
}

bool AppInfo::isSystemApp() const
{
    return _systemApp;
}

void AppInfo::setSystemApp(bool system)
{
    _systemApp = system;
    settings.setValue("system", system);
}

QString AppInfo::packageName() const
{
    return _packageName;
}

QString AppInfo::name() const
{
    return _name;
}

