#include "math_engine.h"
#include <cmath>
#include <QDebug>
#include <limits>

QVector<Point> MathEngine::generateSystemPoints(double a, double b, double c,
                                                double xStart, double xEnd, double step)
{
    QVector<Point> points;
    points.reserve(2000);

    if (xStart >= xEnd) {
        qDebug() << "Ошибка: xStart >= xEnd (" << xStart << ">=" << xEnd << ")";
        return points;
    }
    if (step <= 0) {
        qDebug() << "Ошибка: шаг <= 0 (" << step << ")";
        return points;
    }

    const int MAX_POINTS = 2000;
    int estimatedPoints = static_cast<int>((xEnd - xStart) / step) + 1;
    double actualStep = step;

    if (estimatedPoints > MAX_POINTS) {
        actualStep = (xEnd - xStart) / MAX_POINTS;
        qDebug() << "Слишком много точек:" << estimatedPoints
                 << "Уменьшено до" << MAX_POINTS
                 << "Шаг:" << actualStep;
    }

    int numSteps = static_cast<int>((xEnd - xStart) / actualStep) + 1;
    for (int i = 0; i <= numSteps; ++i) {
        double x = xStart + i * actualStep;
        if (x > xEnd) x = xEnd;

        double y = calculateFunction(x, a, b, c);

        if (std::isfinite(y) && std::abs(y) < 1e6) {
            points.append({x, y});
        } else {
            points.append({x, std::numeric_limits<double>::quiet_NaN()});
        }
    }

    qDebug() << "Сгенерировано точек:" << points.size();
    return points;
}

double MathEngine::calculateFunction(double x, double a, double b, double c)
{
    if (x < 0) {
        return std::cbrt(x) + a;
    } else if (x > 0 && x < 1) {
        if (std::abs(x) < 1e-10) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        double result = 1.0 / x - b;
        if (!std::isfinite(result) || std::abs(result) > 1e6) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return result;
    } else if (x >= 1) {
        double result = x * x - c * x + 1;
        if (!std::isfinite(result) || std::abs(result) > 1e6) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return result;
    }
    return std::numeric_limits<double>::quiet_NaN();
}