#include "database.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QDateTime>

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
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "reset_token VARCHAR(100), "
        "reset_token_expires TIMESTAMP"
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

QString Database::generateToken(int length)
{
    const QString chars = "0123456789";
    QString token;

    for (int i = 0; i < length; ++i) {
        token += chars.at(QRandomGenerator::global()->bounded(chars.length()));
    }
    return token;
}

bool Database::setResetToken(const QString& email, const QString& token, const QDateTime& expires) {
    QSqlQuery query;
    query.prepare("UPDATE users SET reset_token = :token, reset_token_expires = :expires WHERE email = :email");
    query.bindValue(":token", token);
    query.bindValue(":expires", expires);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Ошибка установки токена восстановления:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool Database::validateResetToken(const QString& token) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE reset_token = :token AND reset_token_expires > NOW()");
    query.bindValue(":token", token);

    if (!query.exec() || !query.next()) {
        qDebug() << "Ошибка проверки токена:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool Database::updatePasswordByToken(const QString& token, const QString& newPasswordHash) {
    QSqlQuery query;
    query.prepare("UPDATE users SET password_hash = :password_hash, reset_token = NULL, reset_token_expires = NULL WHERE reset_token = :token");
    query.bindValue(":password_hash", newPasswordHash);
    query.bindValue(":token", token);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления пароля:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
QString Database::hashPassword(const QString& password)
{
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
        );
    return QString(hash.toHex());
}
