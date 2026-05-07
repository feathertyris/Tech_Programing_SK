#include "postgresqlserver.h"
#include "database.h"
#include "math_engine.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QDateTime>
#include <QSqlQuery>

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
        if (dbName.isEmpty()) dbName = "timp_database";
        QString dbUser = qgetenv("POSTGRES_USER");
        if (dbUser.isEmpty()) dbUser = "timp_user";
        QString dbPass = qgetenv("POSTGRES_PASSWORD");
        if (dbPass.isEmpty()) dbPass = "timp123";

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
    sendResponse(client, "Connected to Function Timp Server");
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
    } else if (cmd == "forgot" && parts.size() >= 2) {
        QString email = parts[1];
        Database* db = Database::getInstance();

        QString token = db->generateToken(6);
        QDateTime expires = QDateTime::currentDateTime().addSecs(30 * 60);

        if (db->setResetToken(email, token, expires)) {
            sendRecoveryEmail(email, token);
            sendResponse(client, "FORGOT_SENT|" + email);
            qDebug() << "Код восстановления отправлен на:" << email;
        } else {
            sendResponse(client, "EMAIL_NOT_FOUND|" + email);
            qDebug() << "Email не найден:" << email;
        }
    } else if (cmd == "reset_password" && parts.size() >= 3) {
        QString token = parts[1];
        QString newPassword = parts[2];

        Database* db = Database::getInstance();
        if (db->validateResetToken(token)) {
            if (db->updatePasswordByToken(token, hashPassword(newPassword))) {
                sendResponse(client, "RESET_SUCCESS");
                qDebug() << "Пароль успешно восстановлен по токену:" << token;
            } else {
                sendResponse(client, "RESET_FAILED|Ошибка обновления пароля");
            }
        } else {
            sendResponse(client, "RESET_FAILED|Неверный или просроченный код");
        }
    } else if (cmd == "plot_triple" && parts.size() >= 13) {
        bool ok;
        double a1 = parts[1].toDouble(&ok);
        double b1 = parts[2].toDouble(&ok);
        double c1 = parts[3].toDouble(&ok);
        double a2 = parts[4].toDouble(&ok);
        double b2 = parts[5].toDouble(&ok);
        double c2 = parts[6].toDouble(&ok);
        double a3 = parts[7].toDouble(&ok);
        double b3 = parts[8].toDouble(&ok);
        double c3 = parts[9].toDouble(&ok);
        double xStart = parts[10].toDouble(&ok);
        double xEnd = parts[11].toDouble(&ok);
        double step = parts[12].toDouble(&ok);

        if (ok) {
            TripleFunctionParams params(a1, b1, c1, a2, b2, c2, a3, b3, c3, xStart, xEnd, step);
            sendJsonTriplePoints(client, params);
        } else {
            sendResponse(client, "PLOT_TRIPLE_FAILED|Invalid parameters");
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

void PostgreSQLServer::sendJsonTriplePoints(QTcpSocket* socket, const TripleFunctionParams& params) {
    QJsonObject response;
    response["type"] = "TRIPLE_PLOT_DATA";

    QJsonArray functionsArray;

    // Первая функция (красный цвет)
    QVector<Point> points1 = MathEngine::generateSystemPoints(
        params.a1, params.b1, params.c1,
        params.xStart, params.xEnd, params.step
        );

    QJsonObject func1;
    func1["color"] = "red";
    func1["name"] = QString("f1(x) = a=%1, b=%2, c=%3").arg(params.a1).arg(params.b1).arg(params.c1);

    QJsonArray pointsArray1;
    for (int i = 0; i < points1.size(); ++i) {
        const Point& p = points1.at(i);
        if (std::isfinite(p.y) && std::abs(p.y) < 1e6) {
            QJsonObject pointObj;
            pointObj["x"] = p.x;
            pointObj["y"] = p.y;
            pointsArray1.append(pointObj);
        }
    }
    func1["points"] = pointsArray1;
    functionsArray.append(func1);

    // Вторая функция (синий цвет)
    QVector<Point> points2 = MathEngine::generateSystemPoints(
        params.a2, params.b2, params.c2,
        params.xStart, params.xEnd, params.step
        );

    QJsonObject func2;
    func2["color"] = "blue";
    func2["name"] = QString("f2(x) = a=%1, b=%2, c=%3").arg(params.a2).arg(params.b2).arg(params.c2);

    QJsonArray pointsArray2;
    for (int i = 0; i < points2.size(); ++i) {
        const Point& p = points2.at(i);
        if (std::isfinite(p.y) && std::abs(p.y) < 1e6) {
            QJsonObject pointObj;
            pointObj["x"] = p.x;
            pointObj["y"] = p.y;
            pointsArray2.append(pointObj);
        }
    }
    func2["points"] = pointsArray2;
    functionsArray.append(func2);

    // Третья функция (зелёный цвет)
    QVector<Point> points3 = MathEngine::generateSystemPoints(
        params.a3, params.b3, params.c3,
        params.xStart, params.xEnd, params.step
        );

    QJsonObject func3;
    func3["color"] = "green";
    func3["name"] = QString("f3(x) = a=%1, b=%2, c=%3").arg(params.a3).arg(params.b3).arg(params.c3);

    QJsonArray pointsArray3;
    for (int i = 0; i < points3.size(); ++i) {
        const Point& p = points3.at(i);
        if (std::isfinite(p.y) && std::abs(p.y) < 1e6) {
            QJsonObject pointObj;
            pointObj["x"] = p.x;
            pointObj["y"] = p.y;
            pointsArray3.append(pointObj);
        }
    }
    func3["points"] = pointsArray3;
    functionsArray.append(func3);

    response["functions"] = functionsArray;

    QByteArray jsonData = QJsonDocument(response).toJson(QJsonDocument::Compact);
    socket->write(jsonData + "\n");
}

void PostgreSQLServer::sendResponse(QTcpSocket* client, const QString& response) {
    if (client && client->isOpen()) {
        client->write((response + "\n").toUtf8());
        client->flush();
    }
}

void PostgreSQLServer::sendJsonPoints(QTcpSocket* socket, const FunctionParams& params)
{
    double xStart = -10.0;
    double xEnd = 10.0;
    double step = 0.1;

    QVector<Point> points = MathEngine::generateSystemPoints(
        params.a, params.b, params.c,
        xStart, xEnd, step
        );

    QJsonObject response;
    response["type"] = "PLOT_DATA";

    QJsonArray jsonPoints;
    for (const Point& p : points) {
        if (std::isfinite(p.y) && std::abs(p.y) < 1e6) {
            QJsonObject pointObj;
            pointObj["x"] = p.x;
            pointObj["y"] = p.y;
            jsonPoints.append(pointObj);
        }
    }
    response["points"] = jsonPoints;
    response["is_limited"] = false;

    QByteArray jsonData = QJsonDocument(response).toJson(QJsonDocument::Compact);
    socket->write(jsonData + "\n");
}

void PostgreSQLServer::sendJsonSystemPoints(QTcpSocket* socket, const SystemFunctionParams& params)
{
    QVector<Point> points = MathEngine::generateSystemPoints(
        params.a, params.b, params.c,
        params.xStart, params.xEnd, params.step
        );

    QJsonObject response;
    response["type"] = "PLOT_DATA";

    const int MAX_POINTS = 2000;
    int estimatedPoints = static_cast<int>((params.xEnd - params.xStart) / params.step) + 1;
    bool isLimited = estimatedPoints > MAX_POINTS;

    QJsonArray jsonPoints;
    for (const Point& p : points) {
        if (std::isfinite(p.y) && std::abs(p.y) < 1e6) {
            QJsonObject pointObj;
            pointObj["x"] = p.x;
            pointObj["y"] = p.y;
            jsonPoints.append(pointObj);
        }
    }

    response["points"] = jsonPoints;
    response["is_limited"] = isLimited;
    if (isLimited) {
        response["original_step"] = params.step;
        response["adjusted_step"] = (params.xEnd - params.xStart) / MAX_POINTS;
        response["max_points"] = MAX_POINTS;
    }

    QByteArray jsonData = QJsonDocument(response).toJson(QJsonDocument::Compact);
    socket->write(jsonData + "\n");
}

void PostgreSQLServer::onClientDisconnected() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client && m_clients.contains(client)) {
        qDebug() << "Клиент отключен:" << client->peerAddress().toString();
        m_clients.remove(client);
        client->deleteLater();
    }
}

QString PostgreSQLServer::hashPassword(const QString& password) {
    Database* db = Database::getInstance();
    return db->hashPassword(password);
}

void PostgreSQLServer::sendRecoveryEmail(const QString& to, const QString& code) {
    qDebug() << "==========================================";
    qDebug() << "📧 Email to:" << to;
    qDebug() << "   Subject:" << "Восстановление пароля";
    qDebug() << "   Body:" << QString("Ваш код восстановления: %1. Код действителен 30 минут.").arg(code);
    qDebug() << "==========================================";
}