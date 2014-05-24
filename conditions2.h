double startFunction2(double x) {
    return x * (1 - x) * (1 - x);
}

double rightPartFunction2(double x, double currentValue) {
    return -(x * x * currentValue + 1);
}
