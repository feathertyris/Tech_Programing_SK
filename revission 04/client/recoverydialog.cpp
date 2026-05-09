#include "recoverydialog.h"
#include "ui_recoverydialog.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>

RecoveryDialog::RecoveryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecoveryDialog),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Применяем стили программно
    this->setStyleSheet(
        "QDialog { background: #f8f9fa; }"
        "QLabel { color: #212121; }"
        "QLineEdit { border: 2px solid #2196F3; border-radius: 8px; padding: 5px; background: white; }"
        "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );

    connect(ui->sendRecoveryButton, &QPushButton::clicked,
            this, &RecoveryDialog::on_sendRecoveryButton_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &RecoveryDialog::onReadyRead);
    socket->connectToHost("localhost", 33333);
}

RecoveryDialog::~RecoveryDialog()
{
    delete ui;
}

void RecoveryDialog::on_sendRecoveryButton_clicked()
{
    QString email = ui->emailEdit->text();

    if (email.isEmpty()) {
        ui->statusLabel->setText("Введите email для восстановления");
        return;
    }

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
            displaySuccessMessage();
        } else if (cmd == "EMAIL_NOT_FOUND") {
            ui->statusLabel->setText("Email не найден в системе");
        }
    }
}

void RecoveryDialog::displaySuccessMessage()
{
    ui->statusLabel->setText("Код восстановления отправлен на email");
    QMessageBox::information(this, "Восстановление",
                             "Код восстановления был отправлен на указанный email. Проверьте почту.");
}
