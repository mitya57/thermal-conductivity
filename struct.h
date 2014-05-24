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
};

struct Parameters {
    SchemeType type;
    double a;
    double tau;
    double h;
    RightPartFunction rightPartFunction;
    BoundaryCondition boundaryCondition;
};

class MsrMatrix {
public:
    unsigned size;
    unsigned *indices;
    double *elements;
    double *rightCol;

    MsrMatrix(unsigned size, unsigned nzcount);
    ~MsrMatrix();

    void clear();
    void appendElement(unsigned row, unsigned col, double value);
    void finishFilling();
    unsigned solve(double *result) const;

private:
    unsigned currentRow;
    unsigned filledElement;

    double scalarProduct(double *vector1, double *vector2) const;
    void applyToVector(double *vector, double *newVector) const;
    double getResidual(double *vector) const;
};

void process(unsigned           stepsX,
             unsigned           stepsT,
             InitializeFunction initFunction,
             Parameters        &parameters,
             AbstractCallback  &callback);
