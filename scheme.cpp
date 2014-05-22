#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "struct.h"

void DiagonalMatrix::solve(double *rightCol) {
    unsigned s;
    for (s = 0; s < size - 1; ++s) {
        topDiag[s] /= midDiag[s];
        rightCol[s] /= midDiag[s];
        midDiag[s + 1] -= botDiag[s] * topDiag[s];
        rightCol[s + 1] -= botDiag[s] * rightCol[s];
    }
    rightCol[size - 1] /= midDiag[size - 1];
    for (s = size - 1; s >= 1; --s) {
        rightCol[s - 1] -= topDiag[s - 1] * rightCol[s];
    }
}

void processIteration(DiagonalMatrix   *matrix,
                      double const     *oldValues,
                      double           *newValues,
                      Parameters const *parameters) {
    unsigned m;
    double eta = parameters->a * parameters->tau /
                 (2 * parameters->h * parameters->h);
    unsigned size = matrix->size;
    switch (parameters->type) {
    case ImplicitScheme:
        memcpy(newValues, oldValues, size * sizeof(double));
        matrix->midDiag[0] = 1;
        matrix->topDiag[0] = 0;
        for (m = 1; m < size - 1; ++m) {
            matrix->botDiag[m - 1] = -eta;
            matrix->midDiag[m] = 1 + 2 * eta;
            matrix->topDiag[m] = -eta;
        }
        matrix->midDiag[size - 1] = 1;
        matrix->botDiag[size - 2] = 0;
        matrix->solve(newValues);
        break;
    case ExplicitScheme:
        newValues[0] = oldValues[0];
        for (m = 1; m < size - 1; ++m) {
            newValues[m] = oldValues[m] + eta * (
                           oldValues[m - 1] - 2 * oldValues[0] + oldValues[m + 1]);
        }
        newValues[size - 1] = oldValues[size - 1];
        break;
    }
}

void process(unsigned           stepsX,
             unsigned           stepsT,
             InitializeFunction initFunction,
             Parameters        *parameters,
             AbstractCallback  &callback) {
    double stepX = 1. / stepsX;
    double *oldValues = new double[stepsX];
    double *newValues = new double[stepsX];
    DiagonalMatrix matrix(stepsX);
    parameters->h = stepX;
    parameters->tau = 1. / stepsT;
    for (unsigned m = 0; m < stepsX; ++m) {
        oldValues[m] = initFunction(stepX * m);
    }
    for (unsigned n = 0; n < stepsT; ++n) {
        callback.process(n, oldValues);
        processIteration(&matrix, oldValues, newValues, parameters);
        memcpy(oldValues, newValues, stepsX * sizeof(double));
    }
    delete[] oldValues;
    delete[] newValues;
}

struct DefaultCallback: public AbstractCallback {
    FILE *output;

    DefaultCallback(unsigned _stepsX):
      AbstractCallback(_stepsX),
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

int main() {
    unsigned stepsX = 100;
    unsigned stepsT = 100;
    Parameters parameters;
    DefaultCallback callback(stepsX);
    parameters.a = 1;
    parameters.type = ImplicitScheme;
    process(stepsX, stepsT, sin, &parameters, callback);
    return 0;
}
