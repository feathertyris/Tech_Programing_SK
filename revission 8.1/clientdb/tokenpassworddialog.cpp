#include "tokenpassworddialog.h"
#include "ui_tokenpassworddialog.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>
#include <QCloseEvent>

TokenPasswordDialog::TokenPasswordDialog(const QString &email, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TokenPasswordDialog),
    socket(new QTcpSocket(this)),
    recoveredEmail(email)
{
    ui->setupUi(this);

    connect(socket, &QTcpSocket::readyRead, this, &TokenPasswordDialog::onReadyRead);
    socket->connectToHost("localhost", 33333);

    ui->emailLabel->setText("Email: " + email);
}

TokenPasswordDialog::~TokenPasswordDialog()
{
    delete ui;
}

void TokenPasswordDialog::on_resetPasswordButton_clicked()
{
    QString token = ui->codeEdit->text().trimmed();
    QString newPassword = ui->newPasswordEdit->text();

    if (token.length() != 6) {
        ui->statusLabel->setText("Код должен содержать ровно 6 цифр");
        return;
    }

    if (newPassword.length() < 6) {
        ui->statusLabel->setText("Пароль должен быть минимум 6 символов");
        return;
    }

    QString request = QString("reset_password|%1|%2").arg(token, newPassword);
    socket->write(request.toUtf8() + "\n");
}

void TokenPasswordDialog::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    QStringList parts = response.split('|');

    if (!parts.isEmpty()) {
        QString cmd = parts[0];

        if (cmd == "RESET_SUCCESS") {
            displaySuccessMessage();
            accept();  // закрывает диалог с кодом QDialog::Accepted
        } else if (cmd == "RESET_FAILED") {
            if (parts.size() > 1) {
                QString errorMsg = parts[1];
                ui->statusLabel->setText("Ошибка восстановления: " + errorMsg);
            } else {
                ui->statusLabel->setText("Ошибка восстановления пароля. Попробуйте снова.");
            }
        } else if (cmd == "TOKEN_INVALID") {
            ui->statusLabel->setText("Неверный или просроченный код восстановления");
        } else if (cmd == "TOKEN_EXPIRED") {
            ui->statusLabel->setText("Срок действия кода восстановления истёк");
        }
    }
}

void TokenPasswordDialog::displaySuccessMessage()
{
    ui->statusLabel->setText("Пароль успешно восстановлен!");
    QMessageBox::information(this, "Успех",
                             "Пароль успешно изменён. Теперь вы можете войти в систему.");
    // accept() вызывается в onReadyRead(), поэтому здесь не обязателен
}

void TokenPasswordDialog::closeEvent(QCloseEvent *event)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
    event->accept();
}
