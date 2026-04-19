#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    bool startServer();
    ~MyTcpServer();

public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();
    void onNewConnection();
    void onClientRequest(QTcpSocket *socket);
    void handleRegistration(const QJsonObject &request, QTcpSocket *socket);
    void handleRecovery(const QJsonObject &request, QTcpSocket *socket);

private:
    bool setupDatabase();
    bool authenticateUser(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password, const QString &email);
    bool recoverPassword(const QString &username, const QString &email);
    void handleClientRequest(QTcpSocket *client, const QByteArray &request);
    void handleAuthRequest(QTcpSocket *client, const QJsonObject &request);
    void handleRegisterRequest(QTcpSocket *client, const QJsonObject &request);
    void handleRecoveryRequest(QTcpSocket *client, const QJsonObject &request);
    void sendAuthError(QTcpSocket *client, const QString &errorMsg);
    QTcpServer *mTcpServer = nullptr;
    QTcpSocket *mTcpSocket = nullptr;
    void sendErrorResponse(QTcpSocket *client, const QString &errorMsg);

    double calculateFunction(double x, double a, double b, double c);
    QByteArray processRequest(const QByteArray &data);
    QString generateTemporaryPassword() const;
    bool sendRecoveryEmail(const QString &email,
                           const QString &username,
                           const QString &tempPassword) const;
    QTcpServer *tcpServer;
};

#endif // MYTCPSERVER_H
