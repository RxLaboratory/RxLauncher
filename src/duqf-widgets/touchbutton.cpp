#include "touchbutton.h"

TouchButton::TouchButton()
{
    setIconSize(QSize(64,64));
    QFont f = font();
    f.setPixelSize(32);
    setFont(f);

    this->grabGesture(Qt::TapAndHoldGesture);
    this->grabGesture(Qt::TapGesture);
    this->grabGesture(Qt::SwipeGesture);
    this->grabGesture(Qt::PanGesture);
    _shortTap = true;
    _longTap = true;
}

void TouchButton::release()
{
    setDown(false);
    _shortTap = true;
    _longTap = true;
}

bool TouchButton::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
         QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);
         if (QGesture *tapHold = gestureEvent->gesture(Qt::TapAndHoldGesture))
         {
             Qt::GestureState state = tapHold->state();
             if (state == Qt::GestureFinished && !_shortTap)
             {
                 emit longTapped();
                 gestureEvent->accept(tapHold);
                 _longTap = true;
                 release();
             }
         }

         if (QGesture *tap = gestureEvent->gesture(Qt::TapGesture))
         {
             Qt::GestureState state = tap->state();
             if (state == Qt::GestureFinished && !_longTap)
             {
                 if (_longTap) return QToolButton::event(event);

                 emit tapped();
                 _shortTap = true;
                 gestureEvent->accept(tap);
                 release();
             }

             if (state == Qt::GestureStarted)
             {
                 _shortTap = false;
                 _longTap = false;
                 setDown(true);
                 QTimer::singleShot(2000, this, &TouchButton::release);
             }
         }

         if (QGesture *swipe = gestureEvent->gesture(Qt::SwipeGesture))
         {
             release();
         }

         if (QGesture *pand = gestureEvent->gesture(Qt::PanGesture))
         {
             release();
         }
     }

     return QToolButton::event(event);
}
