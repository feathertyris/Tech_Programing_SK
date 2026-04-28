#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Применяем стили программно
    this->setStyleSheet(
        "QDialog { background: #f8f9fa; }"
        "QLabel { color: #212121; }"
        "QLineEdit { border: 2px solid #2196F3; border-radius: 8px; padding: 5px; background: white; }"
        "QPushButton { background-color: #FF9800; color: white; border: none; padding: 10px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e68900; }"
        );

    connect(socket, &QTcpSocket::readyRead, this, &RegistrationDialog::onReadyRead);
    socket->connectToHost("localhost", 33333);
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

void RegistrationDialog::on_registerButton_clicked()
{
    QString login = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString email = ui->emailEdit->text();

    if (login.length() < 3) {
        ui->statusLabel->setText("Логин должен быть минимум 3 символа");
        return;
    }
    if (password.length() < 6) {
        ui->statusLabel->setText("Пароль должен быть минимум 6 символов");
        return;
    }
    if (email.isEmpty()) {
        ui->statusLabel->setText("Введите email");
        return;
    }

    QString request = QString("reg|%1|%2|%3").arg(login, password, email);
    socket->write(request.toUtf8() + "\n");
}

void RegistrationDialog::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    QStringList parts = response.split('|');

    if (!parts.isEmpty()) {
        QString cmd = parts[0];

        if (cmd == "REG_SUCCESS") {
            displaySuccessMessage();
            emit registrationSuccess();
            accept();
        } else if (cmd == "REG_ERROR") {
            if (parts.size() > 1) {
                QString errorMsg = parts[1];
                ui->statusLabel->setText("Ошибка регистрации: " + errorMsg);
            } else {
                ui->statusLabel->setText("Ошибка регистрации. Попробуйте снова.");
            }
        } else if (cmd == "USER_EXISTS") {
            ui->statusLabel->setText("Пользователь с таким логином уже существует");
        } else if (cmd == "EMAIL_TAKEN") {
            ui->statusLabel->setText("Email уже используется другим пользователем");
        }
    }
}

void RegistrationDialog::displaySuccessMessage()
{
    ui->statusLabel->setText("Регистрация успешна!");
    QMessageBox::information(this, "Успех",
                             "Регистрация завершена");
}
