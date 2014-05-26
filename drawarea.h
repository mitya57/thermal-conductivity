#include <QPaintEvent>
#include <QWidget>

void saveToPng(unsigned stepsX,
               unsigned stepsT,
               double  *data);

class DrawArea: public QWidget {
Q_OBJECT

public:
    double  *data;
    unsigned stepsX;
    unsigned stepsT;
    unsigned stepT;

    virtual void paintEvent(QPaintEvent *event);
};
