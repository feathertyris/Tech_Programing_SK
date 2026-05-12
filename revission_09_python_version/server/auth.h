#ifndef AUTH_H
#define AUTH_H

#include <QString>
#include <QCryptographicHash>
#include <QRandomGenerator>

// Хэширование пароля с помощью SHA-256
inline QString hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),           // преобразуем QString в байтовый массив UTF-8
        QCryptographicHash::Sha256   // выбираем алгоритм SHA-256
    );
    return QString(hash.toHex());    // преобразуем байты в шестнадцатеричную строку
}

// Генерация 6-значного токена для восстановления пароля
inline QString generateResetToken() {
    return QString::number(QRandomGenerator::global()->generate() % 1000000, 10)
        .rightJustified(6, '0');     // дополняем нулями слева до 6 цифр
}

#endif // AUTH_H
