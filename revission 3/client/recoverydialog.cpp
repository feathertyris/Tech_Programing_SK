#include "recoverydialog.h"
#include "ui_recoverydialog.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
RecoveryDialog::RecoveryDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RecoveryDialog)
{
    ui->setupUi(this);
    setWindowTitle("Восстановление пароля");

    // Подключаем обработчик нажатия кнопки восстановления
    connect(ui->recoveryButton, &QPushButton::clicked,
            this, &RecoveryDialog::onRecoveryClicked);

    // Очищаем статусную метку при вводе в любом поле
    connect(ui->usernameEdit, &QLineEdit::textChanged,
            this, &RecoveryDialog::clearStatus);
    connect(ui->emailEdit, &QLineEdit::textChanged,
            this, &RecoveryDialog::clearStatus);
}

RecoveryDialog::~RecoveryDialog()
{
    delete ui;
}

void RecoveryDialog::onRecoveryClicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString email = ui->emailEdit->text().trimmed();

    // Валидация входных данных
    if (username.isEmpty()) {
        showError("Введите логин");
        ui->usernameEdit->setFocus();
        return;
    }

    if (email.isEmpty()) {
        showError("Введите email");
        ui->emailEdit->setFocus();
        return;
    }

    if (!isValidEmail(email)) {
        showError("Некорректный формат email");
        ui->emailEdit->setFocus();
        return;
    }

    // Отправляем запрос на восстановление пароля
    sendRecoveryRequest(username, email);
}

void RecoveryDialog::sendRecoveryRequest(const QString &username, const QString &email)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url("http://localhost:8080/recovery");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["email"] = email;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            qDebug() << "Recovery response:" << responseData;
            showSuccess("Восстановление пароля инициировано. Проверьте почту.");
            QTimer::singleShot(2000, this, &QDialog::accept);
        } else {
            showError("Ошибка сети: " + reply->errorString());
        }
        reply->deleteLater();
    });
}


bool RecoveryDialog::isValidEmail(const QString &email) const
{
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

void RecoveryDialog::showError(const QString &message)
{
    ui->statusLabel->setStyleSheet("color: #e74c3c; font-weight: bold;");
    ui->statusLabel->setText(message);
}

void RecoveryDialog::showSuccess(const QString &message)
{
    ui->statusLabel->setStyleSheet("color: #2ecc71; font-weight: bold;");
    ui->statusLabel->setText(message);
}

void RecoveryDialog::clearStatus()
{
    ui->statusLabel->clear();
}
QString RecoveryDialog::getUsername() const
{
    return ui->usernameEdit->text(); // замените usernameEdit на имя вашего QLineEdit
}

QString RecoveryDialog::getEmail() const
{
    return ui->emailEdit->text(); // замените emailEdit на имя вашего QLineEdit
}

QString RecoveryDialog::getNewPassword() const
{
    return ui->newPasswordEdit->text(); // замените newPasswordEdit на имя вашего QLineEdit
}
