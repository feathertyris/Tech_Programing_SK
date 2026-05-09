#include "clientgui.h"
#include "ui_clientgui.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QValueAxis>
#include <limits>

ClientGUI::ClientGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientGUI)
    , m_username("")
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    mBuffer.clear();

    setupChart();

    connect(mSocket, &QTcpSocket::connected, this, &ClientGUI::onConnected);
    connect(mSocket, &QTcpSocket::disconnected, this, &ClientGUI::onDisconnected);
    connect(mSocket, &QTcpSocket::readyRead, this, &ClientGUI::onReadyRead);
    connect(mSocket, &QAbstractSocket::errorOccurred, this, &ClientGUI::onSocketError);

    // Инициализация полей ввода
    ui->hostEdit->setText("localhost");
    ui->portEdit->setText("33333"); // убедитесь, что тут 33333
    ui->aSpinBox->setValue(1.0);
    ui->bSpinBox->setValue(0.0);
    ui->cSpinBox->setValue(0.0);
    ui->xStartSpinBox->setValue(-10.0);
    ui->xEndSpinBox->setValue(10.0);
    ui->xStepSpinBox->setValue(0.1);
}

ClientGUI::~ClientGUI()
{
    delete ui;
}

void ClientGUI::setupChart()
{
    if (mChart) {
        // График уже создан
        return;
    }

    // Создаём график и серии
    mChart = new QChart();
    mChart->setTitle("Три графика функций");
    mChart->setAnimationOptions(QChart::SeriesAnimations);

    mSeries1 = new QLineSeries();
    mSeries1->setName("f(x)");
    mSeries1->setPen(QPen(Qt::blue, 2));

    mSeries2 = new QLineSeries();
    mSeries2->setName("g(x)");
    mSeries2->setPen(QPen(Qt::red, 2));

    mSeries3 = new QLineSeries();
    mSeries3->setName("h(x)");
    mSeries3->setPen(QPen(Qt::green, 2));

    mChart->addSeries(mSeries1);
    mChart->addSeries(mSeries2);
    mChart->addSeries(mSeries3);

    // Настраиваем оси с поддержкой отрицательных значений
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X");
    axisX->setLabelFormat("%.1f");
    axisX->setTickCount(11); // 11 делений для симметрии

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y");
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(11);

    mChart->addAxis(axisX, Qt::AlignBottom);
    mChart->addAxis(axisY, Qt::AlignLeft);

    mSeries1->attachAxis(axisX);
    mSeries1->attachAxis(axisY);
    mSeries2->attachAxis(axisX);
    mSeries2->attachAxis(axisY);
    mSeries3->attachAxis(axisX);
    mSeries3->attachAxis(axisY);

    // Создаём и размещаем вид
    mChartView = new QChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->setMinimumSize(400, 300);

    // Удаляем предыдущие виджеты
    if (ui->chartContainer->layout()) {
        QLayout *layout = ui->chartContainer->layout();
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                delete child->widget();
            }
            delete child;
        }
        delete layout;
    }
    QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
    layout->addWidget(mChartView);
}

void ClientGUI::updateAxisRanges()
{
    if (!mChart || !mSeries1 || !mSeries2 || !mSeries3) return;

    double minX = ui->xStartSpinBox->value();
    double maxX = ui->xEndSpinBox->value();

    // Определяем диапазон Y по всем точкам
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    auto updateMinMax = [&](const QLineSeries *series) {
        for (const QPointF &point : series->points()) {
            if (!std::isnan(point.y()) && !std::isinf(point.y())) {
                minY = std::min(minY, point.y());
                maxY = std::max(maxY, point.y());
            }
        }
    };

    updateMinMax(mSeries1);
    updateMinMax(mSeries2);
    updateMinMax(mSeries3);

    // Если все значения NaN или Inf, устанавливаем дефолтный диапазон
    if (minY == std::numeric_limits<double>::max()) {
        minY = -10.0;
        maxY = 10.0;
    }

    // Добавляем небольшой отступ для лучшего отображения
    double xMargin = (maxX - minX) * 0.05;
    double yMargin = (maxY - minY) * 0.1;

    // Обновляем оси
    if (!mChart->axes(Qt::Horizontal).isEmpty()) {
        auto axisX = qobject_cast<QValueAxis*>(mChart->axes(Qt::Horizontal).first());
        if (axisX) {
            axisX->setRange(minX - xMargin, maxX + xMargin);
        }
    }

    if (!mChart->axes(Qt::Vertical).isEmpty()) {
        auto axisY = qobject_cast<QValueAxis*>(mChart->axes(Qt::Vertical).first());
        if (axisY) {
            double finalMinY = std::min(minY - yMargin, 0.0);
            double finalMaxY = std::max(maxY + yMargin, 0.0);
            axisY->setRange(finalMinY, finalMaxY);
        }
    }
}

