#include "clientgui.h"
#include "ui_clientgui.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>
#include <limits>

ClientGUI::ClientGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientGUI)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Настройки для SpinBox'ов (только первой функции, остальные скрываем)
    ui->a1SpinBox->setRange(-100.0, 100.0);
    ui->b1SpinBox->setRange(-100.0, 100.0);
    ui->c1SpinBox->setRange(-100.0, 100.0);
    ui->a1SpinBox->setValue(1.0);
    ui->b1SpinBox->setValue(0.0);
    ui->c1SpinBox->setValue(0.0);

    // Скрываем поля для второй и третьей функций (они не нужны)
    ui->groupBoxFunction2->hide();
    ui->groupBoxFunction3->hide();

    // Изменяем заголовок первой функции
    ui->groupBoxFunction1->setTitle("Параметры кусочно-заданной функции");

    ui->xStepSpinBox->setSingleStep(0.01);
    ui->xStepSpinBox->setValue(0.05);
    ui->xStartSpinBox->setValue(-10.0);
    ui->xEndSpinBox->setValue(10.0);

    // Подключаем сигналы сокета
    connect(socket, &QAbstractSocket::errorOccurred,
            this, &ClientGUI::onSocketError);
    connect(socket, &QTcpSocket::readyRead, this, &ClientGUI::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &ClientGUI::on_connected);

    // Изначально кнопка расчёта отключена до подключения
    ui->calculateButton->setEnabled(false);
    ui->calculateTripleButton->hide(); // Скрываем ненужную кнопку
}

ClientGUI::~ClientGUI()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(1000);
    }
    delete ui;
}
void ClientGUI::on_connectButton_clicked()
{
    QString host = ui->hostEdit->text();
    bool ok;
    int port = ui->portEdit->text().toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        port = 33333;
        ui->portEdit->setText("33333");
    }

    socket->connectToHost(host, port);
    ui->statusLabel->setText("Статус: подключение...");
    ui->connectButton->setEnabled(false);
}

void ClientGUI::on_calculateButton_clicked()
{
    double xStart = ui->xStartSpinBox->value();
    double xEnd = ui->xEndSpinBox->value();
    double step = ui->xStepSpinBox->value();

    if (step <= 0) {
        ui->statusLabel->setText("Статус: Ошибка - шаг должен быть положительным");
        return;
    }

    if (xStart >= xEnd) {
        ui->statusLabel->setText("Статус: Ошибка - X начало должно быть меньше X конца");
        return;
    }

    // Берём параметры первой функции
    double a = ui->a1SpinBox->value();
    double b = ui->b1SpinBox->value();
    double c = ui->c1SpinBox->value();

    QString request = QString("plot_system|%1|%2|%3|%4|%5|%6")
                          .arg(a).arg(b).arg(c)
                          .arg(xStart).arg(xEnd).arg(step);
    ui->statusLabel->setText("Статус: запрос отправлен на сервер...");

    socket->write((request + "\n").toUtf8());
    qDebug() << "Отправлен запрос:" << request;
}

