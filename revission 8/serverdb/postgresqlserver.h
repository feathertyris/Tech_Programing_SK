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

struct TripleFunctionParams {
    double a1, b1, c1;
    double a2, b2, c2;
    double a3, b3, c3;
    double xStart, xEnd, step;

    TripleFunctionParams(double a1_, double b1_, double c1_,
                         double a2_, double b2_, double c2_,
                         double a3_, double b3_, double c3_,
                         double xStart_, double xEnd_, double step_)
        : a1(a1_), b1(b1_), c1(c1_),
        a2(a2_), b2(b2_), c2(c2_),
        a3(a3_), b3(b3_), c3(c3_),
        xStart(xStart_), xEnd(xEnd_), step(step_) {}
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
    void sendJsonTriplePoints(QTcpSocket* client, const TripleFunctionParams& params);
    void sendRecoveryEmail(const QString& email, const QString& code);
    QString hashPassword(const QString& password);

    QTcpServer* m_server;
    QMap<QTcpSocket*, ClientSession> m_clients;
};

#endif // POSTGRESQLSERVER_H