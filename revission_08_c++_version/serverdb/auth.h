#ifndef AUTH_H
#define AUTH_H

#include <QString>
#include <QCryptographicHash>
#include <QRandomGenerator>
inline QString hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

inline QString generateResetToken() {
    return QString::number(QRandomGenerator::global()->generate() % 1000000, 10)
    .rightJustified(6, '0');
}

#endif // AUTH_H
