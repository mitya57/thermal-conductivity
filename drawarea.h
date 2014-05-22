#include <QPaintEvent>
#include <QWidget>

class DrawArea: public QWidget {
Q_OBJECT

public:
    double  *data;
    unsigned stepsX;
    unsigned stepsT;
    unsigned stepT;

    virtual void paintEvent(QPaintEvent *event);
};
