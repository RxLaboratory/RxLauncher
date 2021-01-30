#include "progressslider.h"

ProgressSlider::ProgressSlider()
{
    setFormat("");
}

void ProgressSlider::mouseMoveEvent(QMouseEvent *event)
{
    double newVal;
    if (orientation() == Qt::Vertical)
       newVal = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
    else
       newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();

    if (invertedAppearance() == true)
        newVal = maximum() - newVal;

    if (newVal < minimum()) newVal = minimum();
    if (newVal > maximum()) newVal = maximum();

    setValue(newVal);
    emit valueChanged(newVal);

    event->accept();

    QProgressBar::mousePressEvent(event);
}
