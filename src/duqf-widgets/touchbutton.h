#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <QToolButton>
#include <QGestureEvent>
#include <QTimer>

class TouchButton : public QToolButton
{
    Q_OBJECT
public:
    TouchButton();
signals:
    void longTapped();
    void tapped();
private slots:
    void release();
private:
    bool _shortTap;
    bool _longTap;
protected:
    bool event(QEvent *event);
};

#endif // TOUCHBUTTON_H
