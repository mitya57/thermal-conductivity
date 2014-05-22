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
             Parameters        *parameters) {
    double stepX = 1. / stepsX;
    double *oldValues = new double[stepsX];
    double *newValues = new double[stepsX];
    DiagonalMatrix matrix(stepsX);
    parameters->h = stepX;
    parameters->tau = 1. / stepsT;
    unsigned m;
    for (m = 0; m < stepsX; ++m) {
        oldValues[m] = initFunction(stepX * m);
    }
    FILE *output = fopen("results.txt", "w");
    for (unsigned n = 0; n < stepsT; ++n) {
        printf("Time iteration is %u.\n", n + 1);
        fprintf(output, "%u:", n);
        for (m = 0; m < stepsX; ++m) {
            fprintf(output, " %f", oldValues[m]);
        }
        fprintf(output, "\n");
        processIteration(&matrix, oldValues, newValues, parameters);
        memcpy(oldValues, newValues, stepsX * sizeof(double));
    }
    fclose(output);
    delete[] oldValues;
    delete[] newValues;
}

int main() {
    Parameters parameters;
    parameters.a = 1;
    parameters.type = ImplicitScheme;
    process(100, 100, sin, &parameters);
    return 0;
}
