#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    QString getEmail() const;
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    ~RegistrationDialog();

private slots:
    void onRegisterClicked();
    void sendRegistrationRequest(const QString &username,
                                 const QString &password,
                                 const QString &email);

private:
    Ui::RegistrationDialog *ui;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *emailEdit;
    QPushButton *registerButton;
signals:
    void registrationSuccessful(const QString &username);
};

#endif // REGISTRATIONDIALOG_H
