#include "appsettingswidget.h"

AppSettingsWidget::AppSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    _app = nullptr;

    iconSelector = new TouchListWidget;
    addIconDirToSelector(":/appIcons/resources/appIcons/");
    addIconDirToSelector(":/appIcons/resources/icons/");
    iconSelector->setViewType(TouchListWidget::Icon);
    iconSelectorLayout->insertWidget(0,iconSelector,1);

    connect(iconSelector, &TouchListWidget::itemTapped, this, &AppSettingsWidget::internalIconSelected);


    stackedWidget->setCurrentIndex(0);
    _iconPath = ":/icons/app";
}

void AppSettingsWidget::setApp(AppInfo *app)
{
    _app = app;
    _iconPath = app->iconPath();
    iconButton->setIcon(QIcon(_iconPath));
    nameEdit->setPlaceholderText(app->androidLabel());
    if (app->name() != app->androidLabel()) nameEdit->setText(app->name());
    else nameEdit->setText("");
    packageNameLabel->setText(app->packageName());
    if (app->isSystemApp())
    {
        userAppButton->setText("System application");
        userAppButton->setChecked(false);
    }
    else
    {
        userAppButton->setText("User application");
        userAppButton->setChecked(true);
    }
    if (app->visible())
    {
        hideButton->setChecked(true);
        hideButton->setText("Visible");
    }
    else
    {
        hideButton->setChecked(false);
        hideButton->setText("Hidden");
    }

    quickAccessButton->setChecked(app->hasQuickAccess());

}

void AppSettingsWidget::on_applyButton_clicked()
{
    _app->setIconPath(_iconPath);
    _app->setSystemApp(!userAppButton->isChecked());
    _app->setVisible(hideButton->isChecked());
    _app->setQuickAccess(quickAccessButton->isChecked());
    _app->setName(nameEdit->text());
    emit closed();
}

void AppSettingsWidget::on_hideButton_clicked(bool checked)
{
    if (!checked) hideButton->setText("Hidden");
    else hideButton->setText("Visible");
}

AppInfo *AppSettingsWidget::app() const
{
    return _app;
}

void AppSettingsWidget::on_userAppButton_clicked(bool checked)
{
    if (!checked)
    {
        userAppButton->setText("System application");
    }
    else
    {
        userAppButton->setText("User application");
    }
}

void AppSettingsWidget::on_closeButton_clicked()
{
    emit closed();
}

void AppSettingsWidget::on_iconBrowseButton_clicked()
{
    QString f = QFileDialog::getOpenFileName(this, "Select icon");
    if (!f.isNull() && _app != nullptr)
    {
        _iconPath = f;
        iconButton->setIcon(QIcon(f));
    }
}

void AppSettingsWidget::on_iconResetButton_clicked()
{
    _iconPath = ":/icons/app";
    iconButton->setIcon(QIcon(":/icons/app"));
}

void AppSettingsWidget::on_iconInternalButton_clicked()
{
    stackedWidget->setCurrentIndex(1);

}

void AppSettingsWidget::internalIconSelected(QListWidgetItem *item)
{
    QString path = item->data(Qt::UserRole).toString();
    _iconPath = path;
    iconButton->setIcon(QIcon(path));
    stackedWidget->setCurrentIndex(0);
}

void AppSettingsWidget::addIconDirToSelector(QString dirPath)
{
    QDir appIconsDir(dirPath);
    QFileInfoList iconFiles = appIconsDir.entryInfoList(QDir::Files);
    foreach(QFileInfo iconFile, iconFiles)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(iconFile.absoluteFilePath()));
        item->setData(Qt::UserRole, iconFile.absoluteFilePath());
        iconSelector->addItem(item);
    }
}

void AppSettingsWidget::filter(QString f)
{
    for(int row=0; row < iconSelector->count(); row++)
    {
        QListWidgetItem *item = iconSelector->item(row);
        QString iconPath = item->data(Qt::UserRole).toString();
        if (f == "" || iconPath.toLower().contains(f.toLower())) item->setHidden(false);
        else item->setHidden(true);
    }
}

void AppSettingsWidget::on_uninstallButton_clicked()
{
    _app->uninstall();
    emit closed();
}

void AppSettingsWidget::on_closeIconsButton_clicked()
{
    stackedWidget->setCurrentIndex(0);
}

void AppSettingsWidget::on_lineEdit_editingFinished()
{
    filter(lineEdit->text());
}

void AppSettingsWidget::on_clearCmdButton_clicked()
{
    lineEdit->setText("");
    filter("");
}

void AppSettingsWidget::on_searchButton_clicked()
{
    filter(lineEdit->text());
}
