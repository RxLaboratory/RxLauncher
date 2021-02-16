#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    ProgressSlider *ps = new ProgressSlider;
    ps->setMinimum(1);
    ps->setMaximum(10);
    gridSizeLayout->addWidget(ps);
    connect(ps, &ProgressSlider::valueChanged, this, &SettingsWidget::gridSizeSlider_valueChanged);
    ps->setValue(QSettings().value("numColumns", 4).toInt());
    ps->setFormat("%v");

    ProgressSlider *fs = new ProgressSlider;
    fs->setMinimum(9);
    fs->setMaximum(32);
    fontSizeLayout->addWidget(fs);
    connect(fs, &ProgressSlider::valueChanged, this, &SettingsWidget::fontSizeSlider_valueChanged);
    fs->setValue(QSettings().value("fontSize", 14).toInt());
    fs->setFormat("%vpt");

    showSystemAppsButton->setChecked(QSettings().value("showSystemApps", false).toBool());
    showHiddenAppsButton->setChecked(QSettings().value("showHiddenApps", false).toBool());
    showNamesButton->setChecked(QSettings().value("showNames", true).toBool());
    showClockButton->setChecked(QSettings().value("showClock", true).toBool());

    versionLabel->setText("v1.0.0");
}

void SettingsWidget::on_applyButton_clicked()
{
    urlEdit->setText("");
    emit closeSettings();
}

void SettingsWidget::gridSizeSlider_valueChanged(int value)
{
    settings.setValue("numColumns", value);
}

void SettingsWidget::fontSizeSlider_valueChanged(int value)
{
    settings.setValue("fontSize", value);
}

void SettingsWidget::on_showSystemAppsButton_clicked(bool checked)
{
    settings.setValue("showSystemApps", checked);
}

void SettingsWidget::on_showHiddenAppsButton_clicked(bool checked)
{
    settings.setValue("showHiddenApps", checked);
}

void SettingsWidget::on_showNamesButton_clicked(bool checked)
{
    settings.setValue("showNames", checked);
}

void SettingsWidget::on_showClockButton_clicked(bool checked)
{
    settings.setValue("showClock", checked);
}

void SettingsWidget::on_addUrlButton_clicked()
{
    emit addUrl(urlEdit->text());
    urlEdit->setText("");
}

void SettingsWidget::on_helpButton_clicked()
{
    AndroidUtils::launchUrl("http://rxlauncher-docs.rainboxlab.org");
    on_applyButton_clicked();
}

void SettingsWidget::on_bugButton_clicked()
{
    AndroidUtils::launchUrl("https://github.com/Rainbox-dev/RxLauncher/issues/new/choose");
    on_applyButton_clicked();
}