void ClientGUI::onReadyRead()
{
    static QByteArray buffer;
    buffer += socket->readAll();

    qDebug() << "Получено данных (сырые):" << buffer;

    while (true) {
        int jsonStart = buffer.indexOf('{');
        if (jsonStart == -1) break;

        int braceCount = 0;
        int i = jsonStart;
        for (; i < buffer.size(); ++i) {
            if (buffer[i] == '{') braceCount++;
            else if (buffer[i] == '}') braceCount--;
            if (braceCount == 0) break;
        }

        if (braceCount != 0) break;

        QByteArray jsonData = buffer.mid(jsonStart, i - jsonStart + 1);
        buffer.remove(0, i + 1);

        qDebug() << "Найденный JSON:" << jsonData;

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << error.errorString();
            continue;
        }

        QJsonObject root = doc.object();
        QString type = root["type"].toString();

        if (type == "PLOT_DATA") {
            QJsonArray pointsArray = root["points"].toArray();
            bool isLimited = root["is_limited"].toBool();
            double originalStep = root["original_step"].toDouble();
            double adjustedStep = root["adjusted_step"].toDouble();
            int maxPoints = root["max_points"].toInt();

            processPlotData(pointsArray, isLimited, originalStep, adjustedStep, maxPoints);
            qDebug() << "График успешно получен и обработан";
        }
        else if (type == "PLOT_FAILED") {
            QString errorMessage = root["error"].toString();
            ui->statusLabel->setText("Статус: ошибка построения графика — " + errorMessage);
            qDebug() << "Ошибка от сервера:" << errorMessage;
        }
        else if (type == "PLOT_SYSTEM_FAILED") {
            QString errorMessage = root["error"].toString();
            ui->statusLabel->setText("Статус: ошибка построения графика — " + errorMessage);
            qDebug() << "Ошибка от сервера:" << errorMessage;
        }
        else {
            // Обработка обычных текстовых ответов от сервера (не JSON)
            QString response = QString::fromUtf8(jsonData);
            if (response.contains("Connected to Function Timp Server")) {
                ui->statusLabel->setText("Статус: подключено к серверу");
                qDebug() << "Сервер приветствует:" << response;
            } else if (response.contains("REG_SUCCESS")) {
                ui->statusLabel->setText("Статус: регистрация успешна");
                qDebug() << "Регистрация успешна";
            } else if (response.contains("REG_FAILED")) {
                ui->statusLabel->setText("Статус: регистрация не удалась");
                qDebug() << "Регистрация не удалась";
            } else if (response.contains("AUTH_SUCCESS")) {
                ui->statusLabel->setText("Статус: авторизация успешна");
                qDebug() << "Авторизация успешна";
            } else if (response.contains("AUTH_FAILED")) {
                ui->statusLabel->setText("Статус: авторизация не удалась");
                qDebug() << "Авторизация не удалась";
            } else if (response.contains("FORGOT_SENT")) {
                ui->statusLabel->setText("Статус: код восстановления отправлен");
                qDebug() << "Код восстановления отправлен";
            } else if (response.contains("EMAIL_NOT_FOUND")) {
                ui->statusLabel->setText("Статус: email не найден");
                qDebug() << "Email не найден";
            } else if (response.contains("RESET_SUCCESS")) {
                ui->statusLabel->setText("Статус: пароль успешно изменён");
                qDebug() << "Пароль успешно изменён";
            } else if (response.contains("RESET_FAILED")) {
                ui->statusLabel->setText("Статус: ошибка восстановления пароля");
                qDebug() << "Ошибка восстановления пароля";
            } else {
                qDebug() << "Текстовый ответ сервера:" << response;
            }
        }
    }
}

void ClientGUI::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    ui->statusLabel->setText("Статус: ошибка подключения к серверу");
    ui->connectButton->setEnabled(true);
    ui->calculateButton->setEnabled(false);
    qDebug() << "Ошибка сокета:" << socket->errorString();
}

void ClientGUI::on_connected()
{
    ui->statusLabel->setText("Статус: подключено к серверу");
    ui->connectButton->setEnabled(true);
    ui->calculateButton->setEnabled(true);
    qDebug() << "Успешно подключено к серверу";
}

QVector<ClientGUI::PlotSegment> ClientGUI::splitPointsBySegments(const QVector<QPointF>& points,
                                                                 double a, double b, double c)
{
    QVector<PlotSegment> segments;

    if (points.isEmpty()) return segments;

    // Сегмент 1: x < 0 (красный)
    PlotSegment segment1;
    segment1.color = "red";
    segment1.name = QString("x < 0: ∛x + %1").arg(a);

    // Сегмент 2: 0 < x < 1 (синий)
    PlotSegment segment2;
    segment2.color = "blue";
    segment2.name = QString("0 < x < 1: 1/x - %1").arg(b);

    // Сегмент 3: x >= 1 (зелёный)
    PlotSegment segment3;
    segment3.color = "green";
    segment3.name = QString("x ≥ 1: x² - %1·x + 1").arg(c);

    for (const QPointF& point : points) {
        if (!std::isfinite(point.y())) continue;

        if (point.x() < 0) {
            segment1.points.append(point);
        } else if (point.x() > 0 && point.x() < 1) {
            segment2.points.append(point);
        } else if (point.x() >= 1) {
            segment3.points.append(point);
        }
        // x = 0 - пропускаем (особая точка, разрыв)
    }

    // Добавляем только непустые сегменты
    if (!segment1.points.isEmpty()) segments.append(segment1);
    if (!segment2.points.isEmpty()) segments.append(segment2);
    if (!segment3.points.isEmpty()) segments.append(segment3);

    qDebug() << "Разбито на сегменты:"
             << "x<0:" << segment1.points.size()
             << "0<x<1:" << segment2.points.size()
             << "x>=1:" << segment3.points.size();

    return segments;
}

void ClientGUI::processPlotData(const QJsonArray &points,
                                bool isLimited,
                                double originalStep,
                                double adjustedStep,
                                int maxPoints)
{
    Q_UNUSED(isLimited)
    Q_UNUSED(originalStep)
    Q_UNUSED(adjustedStep)
    Q_UNUSED(maxPoints)

    QVector<QPointF> allPoints;

    qDebug() << "Получено точек для графика:" << points.size();

    for (const QJsonValue &pointValue : points) {
        QJsonObject pointObj = pointValue.toObject();
        double x = pointObj["x"].toDouble();
        double y = pointObj["y"].toDouble();

        if (std::isfinite(y) && std::abs(y) <= 10000.0) {
            allPoints.append(QPointF(x, y));
        }
    }

    if (!allPoints.isEmpty()) {
        double a = ui->a1SpinBox->value();
        double b = ui->b1SpinBox->value();
        double c = ui->c1SpinBox->value();

        // Разбиваем точки на три сегмента с разными цветами
        QVector<PlotSegment> segments = splitPointsBySegments(allPoints, a, b, c);
        updateChart(segments);
        ui->statusLabel->setText("Статус: график построен");
    } else {
        ui->statusLabel->setText("Статус: нет данных для построения графика");
    }
}

