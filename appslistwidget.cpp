#include "appslistwidget.h"

bool sortByName(AppButton *b1,AppButton *b2)
{
    return b1->app()->name().toLower() < b2->app()->name().toLower();
}

bool sortByUsage(AppButton *b1, AppButton *b2)
{
    int uses1 = b1->app()->numUses();
    int uses2 = b2->app()->numUses();
    if (uses1 == uses2) return sortByName(b1, b2);

    return uses1 > uses2;
}

AppsListWidget::AppsListWidget(AndroidManager *android, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    _android = android;
    _numColumns = settings.value("numColumns", 4).toInt();
    _numButtons = 0;
    setViewType(settings.value("viewType", "Grid").toString());
    setSortingType(settings.value("sortingType", "Name").toString());
    QScroller::grabGesture(scrollArea, QScroller::LeftMouseButtonGesture);

    connect(_android, &AndroidManager::gettingApplications, this, &AppsListWidget::gettingApplications);
    connect(_android, &AndroidManager::gotApplications, this, &AppsListWidget::updateApplications);
    connect(_android, &AndroidManager::appUninstalled, this, &AppsListWidget::removeApp);
    connect(_android, &AndroidManager::appInstalled, this, &AppsListWidget::addApp);
}

AppButton *AppsListWidget::addApp(AppInfo *app)
{
    AppButton *appButton = new AppButton(app);
    _buttons << appButton;
    connect(appButton, &AppButton::appSettings, this, &AppsListWidget::appSettings);
    connect(app, SIGNAL(launched()), this, SLOT(layout()));
    return appButton;
}

void AppsListWidget::removeApp(AppInfo *app)
{
    for(int i = _buttons.count() -1; i >= 0; i--)
    {
        if (_buttons.at(i)->app() == app)
        {
            _buttons.takeAt(i)->deleteLater();
        }
    }
    layout();
}

void AppsListWidget::appendAppButton(AppButton *appButton)
{
    if (!settings.value("showSystemApps", false).toBool() && appButton->app()->isSystemApp()) return;
    if (!settings.value("showHiddenApps", false).toBool() && !appButton->app()->visible()) return;
    appButton->show();
    if (_viewType == ViewType::List)
    {
        if (settings.value("showNames", true).toBool()) appButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        else appButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        appButton->setText(" " + appButton->app()->name());
        gridLayout->addWidget(appButton, _numButtons, 0, Qt::AlignLeft);
    }
    else
    {
        if (settings.value("showNames", true).toBool()) appButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        else appButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        appButton->setText(appButton->app()->name());
        gridLayout->addWidget(appButton, _numButtons/_numColumns, _numButtons%_numColumns, Qt::AlignCenter);
    }
    _numButtons++;
}

void AppsListWidget::updateApplications()
{
    deleteAll();
    foreach(AppInfo *app, _android->apps())
    {
        addApp(app);
    }
    layout();
}

void AppsListWidget::gettingApplications()
{
    deleteAll();
    loadingLabel->show();
}

void AppsListWidget::layout()
{
    layout("");
}

void AppsListWidget::layout(QString filter)
{
    removeAll();
    loadingLabel->show();
    _numColumns = settings.value("numColumns", 4).toInt();
    int size = (scrollArea->width()-60)/_numColumns;

    if (_sortingType == SortingType::Name) std::sort(_buttons.begin(),_buttons.end(),sortByName);
    else std::sort(_buttons.begin(),_buttons.end(),sortByUsage);

    foreach (AppButton *button, _buttons)
    {
        AppInfo *app = button->app();
        QString name = app->name();
        QString package = app->packageName();
        if ( name.contains(filter, Qt::CaseInsensitive) || package.contains(filter, Qt::CaseInsensitive))
        {
            if (_viewType == ViewType::Grid) button->setMaximumWidth(size);
            else button->setMaximumWidth(100000);
            appendAppButton(button);
        }
    }
    loadingLabel->hide();
}

void AppsListWidget::deleteAll()
{
    foreach (AppButton *button, _buttons)
    {
        gridLayout->removeWidget(button);
    }
    qDeleteAll(_buttons);
    _buttons.clear();
}

void AppsListWidget::removeAll()
{
    foreach (AppButton *button, _buttons)
    {
        gridLayout->removeWidget(button);
        button->hide();
    }
    _numButtons = 0;
}

int AppsListWidget::numColumns() const
{
    return _numColumns;
}

void AppsListWidget::setNumColumns(int numColumns)
{
    _numColumns = numColumns;
    layout();
    settings.setValue("numColumns", numColumns);
}

void AppsListWidget::showNames(bool show)
{
    foreach(AppButton *button, _buttons)
    {
        if (show && _viewType == ViewType::Grid)
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        else if (show)
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        else
            button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

AppsListWidget::ViewType AppsListWidget::viewType() const
{
    return _viewType;
}

void AppsListWidget::setViewType(const ViewType &viewType)
{
    _viewType = viewType;
    layout();
    if (viewType == ViewType::Grid) settings.setValue("viewType", "Grid");
    else settings.setValue("viewType", "List");
}

void AppsListWidget::setViewType(QString type)
{
    if (type == "Grid") setViewType(ViewType::Grid);
    else setViewType(ViewType::List);
}

AppsListWidget::SortingType AppsListWidget::sortingType() const
{
    return _sortingType;
}

void AppsListWidget::setSortingType(const SortingType &sortingType)
{
    _sortingType = sortingType;
    layout();
    if (sortingType == SortingType::Name) settings.setValue("sortingType", "Name");
    else settings.setValue("sortingType", "Usage");
}

void AppsListWidget::setSortingType(QString type)
{
    if (type == "Name") setSortingType(SortingType::Name);
    else setSortingType(SortingType::Usage);
}

