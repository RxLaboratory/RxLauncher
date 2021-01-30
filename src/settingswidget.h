#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "ui_settingswidget.h"
#include "duqf-widgets/progressslider.h"
#include "androidutils.h"

#include <QSettings>

class SettingsWidget : public QWidget, private Ui::SettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
signals:
    void closeSettings();
    void addUrl(QString);
private slots:
    void on_applyButton_clicked();
    void gridSizeSlider_valueChanged(int value);
    void fontSizeSlider_valueChanged(int value);
    void on_showSystemAppsButton_clicked(bool checked);
    void on_showHiddenAppsButton_clicked(bool checked);
    void on_showNamesButton_clicked(bool checked);
    void on_showClockButton_clicked(bool checked);
    void on_addUrlButton_clicked();
    void on_helpButton_clicked();
    void on_bugButton_clicked();

private:
    QSettings settings;
};

#endif // SETTINGSWIDGET_H
