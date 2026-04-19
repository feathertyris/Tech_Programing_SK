#include "authdialog.h"
#include "ui_authdialog.h"
#include "registrationdialog.h"
#include "recoverydialog.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>
#include "clientgui.h"
AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Применяем стили программно
    this->setStyleSheet(
        "QDialog { background: #f8f9fa; }"
        "QLabel { color: #212121; font-weight: bold; }"
        "QLineEdit { border: 2px solid #4CAF50; border-radius: 8px; padding: 5px; background: white; }"
        "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );

    connect(ui->loginButton, &QPushButton::clicked, this, &AuthDialog::on_loginButton_clicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &AuthDialog::on_registerButton_clicked);
    connect(ui->recoveryButton, &QPushButton::clicked, this, &AuthDialog::on_recoveryButton_clicked);

    connect(socket, &QTcpSocket::connected, this, &AuthDialog::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &AuthDialog::onReadyRead);
    connect(socket, &QAbstractSocket::errorOccurred, this, &AuthDialog::onSocketError);

    socket->connectToHost("localhost", 33333);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::onConnected()
{
    qDebug() << "Клиент подключен к серверу";
}

void AuthDialog::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    qDebug() << "Ошибка подключения:" << socket->errorString();
    QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу");
}

void AuthDialog::on_loginButton_clicked()
{
    QString login = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    QString request = QString("auth|%1|%2").arg(login, password);
    socket->write(request.toUtf8() + "\n");
}

void AuthDialog::on_registerButton_clicked()
{
    RegistrationDialog regDialog(this);
    regDialog.exec();
}

void AuthDialog::on_recoveryButton_clicked()
{
    RecoveryDialog recoveryDialog(this);
    recoveryDialog.exec();
}

void AuthDialog::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    QStringList parts = response.split('|');

    if (!parts.isEmpty()) {
        QString cmd = parts[0];
        if (cmd == "AUTH_SUCCESS") {
            displaySuccessMessage();
        } else if (cmd == "REG_SUCCESS") {
            displaySuccessMessage();
        } else if (cmd == "FORGOT_SENT") {
            QMessageBox::information(this, "Восстановление",
                                     "Если email существует, код был отправлен");
        }
    }
}

void AuthDialog::displaySuccessMessage()
{
    QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");

    // Создаём и показываем главное окно с графиком
    ClientGUI *clientgui = new ClientGUI();
    clientgui->show();

    accept(); // Закрываем диалог авторизации
}
