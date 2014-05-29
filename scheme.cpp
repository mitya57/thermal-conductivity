#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "struct.h"

void processIteration(MsrMatrix        &matrix,
                      double const     *oldValues,
                      double           *newValues,
                      Parameters const &parameters) {
    unsigned m;
    double eta = parameters.a * parameters.tau /
                 (2 * parameters.h * parameters.h);
    unsigned size = matrix.size;
    switch (parameters.type) {
    case ImplicitScheme:
        memcpy(matrix.rightCol, oldValues, size * sizeof(double));
        matrix.clear();
        matrix.appendElement(0, 0, -1.);
        if (parameters.boundaryCondition == CircularCondition) {
            matrix.appendElement(0, size - 1, 1.);
            matrix.rightCol[0] = 0.;
        } else {
            matrix.appendElement(0, 1, 1.);
            matrix.rightCol[0] = parameters.h;
        }
        for (m = 1; m < size - 1; ++m) {
            matrix.appendElement(m, m - 1, -eta);
            matrix.appendElement(m, m, 1 + 2 * eta);
            matrix.appendElement(m, m + 1, -eta);
            if (parameters.rightPartFunction) {
                matrix.rightCol[m] += parameters.tau *
                    parameters.rightPartFunction(parameters.h * m, oldValues[m]);
            }
        }
        if (parameters.boundaryCondition == CircularCondition) {
            matrix.appendElement(size - 1, 0, 1.);
            matrix.appendElement(size - 1, 1, -1.);
        }
        matrix.appendElement(size - 1, size - 2, -1.);
        matrix.appendElement(size - 1, size - 1, 1.);
        matrix.rightCol[size - 1] = 0.;
        matrix.finishFilling();
        matrix.solve(newValues);
     // printf("Matrix solved in %u iterations.\n", matrix.solve(newValues));
        break;
    case ExplicitScheme:
        for (m = 1; m < size - 1; ++m) {
            newValues[m] = oldValues[m] + eta * (
                           oldValues[m - 1] - 2 * oldValues[m] + oldValues[m + 1]);
            if (parameters.rightPartFunction) {
                newValues[m] += parameters.tau *
                    parameters.rightPartFunction(parameters.h * m, oldValues[m]);
            }
        }
        if (parameters.boundaryCondition == CircularCondition) {
            newValues[0] = (oldValues[0] + oldValues[1]) * .5;
            newValues[size - 1] = (oldValues[size - 1] + oldValues[size - 2]) * .5;
        } else {
            newValues[0] = oldValues[1] - parameters.h;
            newValues[size - 1] = oldValues[size - 2];
        }
        break;
    }
}

void process(unsigned           stepsX,
             unsigned           stepsT,
             InitializeFunction initFunction,
             Parameters        &parameters,
             AbstractCallback  &callback) {
    double *oldValues = new double[stepsX];
    double *newValues = new double[stepsX];
    MsrMatrix matrix(stepsX, stepsX * 2);
    parameters.h = 1. / stepsX;
    parameters.tau = 1. / stepsT;
    for (unsigned m = 0; m < stepsX; ++m) {
        oldValues[m] = initFunction(parameters.h * m);
    }
    for (unsigned n = 0; n < stepsT; ++n) {
        callback.process(n, oldValues);
        processIteration(matrix, oldValues, newValues, parameters);
        memcpy(oldValues, newValues, stepsX * sizeof(double));
    }
    callback.process(stepsT, newValues);
    delete[] oldValues;
    delete[] newValues;
}

struct DefaultCallback: public AbstractCallback {
    FILE *output;

    DefaultCallback(unsigned _stepsX, unsigned _stepsT):
      AbstractCallback(_stepsX, _stepsT),
      output(fopen("results.txt", "w"))
    {}
    virtual ~DefaultCallback() {
        fclose(output);
    }

    virtual void process(unsigned stepT, double *values) {
        fprintf(output, "%u:", stepT);
        for (unsigned m = 0; m < stepsX; ++m) {
            fprintf(output, " %f", values[m]);
        }
        fprintf(output, "\n");
    }
};

int oldMain() {
    unsigned stepsX = 100;
    unsigned stepsT = 100;
    Parameters parameters;
    DefaultCallback callback(stepsX, stepsT);
    parameters.a = 1;
    parameters.type = ImplicitScheme;
    parameters.rightPartFunction = 0;
    process(stepsX, stepsT, sin, parameters, callback);
    return 0;
}
