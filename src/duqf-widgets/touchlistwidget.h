#ifndef TOUCHLISTWIDGET_H
#define TOUCHLISTWIDGET_H

#include <QListWidget>
#include <QScroller>
#include <QMouseEvent>
#include <QFont>
#include <QGestureEvent>
#include <QDebug>

class TouchListWidget : public QListWidget
{
    Q_OBJECT
public:
    TouchListWidget();

    enum ViewType { List, Icon };
    Q_ENUM(ViewType)
    ViewType viewType() const;
    void setViewType(const ViewType &viewType);

    int numColumns() const;
    void setNumColumns(int numColumns);

signals:
    void itemTapped(QListWidgetItem *item);
    void itemTappedAndHolded(QListWidgetItem *item);
private:
    bool _simpleTap;
    ViewType _viewType;
    int _numColumns;
protected:
    bool event(QEvent *event);

};

#endif // TOUCHLISTWIDGET_H
