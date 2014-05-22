typedef double (*InitializeFunction) (double);

struct AbstractCallback {
    unsigned stepsX;

    AbstractCallback(unsigned _stepsX):
      stepsX(_stepsX)
    {}
    virtual ~AbstractCallback() {}
    virtual void process(unsigned stepT, double *values) = 0;
};

enum SchemeType {
    ExplicitScheme,
    ImplicitScheme
};

struct Parameters {
    SchemeType type;
    double a;
    double tau;
    double h;
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
