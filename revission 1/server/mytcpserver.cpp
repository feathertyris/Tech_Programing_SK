#include "mytcpserver.h"
#include <QDebug>
#include <cmath>
#include <QJsonParseError>
#include <limits>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);
    quint16 port = 33333;  // Порт сервера
    if (!mTcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start on port" << port;
    } else {
        qDebug() << "Server is started on port" << port;
    }
}

MyTcpServer::~MyTcpServer()
{
    if (mTcpServer) {
        mTcpServer->close();
    }
    if (mTcpSocket) {
        mTcpSocket->close();
    }
}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
    qDebug() << "Client connected";

    // Убираем отправку приветствия — клиент не ожидает дополнительных сообщений
    // mTcpSocket->write("Server ready. Send JSON with parameters.\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead()
{
    if (!mTcpSocket) return;

    QByteArray data = mTcpSocket->readAll();

    // Обрабатываем запрос и отправляем ответ
    QByteArray response = processRequest(data);
    mTcpSocket->write(response);
    mTcpSocket->flush(); // Гарантируем отправку данных
}

void MyTcpServer::slotClientDisconnected()
{
    qDebug() << "Client disconnected";
    if (mTcpSocket) {
        mTcpSocket->close();
        mTcpSocket = nullptr;
    }
}

double MyTcpServer::calculateFunction(double x, double a, double b, double c)
{
    if (x < 0) {
        return std::cbrt(x) + a;
    } else if (x > 0 && x < 1) {
        if (std::abs(x) < 1e-10) {
            return std::numeric_limits<double>::max();
        }
        return (1.0 / x) - b;
    } else {
        return x * x - c * x + 1;
    }
}

QByteArray MyTcpServer::processRequest(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        QJsonObject errorObj;
        errorObj["error"] = "Invalid JSON format";
        errorObj["details"] = error.errorString();
        errorObj["status"] = "error";
        return QJsonDocument(errorObj).toJson() + "\n";
    }

    QJsonObject request = doc.object();

    // Проверка обязательных параметров
    if (!request.contains("a") || !request.contains("b") || !request.contains("c") ||
        !request.contains("x_start") || !request.contains("x_end") || !request.contains("x_step")) {
        QJsonObject errorObj;
        errorObj["error"] = "Missing required parameters";
        errorObj["status"] = "error";
        return QJsonDocument(errorObj).toJson() + "\n";
    }

    double a = request["a"].toDouble(0.0);
    double b = request["b"].toDouble(0.0);
    double c = request["c"].toDouble(0.0);

    double xStart = request["x_start"].toDouble(-10.0);
    double xEnd = request["x_end"].toDouble(10.0);
    double xStep = request["x_step"].toDouble(0.1);

    if (xStart >= xEnd || xStep <= 0) {
        QJsonObject errorObj;
        errorObj["error"] = "Invalid parameters: x_start >= x_end or x_step <= 0";
        errorObj["status"] = "error";
        return QJsonDocument(errorObj).toJson() + "\n";
    }

    QJsonArray points_f, points_g, points_h;

    for (double x = xStart; x <= xEnd; x += xStep) {
        double fx = calculateFunction(x, a, b, c);
        double gx = fx * 0.8;
        double hx = fx * 1.2;

        QJsonObject point_f, point_g, point_h;
        point_f["x"] = x; point_f["f_x"] = fx;
        point_g["x"] = x; point_g["g_x"] = gx;
        point_h["x"] = x; point_h["h_x"] = hx;

        points_f.append(point_f);
        points_g.append(point_g);
        points_h.append(point_h);
    }

    QJsonObject response;
    response["points_f"] = points_f;
    response["points_g"] = points_g;
    response["points_h"] = points_h;
    response["status"] = "success";

    return QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n";
}

