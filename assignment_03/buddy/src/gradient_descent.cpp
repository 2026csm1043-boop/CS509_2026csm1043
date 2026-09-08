#include "gradient_descent.h"

#include <cmath>
#include <limits>

double evaluatePolynomial(
    const std::vector<double>& coefficients,
    double x)
{
    // Horner's method:
    // c0 + c1*x + ... + cd*x^d
    double value = 0.0;

    for (auto it = coefficients.rbegin();
         it != coefficients.rend();
         ++it)
    {
        value = value * x + *it;
    }

    return value;
}

double evaluateDerivative(
    const std::vector<double>& coefficients,
    double x)
{
    // Derivative coefficients:
    // c1 + 2*c2*x + ... + d*cd*x^(d-1)
    if (coefficients.size() <= 1)
    {
        return 0.0;
    }

    double derivative = 0.0;

    for (int i = static_cast<int>(coefficients.size()) - 1;
         i >= 1;
         --i)
    {
        derivative =
            derivative * x +
            static_cast<double>(i) * coefficients[i];
    }

    return derivative;
}

GradientDescentResult gradientDescent(
    const std::vector<double>& coefficients,
    double initialX,
    double learningRate,
    double tolerance,
    int maxIterations)
{
    GradientDescentResult result{};
    double x = initialX;
    int iterations = 0;
    bool converged = false;

    for (int i = 0; i < maxIterations; ++i)
    {
        const double derivative =
            evaluateDerivative(coefficients, x);

        if (!std::isfinite(derivative) || !std::isfinite(x))
        {
            break;
        }

        if (std::abs(derivative) <= tolerance)
        {
            converged = true;
            break;
        }

        x -= learningRate * derivative;
        ++iterations;
    }

    // If the last update lands within tolerance, count that
    // completed update and report convergence.
    if (!converged && std::isfinite(x))
    {
        const double derivative =
            evaluateDerivative(coefficients, x);

        if (std::isfinite(derivative) &&
            std::abs(derivative) <= tolerance)
        {
            converged = true;
        }
    }

    result.finalX = x;
    result.finalValue = evaluatePolynomial(coefficients, x);
    result.iterations = iterations;
    result.converged = converged;

    return result;
}
