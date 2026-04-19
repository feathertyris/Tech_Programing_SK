// mathengine.cpp
#include "math_engine.h"
#include <cmath>
#include <QDebug>

QVector<Point> MathEngine::generateSystemPoints(double a, double b, double c,
                                                double xStart, double xEnd, double step)
{
    QVector<Point> points;

    if (xStart >= xEnd) {
        qDebug() << "Ошибка: xStart >= xEnd (" << xStart << ">=" << xEnd << ")";
        return points;
    }
    if (step <= 0) {
        qDebug() << "Ошибка: шаг <= 0 (" << step << ")";
        return points;
    }

    for (double x = xStart; x <= xEnd; x += step) {
        double y = calculateFunction(x, a, b, c);
        points.append({x, y});
    }

    qDebug() << "Сгенерировано точек:" << points.size();
    return points;
}


double MathEngine::calculateFunction(double x, double a, double b, double c)
{
    if (x < 0) {
        return std::cbrt(x) + a;
    } else if (x > 0 && x < 1) {
        if (std::abs(x) < 1e-10) {  // Защита от деления на 0
            return std::numeric_limits<double>::infinity();
        }
        return 1.0 / x - b;
    } else {  // x ≥ 1
        return x * x - c * x + 1;
    }
}

