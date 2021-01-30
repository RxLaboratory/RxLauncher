#ifndef APPSETTINGSWIDGET_H
#define APPSETTINGSWIDGET_H

#include "ui_appsettingswidget.h"
#include "appinfo.h"
#include "duqf-widgets/touchlistwidget.h"
#include "androidutils.h"
#include <QFileDialog>
#include <QtDebug>

class AppSettingsWidget : public QWidget, private Ui::AppSettingsWidget
{
    Q_OBJECT

public:
    explicit AppSettingsWidget(QWidget *parent = nullptr);
    void setApp(AppInfo *app);
    AppInfo *app() const;

signals:
    void closed();

private slots:
    void on_applyButton_clicked();
    void on_hideButton_clicked(bool checked);
    void on_userAppButton_clicked(bool checked);
    void on_closeButton_clicked();
    void on_iconBrowseButton_clicked();
    void on_iconResetButton_clicked();
    void on_iconInternalButton_clicked();
    void internalIconSelected(QListWidgetItem *item);
    void on_uninstallButton_clicked();
    void on_closeIconsButton_clicked();
    void on_lineEdit_editingFinished();
    void on_clearCmdButton_clicked();
    void on_searchButton_clicked();

private:
    AppInfo *_app;
    TouchListWidget *iconSelector;
    QString _iconPath;
    void addIconDirToSelector(QString dirPath);
    void filter(QString f="");
};

#endif // APPSETTINGSWIDGET_H
