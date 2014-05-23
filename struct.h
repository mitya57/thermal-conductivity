typedef double (*InitializeFunction) (double);
typedef double (*RightPartFunction) (double, double);

struct AbstractCallback {
    unsigned stepsX;
    unsigned stepsT;

    AbstractCallback(unsigned _stepsX, unsigned _stepsT):
      stepsX(_stepsX), stepsT(_stepsT)
    {}
    virtual ~AbstractCallback() {}
    virtual void process(unsigned stepT, double *values) = 0;
};

enum SchemeType {
    ExplicitScheme,
    ImplicitScheme
};

enum BoundaryCondition {
    CircularCondition,
    OneZeroCondition
}

struct Parameters {
    SchemeType type;
    double a;
    double tau;
    double h;
    RightPartFunction rightPartFunction;
    BoundaryCondition condition;
};

struct DiagonalMatrix {
    unsigned size;
    double  *botDiag;
    double  *midDiag;
    double  *topDiag;

    DiagonalMatrix(unsigned _size): size(_size) {
        botDiag = new double[size - 1];
        midDiag = new double[size];
        topDiag = new double[size - 1];
    }
    ~DiagonalMatrix() {
        delete[] botDiag;
        delete[] midDiag;
        delete[] topDiag;
    }

    void solve(double *rightCol);
};

void process(unsigned           stepsX,
             unsigned           stepsT,
             InitializeFunction initFunction,
             Parameters        &parameters,
             AbstractCallback  &callback);
