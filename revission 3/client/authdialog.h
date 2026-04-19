#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

    QString getUsername() const;
    QString getPassword() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onRecoveryClicked();
    void onRegistrationSuccess(const QString &username);
private:
    Ui::AuthDialog *ui;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    void sendRegistrationRequest(const QString &username,
                                 const QString &password,
                                 const QString &email);

signals:
    void authRequestSent(const QByteArray &authData);
    void registerRequestSent(const QByteArray &registerData);
    void recoveryRequestSent(const QByteArray &data);
    void registrationRequestSent(const QByteArray &data);
    void registrationSuccessful(const QString &username);

};

#endif // AUTHDIALOG_H
