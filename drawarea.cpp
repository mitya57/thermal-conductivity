#include <QPainter>
#include <QPoint>
#include "drawarea.h"

void DrawArea::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    double w = width(), h = height();
    QPoint oldPoint, newPoint;

    for (unsigned stepX = 0; stepX < stepsX - 1; ++stepX) {
        oldPoint.setX(w * stepX / stepsX);
        oldPoint.setY(h * (.5 - data[stepT * stepsX + stepX] / 2));
        newPoint.setX(w * (stepX + 1) / stepsX);
        newPoint.setY(h * (.5 - data[stepT * stepsX + stepX + 1] / 2));
        painter.drawLine(oldPoint, newPoint);
    }
}