void ClientGUI::on_connectButton_clicked()
{
    QString host = ui->hostEdit->text();
    bool ok;
    quint16 port = ui->portEdit->text().toUShort(&ok);
    if (!ok) {
        ui->statusLabel->setText("Некорректный номер порта");
        return;
    }

    qDebug() << "Connecting to" << host << "on port" << port;
    mSocket->connectToHost(host, port);

    if (mSocket->waitForConnected(3000)) {
        qDebug() << "Connected!";
        sendAuthRequest(); // Отправляем запрос авторизации после подключения
    } else {
        qDebug() << "Failed to connect:" << mSocket->errorString();
        ui->statusLabel->setText("Ошибка подключения");
    }
    ui->statusLabel->setText("Подключение к серверу...");
}

void ClientGUI::sendAuthRequest()
{
    QJsonObject authRequest;
    authRequest["type"] = "auth";
    authRequest["username"] = m_username;

    QJsonDocument doc(authRequest);
    mSocket->write(doc.toJson() + "\n");
    mSocket->flush();
}

void ClientGUI::handleAuthResponse(const QByteArray &response)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "Ошибка", "Ошибка парсинга ответа авторизации");
        return;
    }

    QJsonObject responseObj = doc.object();
    QString status = responseObj["status"].toString();

    if (status == "success") {
        ui->statusLabel->setText("Авторизован как: " + m_username);
        // Теперь можно использовать основные функции
        ui->calculateButton->setEnabled(true);
    } else {
        QString errorMsg = responseObj["error"].toString();
        QMessageBox::critical(this, "Ошибка авторизации", errorMsg);
        ui->statusLabel->setText("Ошибка авторизации");
    }
}

void ClientGUI::on_calculateButton_clicked()
{
    if (mSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray request = createRequest();
        mSocket->write(request + "\n"); // добавляем \n для разделения
        ui->statusLabel->setText("Отправлен запрос на построение графиков...");
        qDebug() << "Request sent:" << request;
    } else {
        ui->statusLabel->setText("Ошибка: не подключено к серверу");
        qDebug() << "Socket not connected";
    }
}

QByteArray ClientGUI::createRequest()
{
    QJsonObject request;
    request["a"] = ui->aSpinBox->value();
    request["b"] = ui->bSpinBox->value();
    request["c"] = ui->cSpinBox->value();
    request["x_start"] = ui->xStartSpinBox->value();
    request["x_end"] = ui->xEndSpinBox->value();
    request["x_step"] = ui->xStepSpinBox->value();

    QJsonDocument doc(request);
    return doc.toJson(QJsonDocument::Compact);
}

