#include "postgresqlserver.h"
#include "database.h"
#include "math_engine.h"
#include "auth.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QRegularExpression>

PostgreSQLServer::PostgreSQLServer(QObject *parent) : QObject(parent) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &PostgreSQLServer::onNewConnection);

    if (!m_server->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Ошибка запуска:" << m_server->errorString();
    } else {
        qDebug() << "Сервер запущен на порту 33333";

        QString dbHost = qgetenv("POSTGRES_HOST");
        if (dbHost.isEmpty()) dbHost = "localhost";
        QString dbName = qgetenv("POSTGRES_DB");
        if (dbName.isEmpty()) dbName = "function_plotter";
        QString dbUser = qgetenv("POSTGRES_USER");
        if (dbUser.isEmpty()) dbUser = "postgres";
        QString dbPass = qgetenv("POSTGRES_PASSWORD");
        if (dbPass.isEmpty()) dbPass = "02468abc";

        Database* db = Database::getInstance();
        if (!db->connect(dbHost, dbName, dbUser, dbPass, 5432)) {
            qDebug() << "Ошибка подключения к PostgreSQL!";
        } else {
            qDebug() << "PostgreSQL подключен";
        }
    }
}

PostgreSQLServer::~PostgreSQLServer() {
    m_server->close();
    Database::getInstance()->close();
}

void PostgreSQLServer::onNewConnection() {
    QTcpSocket* client = m_server->nextPendingConnection();
    if (!client) return;

    ClientSession session;
    session.buffer = "";
    session.currentLogin = "";
    m_clients[client] = session;

    connect(client, &QTcpSocket::readyRead, this, &PostgreSQLServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &PostgreSQLServer::onClientDisconnected);

    qDebug() << "Клиент подключен:" << client->peerAddress().toString();
    sendResponse(client, "Connected to Function Plotter Server");
}

void PostgreSQLServer::onReadyRead() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client || !m_clients.contains(client)) return;

    QByteArray data = client->readAll();
    m_clients[client].buffer += QString::fromUtf8(data);

    if (m_clients[client].buffer.contains('\n')) {
        QStringList lines = m_clients[client].buffer.split('\n');
        for (int i = 0; i < lines.size() - 1; ++i) {
            QString request = lines[i].trimmed();
            if (!request.isEmpty()) processRequest(client, request);
        }
        m_clients[client].buffer = lines.last();
    }
}

void PostgreSQLServer::processRequest(QTcpSocket* client, const QString& request) {
    qDebug() << "Запрос:" << request;
    QStringList parts = request.split('|');
    if (parts.isEmpty()) {
    sendResponse(client, "ERROR|Empty");
        return;
    }

    QString cmd = parts[0];

    if (cmd == "reg" && parts.size() >= 4) {
        Database* db = Database::getInstance();
        if (db->addUser(parts[1], hashPassword(parts[2]), parts[3])) {
            sendResponse(client, "REG_SUCCESS|" + parts[1]);
            qDebug() << "✅ Регистрация успешна:" << parts[1];
        } else {
            sendResponse(client, "REG_FAILED|User exists");
            qDebug() << "❌ Регистрация не удалась:" << parts[1];
        }
    } else if (cmd == "auth" && parts.size() >= 3) {
        Database* db = Database::getInstance();
        if (db->checkAuth(parts[1], hashPassword(parts[2]))) {
            m_clients[client].currentLogin = parts[1];
            sendResponse(client, "AUTH_SUCCESS|" + parts[1]);
            qDebug() << "✅ Авторизация успешна:" << parts[1];
        } else {
            sendResponse(client, "AUTH_FAILED|Invalid credentials");
            qDebug() << "❌ Неверные учётные данные:" << parts[1];
        }
    } else if (cmd == "reset" && parts.size() >= 4) {
        Database* db = Database::getInstance();
        if (db->resetPassword(parts[1], parts[2], hashPassword(parts[3]))) {
            sendResponse(client, "RESET_SUCCESS|" + parts[1]);
            qDebug() << "✅ Пароль восстановлен:" << parts[1];
        } else {
            sendResponse(client, "RESET_FAILED|Error");
            qDebug() << "❌ Ошибка восстановления пароля:" << parts[1];
        }
    } else if (cmd == "plot_system" && parts.size() >= 7) {
        bool ok1, ok2, ok3, ok4, ok5, ok6;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        double c = parts[3].toDouble(&ok3);
        double xStart = parts[4].toDouble(&ok4);
        double xEnd = parts[5].toDouble(&ok5);
        double step = parts[6].toDouble(&ok6);

        if (ok1 && ok2 && ok3 && ok4 && ok5 && ok6) {
            SystemFunctionParams params(a, b, c, xStart, xEnd, step);
            sendJsonSystemPoints(client, params);
        } else {
            sendResponse(client, "PLOT_SYSTEM_FAILED|Invalid parameters");
        }
    } else if (cmd == "plot" && parts.size() >= 4) {
        bool ok1, ok2, ok3;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        double c = parts[3].toDouble(&ok3);

        if (ok1 && ok2 && ok3) {
            FunctionParams params(a, b, c);
            sendJsonPoints(client, params);
        } else {
            sendResponse(client, "PLOT_FAILED|Invalid parameters");
        }
    } else {
        sendResponse(client, "UNKNOWN_COMMAND");
    }
}

