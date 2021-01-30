#ifndef APPBUTTON_H
#define APPBUTTON_H

#include "appinfo.h"
#include "androidutils.h"
#include "duqf-widgets/touchbutton.h"

class AppButton : public TouchButton
{
    Q_OBJECT
public:
    AppButton(AppInfo *app);
    AppInfo *app() const;
public slots:
    void launch();
    void settings();
signals:
    void appSettings(AppInfo *app);
private slots:
    void appIconChanged(QString iconPath);
private:
    AppInfo *_app;
};

#endif // APPBUTTON_H
