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
    bool setResetToken(const QString& email, const QString& token, const QDateTime& expires);
    bool validateResetToken(const QString& token);
    bool updatePasswordByToken(const QString& token, const QString& newPasswordHash);
    QString generateToken(int length = 6);
    QString hashPassword(const QString& password);

private:
    explicit Database();

    static Database* instance;
    QSqlDatabase db;
};

#endif // DATABASE_H
