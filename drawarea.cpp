#include <QPainter>
#include <QPoint>
#include "drawarea.h"

void DrawArea::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    double wFactor = double(width()) / (stepsX - 1);
    double hFactor = .5 * height();
    QPoint oldPoint, newPoint;

    for (unsigned stepX = 0; stepX < stepsX - 1; ++stepX) {
        oldPoint.setX(wFactor * stepX);
        oldPoint.setY(hFactor * (1. - data[stepT * stepsX + stepX]));
        newPoint.setX(wFactor * (stepX + 1));
        newPoint.setY(hFactor * (1. - data[stepT * stepsX + stepX + 1]));
        painter.drawLine(oldPoint, newPoint);
    }
}
