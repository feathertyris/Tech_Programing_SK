// mathengine.cpp
#include "math_engine.h"
#include "qjsonarray.h"
#include <cmath>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
QVector<Point> MathEngine::generateSystemPoints(double a, double b, double c,
                                                double xStart, double xEnd, double step,
                                                QString& jsonResponse)
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

    const int MAX_POINTS = 1000;
    int estimatedPoints = static_cast<int>((xEnd - xStart) / step) + 1;
    bool isLimited = false;
    double originalStep = step;

    if (estimatedPoints > MAX_POINTS) {
        isLimited = true;
        step = (xEnd - xStart) / MAX_POINTS;
        qDebug() << "Слишком много точек для генерации:" << estimatedPoints
                 << "Ограничено до" << MAX_POINTS
                 << "Шаг изменён с" << originalStep << "на" << step;
    }

    for (double x = xStart; x <= xEnd; x += step) {
        double y = calculateFunction(x, a, b, c);
        points.append({x, y});
    }

    qDebug() << "Сгенерировано точек:" << points.size();

    QJsonObject response;
    response["type"] = "PLOT_DATA";
    response["is_limited"] = isLimited;
    if (isLimited) {
        response["original_step"] = originalStep;
        response["adjusted_step"] = step;
        response["max_points"] = MAX_POINTS;
    }

    QJsonArray jsonPoints;
    for (const Point& p : points) {
        QJsonObject pointObj;
        pointObj["x"] = p.x;
        pointObj["y"] = p.y;
        jsonPoints.append(pointObj);
    }
    response["points"] = jsonPoints;

    jsonResponse = QJsonDocument(response).toJson(QJsonDocument::Compact);
    return points;
}

double MathEngine::calculateFunction(double x, double a, double b, double c)
{
    if (x < 0) {
        // Кубический корень для отрицательных x
        return std::cbrt(x) + a;
    } else if (x > 0 && x < 1) {
        // Защита от деления на ноль
        if (std::abs(x) < 1e-10) {
            return std::numeric_limits<double>::infinity();
        }
        double result = 1.0 / x - b;
        // Ограничение экстремальных значений
        if (std::isinf(result) || std::isnan(result)) {
            return 0.0;
        }
        return result;
    } else { // x >= 1
        return x * x - c * x + 1;
    }
}