void PostgreSQLServer::sendResponse(QTcpSocket* client, const QString& response) {
    if (client && client->isOpen()) {
        client->write((response + "\n").toUtf8());
        client->flush();
    }
}

void PostgreSQLServer::sendJsonPoints(QTcpSocket* client, const FunctionParams& params)
{
    double xStart = -10.0;
    double xEnd = 10.0;
    double step = 0.1;

    QVector<Point> points = MathEngine::generateSystemPoints(
        params.a, params.b, params.c,
        xStart, xEnd, step
        );

    QJsonArray jsonPoints;

    for (const Point& point : points) {

        double y = point.y;

        // ❗ ФИЛЬТР БИТЫХ ЗНАЧЕНИЙ
        if (std::isnan(y) || std::isinf(y))
            continue;

        // ❗ ОГРАНИЧЕНИЕ "УЛЁТА"
        if (std::abs(y) > 1e6)
            continue;

        QJsonObject jsonPoint;
        jsonPoint["x"] = point.x;
        jsonPoint["y"] = y;

        jsonPoints.append(jsonPoint);
    }

    QJsonObject response;
    response["type"] = "PLOT_DATA";
    response["points"] = jsonPoints;

    QJsonDocument doc(response);
    sendResponse(client, QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}

void PostgreSQLServer::sendJsonSystemPoints(QTcpSocket* client, const SystemFunctionParams& params)
{
    QVector<Point> points = MathEngine::generateSystemPoints(
        params.a, params.b, params.c,
        params.xStart, params.xEnd, params.step
        );

    QJsonArray jsonPoints;

    for (const Point& point : points) {

        double y = point.y;

        if (std::isnan(y) || std::isinf(y))
            continue;

        if (std::abs(y) > 1e6)
            continue;

        QJsonObject jsonPoint;
        jsonPoint["x"] = point.x;
        jsonPoint["y"] = y;

        jsonPoints.append(jsonPoint);
    }

    QJsonObject response;
    response["type"] = "PLOT_DATA";
    response["points"] = jsonPoints;

    QJsonDocument doc(response);
    sendResponse(client, QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));

    qDebug() << "Graph sent. Points:" << jsonPoints.size();
}


void PostgreSQLServer::onClientDisconnected() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client && m_clients.contains(client)) {
        qDebug() << "Клиент отключен:" << client->peerAddress().toString();
        m_clients.remove(client);
        client->deleteLater();
    }
}
