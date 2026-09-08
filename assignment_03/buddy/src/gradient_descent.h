#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include <vector>

struct GradientDescentResult
{
    double finalX;
    double finalValue;
    int iterations;
    bool converged;
};

double evaluatePolynomial(
    const std::vector<double>& coefficients,
    double x
);

double evaluateDerivative(
    const std::vector<double>& coefficients,
    double x
);

GradientDescentResult gradientDescent(
    const std::vector<double>& coefficients,
    double initialX,
    double learningRate,
    double tolerance,
    int maxIterations
);

#endif
