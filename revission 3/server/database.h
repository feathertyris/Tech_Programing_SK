#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>  // Добавляем для QSqlError
#include <QCryptographicHash>

class Database {
public:
    static Database& getInstance();
    QSqlDatabase getDatabase() const;
    bool initialize();
    bool executeSqlFile(const QString& filename);

    // Новый метод для хеширования пароля
    static QString hashPassword(const QString &password);

private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase m_db;
};

#endif // DATABASE_H
