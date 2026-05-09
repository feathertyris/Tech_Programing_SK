#include "database.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() {
    m_db = QSqlDatabase::addDatabase("QPSQL", "SingletonConnection");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("chart_app_db");
    m_db.setUserName("postgres");
    m_db.setPassword("1111");  // Установите пароль, если нужен
    m_db.setPort(5432);
}

Database::~Database() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase("SingletonConnection");
}

QSqlDatabase Database::getDatabase() const {
    return m_db;
}

bool Database::initialize() {
    qDebug() << "Initializing database connection...";

    if (!m_db.open()) {
        qCritical() << "Database connection failed:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully!";

    // Теперь соединение открыто — можно выполнять SQL‑команды
    QSqlQuery localeQuery(m_db);
    if (!localeQuery.exec("SET lc_messages = 'C'")) {
        qWarning() << "Failed to set locale for messages:" << localeQuery.lastError().text();
    } else {
        qDebug() << "Locale for messages set to 'C' (English)";
    }

    if (!executeSqlFile("init_database.sql")) {
        qCritical() << "Failed to execute initialization SQL file";
        return false;
    }

    return true;
}

bool Database::executeSqlFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open SQL file:" << filename;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        qCritical() << "SQL execution error:" << query.lastError().text();
        return false;
    }

    qDebug() << "SQL file executed successfully:" << filename;
    return true;
}
QString Database::hashPassword(const QString &password) {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hash = QCryptographicHash::hash(
        passwordBytes, QCryptographicHash::Sha256
        );
    return QString(hash.toHex());
}
