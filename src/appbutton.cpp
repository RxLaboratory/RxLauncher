#include "appbutton.h"

AppButton::AppButton(AppInfo *app)
{
    _app = app;

    this->setCheckable(false);

    this->setIcon(QIcon(app->iconPath()));
    this->setText(app->name());

    connect(app, &AppInfo::urlRemoved, this, &QToolButton::hide);
    connect(app, &QObject::destroyed, this, &QObject::deleteLater);
    connect(app, &AppInfo::iconChanged, this, &AppButton::appIconChanged);

    connect(this, &TouchButton::tapped, this, &AppButton::launch);
    connect(this, &TouchButton::longTapped, this, &AppButton::settings);
}

void AppButton::appIconChanged(QString iconPath)
{
    this->setIcon(QIcon(iconPath));
}

AppInfo *AppButton::app() const
{
    return _app;
}

void AppButton::launch()
{
    _app->launch();
}

void AppButton::settings()
{
    emit appSettings(_app);
}
