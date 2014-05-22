#include <QApplication>
#include <cmath>
#include <cstring>
#include "drawarea.h"
#include "struct.h"

struct StoreCallback: public AbstractCallback {
    double *data;

    StoreCallback(unsigned _stepsX, unsigned _stepsT):
      AbstractCallback(_stepsX, _stepsT),
      data(new double[stepsX * stepsT])
    {}
    virtual ~StoreCallback() {
        delete[] data;
    }

    virtual void process(unsigned stepT, double *values) {
        memcpy(data + stepT * stepsX, values, stepsX * sizeof(double));
    }
};

double startFunction(double x) {
    return sin(x * 2 * M_PI);
}

int main(int argc, char **argv) {
    unsigned stepsX = 100;
    unsigned stepsT = 100;

    QApplication app(argc, argv);
    DrawArea drawArea;
    Parameters parameters;
    StoreCallback callback(stepsX, stepsT);
    parameters.a = 1;
    parameters.type = ImplicitScheme;
    process(stepsX, stepsT, startFunction, parameters, callback);

    drawArea.data = callback.data;
    drawArea.stepsT = stepsT;
    drawArea.stepsX = stepsX;
    drawArea.stepT = 1;
    drawArea.show();
    drawArea.repaint();
    return app.exec();
}
