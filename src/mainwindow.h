#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QTimer>
#include <QtDebug>
#include <QDateTime>
#include <QScreen>
#include <QSettings>
#include <QScroller>
#include <QAndroidJniEnvironment>
#include <QJSEngine>

#include "androidmanager.h"
#include "settingswidget.h"
#include "appsettingswidget.h"
#include "appbutton.h"
#include "duqf-app/app-style.h"
#include "appslistwidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    enum ViewType { Grid, List };
    Q_ENUM(ViewType);
    enum LogType { Add, Remove, Settings, Modify, Display, Info, Debug, Important, Warning, Critical, Fatal, Code, CalcResult };
    Q_ENUM(LogType)
public slots:
    void show();
signals:
    void windowShown();
private:
    AppsListWidget *appsListWidget;
    SettingsWidget *settingsWidget;
    AppSettingsWidget *appSettingsWidget;
    QSize appSize;
    QSettings settings;
    AndroidManager *android;
    QList <AppButton *> _quickButtons;
    ViewType _viewType;
    QTimer *_clockTimer;

    void filterAppsList(QString filter="");
private slots:
    void updateTime();
    void finishSetupUi();
    void on_lineEdit_textEdited(const QString &arg1);
    void onKeyboardRectangleChanged();
    void on_clearCmdButton_clicked();
    void on_viewTypeButton_clicked();
    void updateApplications();
    void updateQuickAccess();
    void on_settingsButton_clicked();
    void closeSettings();
    void closeAppSettings();
    void editApplication(AppInfo *app);
    void on_sortButton_clicked();
    void setViewType(ViewType type);
    void setViewType(QString type);
    void showClock(bool show=true);
    void addUrl(QString urlStr);
    // Cmd
    void initCalc();
    void parseLineEdit();
    //Monitoring/console
    void log(QString message, LogType type = LogType::Info);
    void log(QStringList message, LogType type = LogType::Info);
    void appLaunched();
    void appUninstalled(AppInfo *app);
    void appInstalled(AppInfo *app);
    void showConsole(bool show = true);
    void on_consoleSplitter_splitterMoved(int pos, int index);

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
