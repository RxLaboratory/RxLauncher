#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    consoleEdit->setFontPointSize(settings.value("fontSize", 14).toInt());
    log("Starting RxLauncher...");

    stackedWidget->setCurrentIndex(1);

    settingsWidget = new SettingsWidget;
    settingsLayout->addWidget(settingsWidget);

    appSettingsWidget = new AppSettingsWidget;
    appSettingsLayout->addWidget(appSettingsWidget);

    DuUI::updateCSS(":/styles/default");
    DuUI::setFont("Ubuntu");
    AndroidUtils::setAndroidBGColor("#1c1c1c");

    //init
    appSize = qApp->screens()[0]->availableGeometry().size();

    //launch clock timer
    _clockTimer = new QTimer(this);
    connect(_clockTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    showClock(settings.value("showClock", true).toBool());

    android = new AndroidManager(this);
    appsListWidget = new AppsListWidget(android);
    mainLayout->addWidget(appsListWidget);
    if (appsListWidget->sortingType() == AppsListWidget::Name) sortButton->setIcon(QIcon(":/icons/sortUsage"));
    else sortButton->setIcon(QIcon(":/icons/sortName"));
    setViewType(settings.value("viewType", "Grid").toString());

    bool showConsole = settings.value("showConsole", false).toBool();
    consoleButton->setChecked(showConsole);
    on_consoleButton_clicked(showConsole);

    mainLayout->setStretch(0,25);
    mainLayout->setStretch(1,75);

    QScroller::grabGesture(quickScrollArea, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(consoleEdit, QScroller::LeftMouseButtonGesture);

    //connections
    connect(QApplication::inputMethod(), &QInputMethod::keyboardRectangleChanged, this, &MainWindow::onKeyboardRectangleChanged);
    connect(this, &MainWindow::windowShown, this, &MainWindow::finishSetupUi);
    connect(settingsWidget, &SettingsWidget::closeSettings, this, &MainWindow::closeSettings);
    connect(settingsWidget, &SettingsWidget::addUrl, this, &MainWindow::addUrl);
    connect(appSettingsWidget, &AppSettingsWidget::closed, this, &MainWindow::closeAppSettings);
    connect(android, &AndroidManager::gotApplications, this, &MainWindow::updateApplications);
    connect(android, &AndroidManager::appInstalled, this, &MainWindow::appInstalled);
    connect(android, SIGNAL(appUninstalled(AppInfo*)), this, SLOT(appUninstalled(AppInfo*)));
    connect(appsListWidget, &AppsListWidget::appSettings, this, &MainWindow::editApplication);

    // List apps
    android->getApplications();

    log("RxLauncher ready.");
}

void MainWindow::show()
{
    QMainWindow::show();
    QApplication::processEvents();
    emit windowShown();
}

void MainWindow::updateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    timeLabel->setText(currentDateTime.toString("ddd dd MMMM - hh:mm:ss"));
}

void MainWindow::finishSetupUi()
{
    //nothing
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    appsListWidget->layout(arg1);
}

void MainWindow::onKeyboardRectangleChanged()
{
    QInputMethod *inputMethod = QApplication::inputMethod();
    bool keyboardVisible = inputMethod->isVisible();

    if (!keyboardVisible)
    {
        this->resize(appSize);
        this->move(0,0);
    }
    else
    {
        QRectF keyboardRectangle = inputMethod->keyboardRectangle();
        int keyboardHeight = int(keyboardRectangle.height());
        this->resize(appSize.width(), appSize.height() - keyboardHeight - 15);
        if (stackedWidget->currentIndex() == 1 || stackedWidget->currentIndex() == 3) this->move(0,keyboardHeight);
    }
}

void MainWindow::on_clearCmdButton_clicked()
{
    lineEdit->setText("");
    appsListWidget->layout("");
}

void MainWindow::on_viewTypeButton_clicked()
{
    if (_viewType == ViewType::List) setViewType(ViewType::Grid);
    else setViewType(ViewType::List);

}

void MainWindow::on_sortButton_clicked()
{
    if (appsListWidget->sortingType() == AppsListWidget::Name)
    {
        appsListWidget->setSortingType(AppsListWidget::Usage);
        sortButton->setIcon(QIcon(":/icons/sortName"));
    }
    else
    {
        appsListWidget->setSortingType(AppsListWidget::Name);
        sortButton->setIcon(QIcon(":/icons/sortUsage"));
    }
}

void MainWindow::setViewType(MainWindow::ViewType type)
{
    _viewType = type;
    if (_viewType == ViewType::List)
    {
        appsListWidget->setViewType(AppsListWidget::List);
        viewTypeButton->setIcon(QIcon(":/icons/grid"));
        settings.setValue("viewType", "List");
    }
    else
    {
        appsListWidget->setViewType(AppsListWidget::Grid);
        viewTypeButton->setIcon(QIcon(":/icons/list"));
        settings.setValue("viewType", "Grid");
    }
}

void MainWindow::setViewType(QString type)
{
    if (type == "List") setViewType(ViewType::List);
    else setViewType(ViewType::Grid);
}

void MainWindow::showClock(bool show)
{
    if (show)
    {
        _clockTimer->start(1000);
        updateTime();
        timeLabel->show();
    }
    else
    {
        _clockTimer->stop();
        timeLabel->hide();
    }
}

void MainWindow::addUrl(QString urlStr)
{
    AppInfo *url = android->createUrl(urlStr);
    closeSettings();
    editApplication(url);
}

