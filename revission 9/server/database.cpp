#include "database.h"
#include <QDebug>
#include <QSqlError>

Database* Database::instance = nullptr;

Database::Database() {}

Database* Database::getInstance() {
    if (!instance) {
        instance = new Database();
    }
    return instance;
}

bool Database::connect(const QString& host, const QString& dbName,
                    const QString& user, const QString& password, int port) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    db.setPort(port);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }

    // Создаём таблицу пользователей, если её нет
    QSqlQuery query;
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "login VARCHAR(50) UNIQUE NOT NULL, "
        "password_hash VARCHAR(255) NOT NULL, "
        "email VARCHAR(100) NOT NULL, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    bool tableCreated = query.exec(createTableQuery);

    if (!tableCreated) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().text();
    } else {
        qDebug() << "Таблица users готова к работе";
    }

    return true;
}

void Database::close() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Соединение с БД закрыто";
    }
}

bool Database::addUser(const QString& login, const QString& passwordHash, const QString& email) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, password_hash, email) VALUES (:login, :password_hash, :email)");
    query.bindValue(":login", login);
    query.bindValue(":password_hash", passwordHash);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Ошибка регистрации пользователя:" << query.lastError().text();
        return false;
    }

    qDebug() << "Пользователь добавлен в БД:" << login;
    return true;
}

bool Database::checkAuth(const QString& login, const QString& passwordHash) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login AND password_hash = :password_hash");
    query.bindValue(":login", login);
    query.bindValue(":password_hash", passwordHash);

    if (!query.exec() || !query.next()) {
        qDebug() << "Ошибка авторизации:" << query.lastError().text();
        return false;
    }

    int count = query.value(0).toInt();
    bool isAuthenticated = (count > 0);

    if (isAuthenticated) {
        qDebug() << "Авторизация успешна для:" << login;
    } else {
        qDebug() << "Неудачная попытка авторизации для:" << login;
    }

    return isAuthenticated;
}

bool Database::resetPassword(const QString& login, const QString& email, const QString& newPasswordHash) {
    QSqlQuery query;
    query.prepare("UPDATE users SET password_hash = :new_password "
                  "WHERE login = :login AND email = :email");
    query.bindValue(":new_password", newPasswordHash);
    query.bindValue(":login", login);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Ошибка восстановления пароля:" << query.lastError().text();
        return false;
    }

    // Проверяем, что запись была обновлена
    if (query.numRowsAffected() > 0) {
        qDebug() << "Пароль восстановлен для пользователя:" << login;
        return true;
    } else {
        qDebug() << "Восстановление пароля не удалось — пользователь не найден:" << login;
        return false;
    }
}
