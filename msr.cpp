#include <cmath>
#include <cstring>
#include <stdexcept>
#include "struct.h"

static const double accuracy = 1e-10;
static const unsigned max_iter = 1000;

MsrMatrix::MsrMatrix(unsigned size,
                     unsigned nzcount):
  size(size),
  indices(new unsigned[size+nzcount+1]),
  elements(new double[size+nzcount+1]),
  rightCol(new double[size]) {
    clear();
}

MsrMatrix::~MsrMatrix() {
    delete[] indices;
    delete[] elements;
    delete[] rightCol;
}

void MsrMatrix::clear() {
    currentRow = 0;
    indices[0] = size + 1;
    filledElement = size;
}

void MsrMatrix::appendElement(unsigned row,
                              unsigned col,
                              double   value) {
    if (row == col) {
        elements[row] = value;
        return;
    }

    ++filledElement;
    while (currentRow < row) {
        ++currentRow;
        indices[currentRow] = filledElement;
    }
    indices[filledElement] = col;
    elements[filledElement] = value;
}

void MsrMatrix::finishFilling() {
    ++filledElement;
    while (currentRow < size) {
        ++currentRow;
        indices[currentRow] = filledElement;
    }
}

double MsrMatrix::scalarProduct(double *vector1, double *vector2) const {
    double result = 0;
    for (unsigned i = 0; i < size; ++i)
        result += vector1[i] * vector2[i];
    return result;
}

void MsrMatrix::applyToVector(double *vector, double *newVector) const {
    unsigned i, ind;
    for (i = 0; i < size; ++i) {
        newVector[i] = elements[i] * vector[i];
        for (ind = indices[i]; ind < indices[i+1]; ++ind) {
            newVector[i] += elements[ind] * vector[indices[ind]];
        }
    }
}

double MsrMatrix::getResidual(double *vector) const {
    double result = 0, c;
    unsigned i, ind;
    for (i = 0; i < size; ++i) {
        c = elements[i] * vector[i];
        for (ind = indices[i]; ind < indices[i+1]; ++ind) {
            c += elements[ind] * vector[indices[ind]];
        }
        result += (rightCol[i] - c) * (rightCol[i] - c);
    }
    return result;
}

unsigned MsrMatrix::solve(double *result) const {
    double alpha   = 1;
    double rho     = 1;
    double omega   = 1;
    double *buffer = new double[size * 6];
    double *r      = buffer;
    double *rcap   = buffer + size;
    double *v      = buffer + size * 2;
    double *p      = buffer + size * 3;
    double *s      = buffer + size * 4;
    double *t      = buffer + size * 5;;

    unsigned i, iter;
    double beta, rhoold;

    const double bnorm2 = scalarProduct(rightCol, rightCol);
    const double accuracy2 = accuracy * accuracy;

    applyToVector(result, r);
    memset (v, 0, size * sizeof(double));
    memset (p, 0, size * sizeof(double));
    for (i = 0; i < size; ++i) {
        r[i] = rightCol[i] - r[i];
        rcap[i] = r[i];
    }

    for (iter = 1; iter < max_iter; ++iter) {
        rhoold = rho;
        rho = scalarProduct(rcap, r);
        beta = (rho * alpha) / (rhoold * omega);
        for (i = 0; i < size; ++i) {
            p[i] = r[i] + beta * (p[i] - omega * v[i]);
        }
        applyToVector(p, v);
        alpha = rho / scalarProduct(rcap, v);
        for (i = 0; i < size; ++i) {
            s[i] = r[i] - alpha * v[i];
        }
        applyToVector(s, t);
        omega = scalarProduct(t, s) / scalarProduct(t, t);
        for (i = 0; i < size; ++i) {
            result[i] += (alpha * p[i] + omega * s[i]);
        }
        if (scalarProduct(r, r) < accuracy2 * bnorm2) {
            delete[] buffer;
            return iter;
        }
        for (i = 0; i < size; ++i) {
            r[i] = s[i] - omega * t[i];
        }
    }
    delete[] buffer;
    throw std::runtime_error("Maximum number of iterations reached.");
}
