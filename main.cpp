#include <QApplication>
#include <cstring>
#include <stdexcept>
#include "window.h"
#include "struct.h"

#include "conditions1.h"
#include "conditions2.h"

struct StoreCallback: public AbstractCallback {
    double *data;

    StoreCallback(unsigned _stepsX, unsigned _stepsT):
      AbstractCallback(_stepsX, _stepsT),
      data(new double[stepsX * (stepsT + 1)])
    {}
    virtual ~StoreCallback() {
        delete[] data;
    }

    virtual void process(unsigned stepT, double *values) {
        memcpy(data + stepT * stepsX, values, stepsX * sizeof(double));
    }
};

int main(int argc, char **argv) {
    unsigned stepsX = 100;
    unsigned stepsT = 100;
    Parameters parameters;
    InitializeFunction startFunction;

    if (argc < 2) {
        throw std::runtime_error("Please specify program number.");
    }

    if (argv[1][0] == '1') {
        startFunction = startFunction1;
        parameters.rightPartFunction = 0;
    } else if (argv[1][0] == '2') {
        startFunction = startFunction2;
        parameters.rightPartFunction = rightPartFunction2;
    } else {
        throw std::out_of_range("Valid program numbers are 1 and 2.");
    }

    QApplication app(argc, argv);
    MainWindow window(stepsT);
    StoreCallback callback(stepsX, stepsT);
    parameters.a = 1;
    parameters.type = ImplicitScheme;
    process(stepsX, stepsT, startFunction, parameters, callback);

    DrawArea &drawArea = window.drawArea;
    drawArea.data = callback.data;
    drawArea.stepsT = stepsT;
    drawArea.stepsX = stepsX;
    drawArea.stepT = 1;
    window.show();
    return app.exec();
}