void MainWindow::log(QString message, LogType type)
{
    consoleEdit->setTextColor(QColor(227,227,227));
    consoleEdit->setFontWeight(400);
    consoleEdit->setFontItalic(false);
    QString time = "[" + QLocale::system().toString(QDateTime::currentDateTime(), QLocale::ShortFormat) + "] ";
    consoleEdit->append(time);
    if (type == LogType::Add)
    {
        consoleEdit->setTextColor(QColor(138,216,145));
    }
    else if (type == LogType::Remove)
    {
        consoleEdit->setTextColor(QColor(249,105,105));
    }
    else if (type == LogType::Settings)
    {
        consoleEdit->setTextColor(QColor(131,211,246));
    }
    else if (type == LogType::Modify)
    {
        consoleEdit->setTextColor(QColor(213,136,241));
    }
    else if (type == LogType::Display)
    {
        consoleEdit->setTextColor(QColor(216,255,140));
    }
    else if (type == LogType::Important)
    {
        consoleEdit->setTextColor(QColor(236,215,24));
    }
    else if (type == LogType::Warning)
    {
        consoleEdit->setTextColor(QColor(249,105,105));
    }
    else if (type == LogType::Critical)
    {
        consoleEdit->setTextColor(QColor(241,136,186));
    }
    else if (type == LogType::Fatal)
    {
        consoleEdit->setTextColor(QColor(255,255,255));
    }
    consoleEdit->setFontWeight(800);
    consoleEdit->insertPlainText(message);
}

void MainWindow::log(QStringList message, LogType type)
{
    if (message.count() == 0) return;
    log(message[0], type);
    consoleEdit->setFontWeight(400);
    consoleEdit->setTextColor(QColor(227,227,227));
    consoleEdit->setFontItalic(true);
    for (int i = 1; i < message.count(); i++)
    {
        consoleEdit->append(message[i]);
    }
}

void MainWindow::appLaunched()
{
    AppInfo *app = static_cast<AppInfo*>(sender());
    QStringList message("Launched " + app->name());
    message << "(" + app->packageName() + ")";
    message << QString::number(app->numUses()) + " times.";
    log(message, LogType::Display);
    lineEdit->setText("");
    appsListWidget->layout();
}

void MainWindow::appUninstalled(AppInfo *app)
{
    //if it's in the quick access, remove it
    for (int i = _quickButtons.count() -1; i >= 0; i-- )
    {
        if (_quickButtons[i]->app() == app)
        {
            AppButton *button = _quickButtons.takeAt(i);
            quickAccessLayout->removeWidget(button);
            button->hide();
            delete button;
        }
    }
    QString name = app->name();
    QString packageName = app->packageName();
    QStringList m("App uninstalled: " + name);
    if (packageName != "") m << "(" + packageName + ")";
    log(m, LogType::Remove);
}

void MainWindow::appInstalled(AppInfo *app)
{
    QString name = app->name();
    QString packageName = app->packageName();
    QStringList m("App installed: " + name);
    if (packageName != "") m << "(" + packageName + ")";
    log(m, LogType::Add);
    //update quick access in case the app has to be in it
    updateQuickAccess();
}

void MainWindow::editApplication(AppInfo *app)
{
    log("Opening \""+app->name()+"\" settings...", LogType::Settings);
    appSettingsWidget->setApp(app);
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::updateApplications()
{
    updateQuickAccess();
    foreach(AppInfo *app, android->apps())
    {
        connect(app, &AppInfo::launched, this, &MainWindow::appLaunched);
    }
    log("Updated applications list.", LogType::Settings);
}

void MainWindow::updateQuickAccess()
{
    for (int i = _quickButtons.count() -1; i >= 0; i-- )
    {
        AppButton *button = _quickButtons.takeAt(i);
        quickAccessLayout->removeWidget(button);
        button->hide();
        delete button;
    }
    _quickButtons.clear();

    foreach(AppInfo *app, android->apps())
    {
        if (app->hasQuickAccess())
        {
            AppButton *button = new AppButton(app);
            connect(button, SIGNAL(appSettings(AppInfo*)), this, SLOT(editApplication(AppInfo*)));
            quickAccessLayout->addWidget(button);
            _quickButtons << button;
        }
    }
}

void MainWindow::on_settingsButton_clicked()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::closeSettings()
{
    appsListWidget->layout(lineEdit->text());
    log("Settings saved.", LogType::Settings);
    showClock(settings.value("showClock", true).toBool());
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::closeAppSettings()
{
    appsListWidget->layout(lineEdit->text());
    updateQuickAccess();
    consoleEdit->setFontPointSize(settings.value("fontSize", 14).toInt());
    log("Application settings saved.", LogType::Modify);
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_consoleButton_clicked(bool checked)
{
    if (checked)
    {
        QList<int>sizes;
        sizes << settings.value("consoleSize",15).toInt();
        sizes << settings.value("listSize",85).toInt();
        consoleSplitter->setSizes(sizes);
    }
    else
    {
        QList<int>sizes;
        sizes << settings.value("consolesize",0).toInt();
        sizes << settings.value("listSize",100).toInt();
        consoleSplitter->setSizes(sizes);
    }
    settings.setValue("showConsole", checked);
}

void MainWindow::on_consoleSplitter_splitterMoved(int pos, int index)
{
    QList<int> sizes = consoleSplitter->sizes();
    if (sizes[0] > 100)
    {
        settings.setValue("consoleSize", sizes[0]);
        settings.setValue("listSize", sizes[1]);
    }

    bool showConsole = sizes[0] != 0;
    consoleButton->setChecked(showConsole);
    settings.setValue("showConsole", showConsole);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
