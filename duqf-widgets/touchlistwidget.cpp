#include "touchlistwidget.h"

TouchListWidget::TouchListWidget()
{
    this->setEditTriggers(QListWidget::NoEditTriggers);
    this->setDragDropMode(QListWidget::NoDragDrop);
    this->setIconSize(QSize(64,64));
    this->setSelectionMode(QListWidget::SingleSelection);
    QFont font = this->font();
    font.setPointSize(16);
    this->setFont(font);
    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    this->grabGesture(Qt::TapAndHoldGesture);
    this->grabGesture(Qt::TapGesture);
    _simpleTap = false;
    _numColumns = 4;
}

TouchListWidget::ViewType TouchListWidget::viewType() const
{
    return _viewType;
}

void TouchListWidget::setViewType(const ViewType &viewType)
{
    if (viewType == TouchListWidget::Icon)
    {
        this->setViewMode(QListView::IconMode);
        this->setMovement(QListView::Static);
        int size = (this->viewport()->width()-40)/_numColumns;
        this->setGridSize(QSize(size,168));
    }
    else
    {
        this->setViewMode(QListView::IconMode);
        this->setMovement(QListView::Static);
        int size = (this->viewport()->width()-40);
        this->setGridSize(QSize(size, 168));
    }
    _viewType = viewType;
}

int TouchListWidget::numColumns() const
{
    return _numColumns;
}

void TouchListWidget::setNumColumns(int numColumns)
{
    _numColumns = numColumns;
}

bool TouchListWidget::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);
         if (QGesture *tapHold = gestureEvent->gesture(Qt::TapAndHoldGesture))
         {
             Qt::GestureState state = tapHold->state();
             if (state == Qt::GestureFinished)
             {
                 emit itemTappedAndHolded(this->currentItem());
                 gestureEvent->accept(tapHold);
                 _simpleTap = false;
             }
         }
         else if (QGesture *tap = gestureEvent->gesture(Qt::TapGesture))
         {
             Qt::GestureState state = tap->state();
             if (state == Qt::GestureFinished)
             {
                 if (_simpleTap) emit itemTapped(this->currentItem());
                 gestureEvent->accept(tap);
             }
             else if (state == Qt::GestureStarted)
             {
                 _simpleTap = true;
                 QTapGesture *tapGesture = static_cast<QTapGesture*>(tap);
                 QListWidgetItem *item = this->itemAt(tapGesture->position().toPoint());
                 if (item) this->setCurrentItem(item);
             }
         }

         return true;
     }

     return QListWidget::event(event);
}

