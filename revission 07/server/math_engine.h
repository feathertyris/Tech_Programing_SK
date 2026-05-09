#ifndef MATHENGINE_H
#define MATHENGINE_H

#include <QVector>
#include <limits>

struct Point {
    double x;
    double y;
};

struct SystemFunctionParams {
    double a, b, c;
    double xStart, xEnd, step;

    SystemFunctionParams(double a_, double b_, double c_,
                         double xStart_, double xEnd_, double step_)
        : a(a_), b(b_), c(c_),
        xStart(xStart_), xEnd(xEnd_), step(step_) {}
};

class MathEngine
{
public:
    static QVector<Point> generateSystemPoints(double a, double b, double c,
                                               double xStart, double xEnd, double step,
                                               QString& jsonResponse);

private:
    static double calculateFunction(double x, double a, double b, double c);
};

#endif // MATHENGINE_H
