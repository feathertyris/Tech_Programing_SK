#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Database : public QObject {
    Q_OBJECT

public:
    static Database* getInstance();
    bool connect(const QString& host, const QString& dbName,
                 const QString& user, const QString& password, int port);
    void close();
    bool addUser(const QString& login, const QString& passwordHash, const QString& email);
    bool checkAuth(const QString& login, const QString& passwordHash);
    bool resetPassword(const QString& login, const QString& email, const QString& newPasswordHash);

private:
    explicit Database();
    static Database* instance;
    QSqlDatabase db;
};

#endif // DATABASE_H
