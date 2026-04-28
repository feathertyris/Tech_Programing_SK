#include "recoverydialog.h"
#include "ui_recoverydialog.h"
#include <QMessageBox>
#include <QTcpSocket>
#include "tokenpassworddialog.h"  // подключаем заголовочный файл TokenPasswordDialog


RecoveryDialog::RecoveryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecoveryDialog),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(socket, &QTcpSocket::readyRead, this, &RecoveryDialog::onReadyRead);
    socket->connectToHost("localhost", 33333);
}
RecoveryDialog::~RecoveryDialog()
{
    delete ui;
}
void RecoveryDialog::on_sendRecoveryButton_clicked()
{
    QString email = ui->emailEdit->text().trimmed();

    if (email.isEmpty()) {
        ui->statusLabel->setText("Введите email");
        return;
    }

    if (!email.contains('@') || !email.contains('.')) {
        ui->statusLabel->setText("Введите корректный email");
        return;
    }

    ui->sendRecoveryButton->setEnabled(false);
    ui->statusLabel->setText("Отправка запроса...");

    QString request = QString("forgot|%1").arg(email);
    socket->write(request.toUtf8() + "\n");
}

void RecoveryDialog::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    QStringList parts = response.split('|');

    if (!parts.isEmpty()) {
        QString cmd = parts[0];

        if (cmd == "FORGOT_SENT") {
            if (parts.size() > 1) {
                QString receivedEmail = parts[1];
                ui->statusLabel->setText(
                    "Код восстановления отправлен на: " + receivedEmail +
                    "\nПроверьте вашу почту.");

                // ОТКРЫВАЕМ TokenPasswordDialog после успешной отправки запроса
                TokenPasswordDialog *tokenDialog = new TokenPasswordDialog(receivedEmail, this);
                tokenDialog->exec();  // модальное окно

                ui->sendRecoveryButton->setEnabled(true);
            }
        } else if (cmd == "EMAIL_NOT_FOUND") {
            ui->statusLabel->setText("Email не найден в системе");
            ui->sendRecoveryButton->setEnabled(true);
        } else if (cmd == "RATE_LIMITED") {
            ui->statusLabel->setText("Слишком много запросов. Попробуйте позже.");
            ui->sendRecoveryButton->setEnabled(true);
        } else {
            ui->statusLabel->setText("Ошибка отправки. Попробуйте снова.");
            ui->sendRecoveryButton->setEnabled(true);
        }
    }
}
