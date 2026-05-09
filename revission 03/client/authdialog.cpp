#include "authdialog.h"
#include "recoverydialog.h"
#include "ui_authdialog.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "registrationdialog.h"
#include "clientgui.h"
AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    setWindowTitle("Авторизация");

    connect(ui->loginButton, &QPushButton::clicked, this, &AuthDialog::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &AuthDialog::onRegisterClicked);
    connect(ui->recoveryButton, &QPushButton::clicked, this, &AuthDialog::onRecoveryClicked);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

QString AuthDialog::getUsername() const
{
    return ui->usernameEdit->text();
}

QString AuthDialog::getPassword() const
{
    return ui->passwordEdit->text();
}

void AuthDialog::onLoginClicked()
{
    QString username = getUsername();
    QString password = getPassword();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    QJsonObject authRequest;
    authRequest["type"] = "auth";
    authRequest["username"] = username;
    authRequest["password"] = password;

    QJsonDocument doc(authRequest);
    emit authRequestSent(doc.toJson());
    accept(); // Закрываем диалог после отправки
}

void AuthDialog::onRegisterClicked()
{
    RegistrationDialog regDialog(this);

    connect(&regDialog, &RegistrationDialog::registrationSuccessful,
            this, &AuthDialog::onRegistrationSuccess);

    if (regDialog.exec() == QDialog::Accepted) {
        qDebug() << "Registration completed successfully";
    } else {
        qDebug() << "Registration dialog cancelled";
    }
}


void AuthDialog::onRecoveryClicked()
{
    RecoveryDialog recoveryDialog;
    if (recoveryDialog.exec() == QDialog::Accepted) {
        QString username = recoveryDialog.getUsername();
        QString email = recoveryDialog.getEmail();
        QString newPassword = recoveryDialog.getNewPassword();

        QJsonObject recoveryRequest;
        recoveryRequest["type"] = "recovery";
        recoveryRequest["username"] = username;
        recoveryRequest["email"] = email;
        recoveryRequest["new_password"] = newPassword;

        QJsonDocument doc(recoveryRequest);
        emit recoveryRequestSent(doc.toJson());
    }
    if (recoveryDialog.exec() == QDialog::Accepted) {
        QJsonObject request;
        request["type"] = "recovery";
        request["username"] = recoveryDialog.getUsername();
        request["email"] = recoveryDialog.getEmail();
        QJsonDocument doc(request);
        emit recoveryRequestSent(doc.toJson());
    }
}
void AuthDialog::onRegistrationSuccess(const QString &username)
{
    QMessageBox::information(this, "Успех",
                             QString("Регистрация успешна! Добро пожаловать, %1!").arg(username));

    this->close();

    ClientGUI *mainWindow = new ClientGUI();
    mainWindow->show();
}