void ClientGUI::updateChart(const QVector<PlotSegment>& segments)
{
    if (segments.isEmpty()) {
        qDebug() << "Нет данных для построения графика";
        ui->statusLabel->setText("Статус: нет данных для построения графика");

        if (ui->chartView && ui->chartView->chart()) {
            ui->chartView->chart()->removeAllSeries();
        }
        return;
    }

    if (!ui->chartView) {
        qDebug() << "Ошибка: chartView не инициализирован";
        ui->statusLabel->setText("Статус: ошибка — график не инициализирован");
        return;
    }

    // Очищаем существующий график правильно
    if (ui->chartView->chart()) {
        QChart *oldChart = ui->chartView->chart();
        oldChart->removeAllSeries();
        // Не удаляем старый график сразу, а просто отключаем
        // Удалим его после использования нового
    }

    // Создаём новый график
    QChart *chart = new QChart();
    chart->setTitle("Кусочно-заданная функция f(x)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundBrush(QBrush(QColor(15, 15, 15)));
    chart->setTitleBrush(QBrush(Qt::white));

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    // Создаём серии для каждого сегмента
    for (const PlotSegment &segment : segments) {
        QLineSeries *series = new QLineSeries();
        series->setName(segment.name);

        // Устанавливаем цвет
        QColor color;
        if (segment.color == "red") color = Qt::red;
        else if (segment.color == "blue") color = Qt::blue;
        else if (segment.color == "green") color = Qt::green;
        else color = Qt::cyan;

        series->setPen(QPen(color, 2.5));

        for (const QPointF &point : segment.points) {
            if (std::isfinite(point.y())) {
                series->append(point);
                if (point.x() < minX) minX = point.x();
                if (point.x() > maxX) maxX = point.x();
                if (point.y() < minY) minY = point.y();
                if (point.y() > maxY) maxY = point.y();
            }
        }
        chart->addSeries(series);
    }

    // Добавляем оси
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X");
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setLabelsColor(Qt::white);
    axisX->setLabelFormat("%.1f");

    // Добавляем отступы для осей
    double paddingX = (maxX - minX) * 0.05;
    if (paddingX <= 0 || !std::isfinite(paddingX)) paddingX = 1.0;
    axisX->setRange(minX - paddingX, maxX + paddingX);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Добавляем вертикальные линии разрыва
    if (minX <= 0 && maxX >= 0) {
        QLineSeries *breakLine1 = new QLineSeries();
        breakLine1->setName("x = 0 (разрыв)");
        breakLine1->setPen(QPen(QColor(128, 128, 128), 1, Qt::DashLine));
        breakLine1->append(0, minY - 5);
        breakLine1->append(0, maxY + 5);
        chart->addSeries(breakLine1);
    }

    if (minX <= 1 && maxX >= 1) {
        QLineSeries *breakLine2 = new QLineSeries();
        breakLine2->setName("x = 1 (разрыв)");
        breakLine2->setPen(QPen(QColor(128, 128, 128), 1, Qt::DashLine));
        breakLine2->append(1, minY - 5);
        breakLine2->append(1, maxY + 5);
        chart->addSeries(breakLine2);
    }

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y");
    axisY->setTitleBrush(QBrush(Qt::white));
    axisY->setLabelsColor(Qt::white);
    axisY->setLabelFormat("%.2f");

    double paddingY = (maxY - minY) * 0.1;
    if (paddingY <= 0 || !std::isfinite(paddingY)) paddingY = 1.0;
    axisY->setRange(minY - paddingY, maxY + paddingY);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Привязываем оси ко всем сериям
    for (QAbstractSeries *abstractSeries : chart->series()) {
        if (QLineSeries *lineSeries = qobject_cast<QLineSeries*>(abstractSeries)) {
            lineSeries->attachAxis(axisX);
            lineSeries->attachAxis(axisY);
        }
    }

    // Сохраняем старый график и устанавливаем новый
    QChart *oldChart = ui->chartView->chart();
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    // Удаляем старый график после установки нового
    if (oldChart) {
        oldChart->deleteLater();
    }

    qDebug() << "График обновлён с" << segments.size() << "сегментами";
}