void ClientGUI::parseResponse(const QByteArray &response)
{
    try {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(response, &error);
        if (error.error != QJsonParseError::NoError) {
            QMessageBox::critical(this, "Ошибка", QString("Ошибка парсинга JSON: %1").arg(error.errorString()));
            ui->statusLabel->setText("Ошибка при получении данных");
            return;
        }

        QJsonObject responseObj = doc.object();
        QString status = responseObj["status"].toString();

        if (status == "success") {
            // Очистка предыдущих данных
            clearChart();

            QJsonArray points1 = responseObj["points_f"].toArray();
            QJsonArray points2 = responseObj["points_g"].toArray();
            QJsonArray points3 = responseObj["points_h"].toArray();

            mPointCount = 0;

            // Добавление точек с проверкой на валидность
            for (const QJsonValue &val : points1) {
                QJsonObject point = val.toObject();
                double x = point["x"].toDouble();
                double f_x = point["f_x"].toDouble();

                if (!std::isnan(f_x) && !std::isinf(f_x)) {
                    mSeries1->append(x, f_x);
                    mPointCount++;
                }
            }

            for (const QJsonValue &val : points2) {
                QJsonObject point = val.toObject();
                double x = point["x"].toDouble();
                double g_x = point["g_x"].toDouble();

                if (!std::isnan(g_x) && !std::isinf(g_x)) {
                    mSeries2->append(x, g_x);
                }
            }

            for (const QJsonValue &val : points3) {
                QJsonObject point = val.toObject();
                double x = point["x"].toDouble();
                double h_x = point["h_x"].toDouble();

                if (!std::isnan(h_x) && !std::isinf(h_x)) {
                    mSeries3->append(x, h_x);
                }
            }

            // Явное обновление графика
            updateAxisRanges();
            mChart->update(); // Принудительное обновление графика

            ui->statusLabel->setText(QString("Построено %1 точек: f(x)=%2, g(x)=%3, h(x)=%4")
                                         .arg(mPointCount)
                                         .arg(points1.size())
                                         .arg(points2.size())
                                         .arg(points3.size()));
        } else {
            QMessageBox::critical(this, "Ошибка сервера", responseObj["error"].toString());
            ui->statusLabel->setText("Ошибка от сервера");
        }
    } catch (...) {
        qDebug() << "CRITICAL ERROR: Exception in parseResponse";
        ui->statusLabel->setText("Критическая ошибка при обработке данных");
    }
}
void ClientGUI::onConnected()
{
    ui->statusLabel->setText("Подключено к серверу");
    ui->connectButton->setEnabled(false);
    ui->calculateButton->setEnabled(true);
    qDebug() << "Connected to server.";
}

void ClientGUI::onDisconnected()
{
    ui->statusLabel->setText("Отключено от сервера");
    ui->connectButton->setEnabled(true);
    ui->calculateButton->setEnabled(false);
    qDebug() << "Disconnected from server.";
}

void ClientGUI::onReadyRead()
{
    QByteArray incomingData = mSocket->readAll();
    mBuffer.append(incomingData);
    qDebug() << "Incoming data:" << incomingData;
    qDebug() << "Buffer size:" << mBuffer.size();

    // Разбиваем буфер на строки по символу '\n'
    QList<QByteArray> lines = mBuffer.split('\n');

    // Оставляем последнюю часть буфера (возможно, неполную строку) для следующего чтения
    mBuffer = lines.takeLast();

    for (const QByteArray &line : lines) {
        QByteArray trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty()) continue;

        if (trimmedLine.startsWith("{") && trimmedLine.endsWith("}")) {
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(trimmedLine, &error);
            if (error.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << error.errorString();
                continue;
            }

            QJsonObject obj = doc.object(); // Объявляем obj здесь

            if (obj["type"].toString() == "auth_response") {
                handleAuthResponse(trimmedLine);
            } else {
                parseResponse(trimmedLine);
            }
        }
    }
}


void ClientGUI::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    QMessageBox::critical(this, "Ошибка соединения",
                          QString("Ошибка: %1").arg(mSocket->errorString()));
    ui->statusLabel->setText("Ошибка соединения");
    qDebug() << "Socket error:" << mSocket->errorString();
}
void ClientGUI::clearChart()
{
    if (mSeries1) mSeries1->clear();
    if (mSeries2) mSeries2->clear();
    if (mSeries3) mSeries3->clear();
}

void ClientGUI::resetChart()
{
    clearChart();
    updateAxisRanges();
    mChart->update();
}

void ClientGUI::setUsername(const QString &username)
{
    m_username = username;
}
