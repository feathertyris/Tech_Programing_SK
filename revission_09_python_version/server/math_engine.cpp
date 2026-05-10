#include "math_engine.h"
#include <cmath>
#include <QDebug>
#include <limits>

QVector<Point> MathEngine::generateSystemPoints(double a, double b, double c,
                                                double xStart, double xEnd, double step)
{
    QVector<Point> points;

    if (xStart >= xEnd) {
        qDebug() << "Ошибка: xStart >= xEnd";
        return points;
    }

    if (step <= 0) {
        qDebug() << "Ошибка: step <= 0";
        return points;
    }

    for (double x = xStart; x <= xEnd; x += step)
    {
        // ❗ защита от разрывов (0 и почти 0)
        if (std::abs(x) < 1e-10) {
            continue;
        }

        double y = calculateFunction(x, a, b, c);

        // ❗ фильтр некорректных значений
        if (std::isnan(y) || std::isinf(y)) {
            continue;
        }

        // ❗ защита от "взрывов" (ломает график)
        if (std::abs(y) > 1e6) {
            continue;
        }

        points.append({x, y});
    }

    qDebug() << "Сгенерировано точек:" << points.size();
    return points;
}

double MathEngine::calculateFunction(double x, double a, double b, double c)
{
    if (x < 0) {
        return std::cbrt(x) + a;
    }

    if (x > 0 && x < 1) {
        // ❗ защита от деления на 0
        if (std::abs(x) < 1e-10) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return 1.0 / x - b;
    }

    // x ≥ 1
    return x * x - c * x + 1;
}