#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>
RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    setWindowTitle("Регистрация");

    connect(ui->registerButton, &QPushButton::clicked, this, &RegistrationDialog::onRegisterClicked);
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

QString RegistrationDialog::getUsername() const
{
    return ui->usernameEdit->text();
}

QString RegistrationDialog::getPassword() const
{
    return ui->passwordEdit->text();
}

QString RegistrationDialog::getEmail() const
{
    return ui->emailEdit->text();
}

void RegistrationDialog::onRegisterClicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString email = ui->emailEdit->text().trimmed();

    // Валидация данных — выполняется только при нажатии кнопки
    if (username.isEmpty()) {
        ui->statusLabel->setText("Введите имя пользователя!");
        return;
    }
    if (password.isEmpty()) {
        ui->statusLabel->setText("Введите пароль!");
        return;
    }
    if (email.isEmpty()) {
        ui->statusLabel->setText("Введите email!");
        return;
    }

    ui->statusLabel->setText("Отправка данных...");
    ui->registerButton->setEnabled(false);

    sendRegistrationRequest(username, password, email);
}


void RegistrationDialog::sendRegistrationRequest(const QString &username,
                                                 const QString &password,
                                                 const QString &email)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://localhost:8080/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["email"] = email;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, [this, reply, timeoutTimer]() {
        if (reply && !reply->isFinished()) {
            reply->abort();
            ui->statusLabel->setText("Ошибка: таймаут соединения");
            ui->registerButton->setEnabled(true);
        }
        timeoutTimer->deleteLater();
    });
    timeoutTimer->start(10000);

    connect(reply, &QNetworkReply::finished, this, [this, reply, timeoutTimer, username]() {
        timeoutTimer->stop();
        timeoutTimer->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "Registration response:" << responseData;

            // Эмит сигнала об успешной регистрации
            //emit registrationSuccessful(username);

            ui->statusLabel->setText("Регистрация успешна! Открытие главного окна...");
            QTimer::singleShot(1500, this, &QDialog::accept);
        } else {
            ui->statusLabel->setText("Ошибка регистрации: " + reply->errorString());
            ui->registerButton->setEnabled(true);
        }

        reply->deleteLater();
    });
}

