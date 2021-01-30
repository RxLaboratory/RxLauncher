#ifndef APPSLISTWIDGET_H
#define APPSLISTWIDGET_H

#include "ui_appslistwidget.h"

#include <QScroller>

#include "appbutton.h"
#include "androidmanager.h"

class AppsListWidget : public QWidget, private Ui::AppsListWidget
{
    Q_OBJECT

public:
    explicit AppsListWidget(AndroidManager *android,QWidget *parent = nullptr);

    enum ViewType { Grid, List };
    Q_ENUM(ViewType)

    enum SortingType { Name, Usage };
    Q_ENUM(SortingType)

    AppButton *addApp(AppInfo *app);

    void deleteAll();

    int numColumns() const;
    ViewType viewType() const;
    SortingType sortingType() const;

    void layout(QString filter);

public slots:
    void showNames(bool show);
    void setNumColumns(int numColumns);
    void setViewType(const ViewType &viewType);
    void setViewType(QString type);
    void setSortingType(const SortingType &sortingType);
    void setSortingType(QString type);
    void layout();
    void removeApp(AppInfo *app);


signals:
    void appSettings(AppInfo *app);

private:
    int _numColumns;
    int _numButtons;
    ViewType _viewType;
    QSettings settings;
    AndroidManager *_android;
    SortingType _sortingType;

    QList<AppButton *> _buttons;

    void removeAll();
    void appendAppButton(AppButton *appButton);

private slots:
    void updateApplications();
    void gettingApplications();

protected:

};

#endif // APPSLISTWIDGET_H
