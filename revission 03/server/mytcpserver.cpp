#include "mytcpserver.h"
#include "database.h"
#include <QDebug>
#include <cmath>
#include <QJsonParseError>
#include <limits>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRandomGenerator>
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::onNewConnection);
    if (!setupDatabase()) {
        qCritical() << "Failed to initialize database";
        return;
    }

    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    quint16 port = 33333;
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

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead()
{
    if (!mTcpSocket) return;

    QByteArray data = mTcpSocket->readAll();
    handleClientRequest(mTcpSocket, data);
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
        // f(x) = ∛x + a для x < 0
        return std::cbrt(x) + a;
    } else if (x > 0 && x < 1) {
        // f(x) = 1/x - b для 0 < x < 1
        if (std::abs(x) < 1e-10) {
            return std::numeric_limits<double>::max();
        }
        return (1.0 / x) - b;
    } else {
        // f(x) = x² - cx + 1 для x ≥ 1
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

        // Обработка особых случаев (бесконечности и NaN)
        if (std::isinf(fx) || std::isnan(fx)) {
            continue; // Пропускаем точки, где функция не определена
        }

        double gx = fx * 0.8;  // g(x) = 0.8 * f(x)
        double hx = fx * 1.2;  // h(x) = 1.2 * f(x)

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

bool MyTcpServer::setupDatabase() {
    Database& db = Database::getInstance();
    if (!db.initialize()) {
        qCritical() << "Failed to initialize database";
        return false;
    }

    qDebug() << "Database initialized successfully";
    return true;
}
bool MyTcpServer::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password_hash FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        return false; // Пользователь не найден
    }

    QString storedHash = query.value(0).toString();
    // Здесь должна быть проверка хеша пароля
    // В реальном приложении используйте bcrypt, scrypt или PBKDF2
    return (storedHash == password); // Упрощённая проверка для примера
}
bool MyTcpServer::registerUser(const QString &username,
                               const QString &password,
                               const QString &email)
{
    Database& db = Database::getInstance();
    QSqlDatabase database = db.getDatabase();

    if (!database.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    QString hashedPassword = Database::hashPassword(password);

    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password_hash, email) VALUES (:username, :password_hash, :email)");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", hashedPassword);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qWarning() << "Registration failed:" << query.lastError().text();
        return false;
    }

    qDebug() << "User registered:" << username;
    return true;
}

void MyTcpServer::handleClientRequest(QTcpSocket *client, const QByteArray &request)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(request, &error);

    if (error.error != QJsonParseError::NoError) {
        sendErrorResponse(client, "Invalid JSON format");
        return;
    }

    QJsonObject requestObj = doc.object();
    QString requestType = requestObj["type"].toString();

    if (requestType == "auth") {
        handleAuthRequest(client, requestObj);
    } else if (requestType == "register") {
        handleRegisterRequest(client, requestObj);
    } else {
        // Обычная обработка запросов (расчёт графиков и т. д.)
        QByteArray response = processRequest(request);
        client->write(response);
        client->flush();
    }
}

void MyTcpServer::handleAuthRequest(QTcpSocket *client, const QJsonObject &request)
{
    QString username = request["username"].toString();
    QString password = request["password"].toString();

    qDebug() << "Auth request for user:" << username;

    if (username.isEmpty() || password.isEmpty()) {
        sendAuthError(client, "Логин и пароль не могут быть пустыми");
        return;
    }

    // Проверяем существование пользователя и корректность пароля
    if (authenticateUser(username, password)) {
        QJsonObject response;
        response["type"] = "auth_response";
        response["status"] = "success";
        response["message"] = "Авторизация успешна";
        response["username"] = username;

        QJsonDocument doc(response);
        client->write(doc.toJson() + "\n");
        qDebug() << "User" << username << "authenticated successfully";
    } else {
        sendAuthError(client, "Неверный логин или пароль");
        qDebug() << "Authentication failed for user:" << username;
    }
}

void MyTcpServer::sendAuthError(QTcpSocket *client, const QString &errorMsg)
{
    QJsonObject response;
    response["type"] = "auth_response";
    response["status"] = "error";
    response["error"] = errorMsg;

    QJsonDocument doc(response);
    client->write(doc.toJson() + "\n");
}
void MyTcpServer::sendErrorResponse(QTcpSocket *client, const QString &errorMsg)
{
    QJsonObject response;
    response["status"] = "error";
    response["error"] = errorMsg;

    QJsonDocument doc(response);
    client->write(doc.toJson() + "\n");
    client->flush();
}


