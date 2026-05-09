#ifndef POSTGRESQLSERVER_H
#define POSTGRESQLSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "math_engine.h"

struct ClientSession {
    QString buffer;
    QString currentLogin;
};

struct FunctionParams {
    double a, b, c;

    FunctionParams(double a_, double b_, double c_)
        : a(a_), b(b_), c(c_) {}
};

class PostgreSQLServer : public QObject
{
    Q_OBJECT

public:
    explicit PostgreSQLServer(QObject *parent = nullptr);
    ~PostgreSQLServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    void processRequest(QTcpSocket* client, const QString& request);
    void sendResponse(QTcpSocket* client, const QString& response);
    void sendJsonPoints(QTcpSocket* client, const FunctionParams& params);
    void sendJsonSystemPoints(QTcpSocket* client, const SystemFunctionParams& params);
    void sendRecoveryEmail(const QString& email, const QString& code);

    QTcpServer* m_server;
    QMap<QTcpSocket*, ClientSession> m_clients;
};

#endif // POSTGRESQLSERVER_H
