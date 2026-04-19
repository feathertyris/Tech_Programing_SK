#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_recoveryButton_clicked();
    void onConnected();
    void onReadyRead();
    void displaySuccessMessage();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    Ui::AuthDialog *ui;
    QTcpSocket *socket;
};

#endif // AUTHDIALOG_H