bool MyTcpServer::recoverPassword(const QString &username, const QString &email)
{
    Database& db = Database::getInstance();
    QSqlDatabase database = db.getDatabase();

    if (!database.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    // Проверка существования пользователя
    QSqlQuery checkQuery(database);
    checkQuery.prepare("SELECT id FROM users WHERE username = :username AND email = :email");
    checkQuery.bindValue(":username", username);
    checkQuery.bindValue(":email", email);

    if (!checkQuery.exec()) {
        qWarning() << "Check query failed:" << checkQuery.lastError().text();
        return false;
    }

    if (!checkQuery.next()) {
        qDebug() << "User not found for recovery: " << username << email;
        return false;
    }

    int userId = checkQuery.value("id").toInt();

    // Генерация временного пароля
    QString tempPassword = generateTemporaryPassword();
    QString hashedTempPassword = Database::hashPassword(tempPassword);

    // Обновление пароля в БД
    QSqlQuery updateQuery(database);
    updateQuery.prepare("UPDATE users SET password_hash = :password_hash WHERE id = :id");
    updateQuery.bindValue(":password_hash", hashedTempPassword);
    updateQuery.bindValue(":id", userId);

    if (!updateQuery.exec()) {
        qWarning() << "Password update failed:" << updateQuery.lastError().text();
        return false;
    }

    // Отправка email
    bool emailSent = sendRecoveryEmail(email, username, tempPassword);
    if (!emailSent) {
        qWarning() << "Failed to send recovery email to:" << email;
        return false;
    }

    qDebug() << "Password recovery successful for user:" << username;
    return true;
}




// Вспомогательная функция для отправки email
bool MyTcpServer::sendRecoveryEmail(const QString &email,
                                    const QString &username,
                                    const QString &tempPassword) const
{
    Q_UNUSED(email)
    Q_UNUSED(username)
    Q_UNUSED(tempPassword)

    // Здесь должна быть реализация отправки email через SMTP
    // Например, с использованием QTcpSocket и протокола SMTP

    qDebug() << "Recovery email would be sent to:" << email
             << "Username:" << username
             << "Temporary password:" << tempPassword;

    // В реальной реализации:
    // 1. Установить соединение с SMTP‑сервером
    // 2. Авторизоваться
    // 3. Отправить письмо с временным паролем
    // 4. Обработать возможные ошибки

    // Для тестирования возвращаем true
    return true;
}


void MyTcpServer::handleRecoveryRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString username = request["username"].toString();
    QString email = request["email"].toString();
    // QString newPassword = request["new_password"].toString(); // Убираем эту строку — не нужна


    if (recoverPassword(username, email)) {  // Только два аргумента
        QJsonObject response;
        response["success"] = true;
        QJsonDocument doc(response);
        socket->write(doc.toJson());
    } else {
        // Обработка ошибки
    }
}

void MyTcpServer::handleRegistration(const QJsonObject &request, QTcpSocket *socket)
{
    QString username = request["username"].toString();
    QString password = request["password"].toString();
    QString email = request["email"].toString();

    bool success = registerUser(username, password, email);

    QJsonObject response;
    response["success"] = success;
    if (!success) {
        response["error"] = "Регистрация не удалась";
    }

    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->flush();
}

void MyTcpServer::handleRecovery(const QJsonObject &request, QTcpSocket *socket)
{
    QString username = request["username"].toString();
    QString email = request["email"].toString();

    bool success = recoverPassword(username, email);

    QJsonObject response;
    response["success"] = success;
    if (!success) {
        response["error"] = "Ошибка восстановления пароля";
    }

    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->flush();
}
bool MyTcpServer::startServer()
{
    return tcpServer->listen(QHostAddress::Any, 8080);
}
QString MyTcpServer::generateTemporaryPassword() const
{
    const QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;
    QRandomGenerator random;

    for (int i = 0; i < 8; ++i) {
        password.append(characters.at(random.bounded(characters.length())));
    }

    return password;
}
void MyTcpServer::handleRegisterRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString username = request["username"].toString();
    QString password = request["password"].toString();
    QString email = request["email"].toString();

    bool success = registerUser(username, password, email);

    QJsonObject response;
    response["success"] = success;
    if (!success) {
        response["error"] = "Регистрация не удалась";
    }

    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->flush();
}
void MyTcpServer::onNewConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        connect(clientSocket, &QTcpSocket::readyRead,
                this, [this, clientSocket]() { onClientRequest(clientSocket); });
        connect(clientSocket, &QTcpSocket::disconnected,
                clientSocket, &QTcpSocket::deleteLater);

        qDebug() << "New client connected:" << clientSocket->peerAddress().toString();
    }
}

void MyTcpServer::onClientRequest(QTcpSocket *socket)
{
    if (socket->bytesAvailable() <= 0) {
        return;
    }

    QByteArray data = socket->readAll();
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Invalid JSON received:" << error.errorString();
        return;
    }

    QJsonObject request = jsonDoc.object();
    QString requestType = request["type"].toString();

    if (requestType == "register") {
        handleRegisterRequest(socket, request);
    } else if (requestType == "recovery") {
        handleRecoveryRequest(socket, request);
    } else {
        QJsonObject response;
        response["success"] = false;
        response["error"] = "Unknown request type";
        QJsonDocument doc(response);
        socket->write(doc.toJson());
        socket->flush();
    }
}
