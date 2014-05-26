#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <cmath>
#include "drawarea.h"

void saveToPng(unsigned stepsX,
               unsigned stepsT,
               double  *data) {
    const unsigned w = 640;
    const unsigned h = 480;
    QPixmap pixmap(QSize(w, h));
    pixmap.fill();
    QPainter painter(&pixmap);
    double wFactor = double(w) / (stepsX - 1);
    double hFactor = .5 * h;
    QPoint oldPoint, newPoint;

    for (unsigned drawT = 0; drawT < stepsT; ++drawT) {
        painter.setPen(QColor(0, 0, 0, 255 * exp(-.2 * drawT)));

        for (unsigned stepX = 0; stepX < stepsX - 1; ++stepX) {
            oldPoint.setX(wFactor * stepX);
            oldPoint.setY(hFactor * (1. - data[drawT * stepsX + stepX]));
            newPoint.setX(wFactor * (stepX + 1));
            newPoint.setY(hFactor * (1. - data[drawT * stepsX + stepX + 1]));
            painter.drawLine(oldPoint, newPoint);
        }
    }
    painter.end();
    pixmap.save("results.png", "PNG");
}

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
