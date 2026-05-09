#include "clientgui.h"
#include "ui_clientgui.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>
#include <limits>
#include <QHeaderView>
#include <QTimer>

ClientGUI::ClientGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientGUI)
    , socket(new QTcpSocket(this))
    , updateTimer(nullptr)
    , autoUpdateEnabled(true)
{
    ui->setupUi(this);

    // ========== НАСТРОЙКА ПОЛЗУНКОВ ==========
    // Настройка ползунков для параметра a
    ui->aSlider->setRange(-100, 100);
    ui->aSlider->setValue(100);  // 1.00 * 100 = 100
    ui->aSlider->setTickInterval(20);
    ui->aSlider->setTickPosition(QSlider::TicksBelow);

    // Настройка ползунков для параметра b
    ui->bSlider->setRange(-100, 100);
    ui->bSlider->setValue(0);
    ui->bSlider->setTickInterval(20);
    ui->bSlider->setTickPosition(QSlider::TicksBelow);

    // Настройка ползунков для параметра c
    ui->cSlider->setRange(-100, 100);
    ui->cSlider->setValue(0);
    ui->cSlider->setTickInterval(20);
    ui->cSlider->setTickPosition(QSlider::TicksBelow);

    // Отображаем значения параметров на ползунках
    ui->aValueLabel->setText("1.00");
    ui->bValueLabel->setText("0.00");
    ui->cValueLabel->setText("0.00");

    // Настройки для SpinBox'ов (синхронизация с ползунками)
    ui->a1SpinBox->setRange(-100.0, 100.0);
    ui->b1SpinBox->setRange(-100.0, 100.0);
    ui->c1SpinBox->setRange(-100.0, 100.0);
    ui->a1SpinBox->setValue(1.0);
    ui->b1SpinBox->setValue(0.0);
    ui->c1SpinBox->setValue(0.0);

    // ========== КОММЕНТИРУЕМ СТРОКИ, КОТОРЫЕ ВЫЗЫВАЮТ ОШИБКУ ==========
    // Этих groupBox'ов больше нет в UI файле, поэтому закомментируем
    // ui->groupBoxFunction2->hide();
    // ui->groupBoxFunction3->hide();

    // Изменяем заголовок первой функции
    ui->groupBoxFunction1->setTitle("Параметры функции (ползунки -100..100)");

    // Настройки диапазона X
    ui->xStepSpinBox->setSingleStep(0.01);
    ui->xStepSpinBox->setValue(0.05);
    ui->xStartSpinBox->setValue(-10.0);
    ui->xEndSpinBox->setValue(10.0);

    // Подключаем сигналы изменения ползунков
    connect(ui->aSlider, &QSlider::valueChanged, this, &ClientGUI::on_aSlider_valueChanged);
    connect(ui->bSlider, &QSlider::valueChanged, this, &ClientGUI::on_bSlider_valueChanged);
    connect(ui->cSlider, &QSlider::valueChanged, this, &ClientGUI::on_cSlider_valueChanged);

    // Подключаем сигналы изменения спинбоксов
    connect(ui->a1SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::on_aSpinBox_valueChanged);
    connect(ui->b1SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::on_bSpinBox_valueChanged);
    connect(ui->c1SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::on_cSpinBox_valueChanged);

    // Подключаем сигналы изменения диапазона X для автоматического обновления
    connect(ui->xStartSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::autoUpdatePlot);
    connect(ui->xEndSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::autoUpdatePlot);
    connect(ui->xStepSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ClientGUI::autoUpdatePlot);

    // Настройка таймера для дебаунса (чтобы не слать слишком много запросов)
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(300);  // 300 мс задержки
    connect(updateTimer, &QTimer::timeout, this, &ClientGUI::sendPlotRequest);

    // Настройка таблицы для отображения координат
    setupPointsTable();

    // Подключаем сигналы сокета
    connect(socket, &QAbstractSocket::errorOccurred,
            this, &ClientGUI::onSocketError);
    connect(socket, &QTcpSocket::readyRead, this, &ClientGUI::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &ClientGUI::on_connected);

    // Изначально кнопка расчёта отключена до подключения
    ui->calculateButton->setEnabled(false);
    ui->calculateTripleButton->hide(); // Скрываем ненужную кнопку
    ui->calculateButton->setText("🔄 Автообновление: ВКЛ");
    ui->calculateButton->setStyleSheet("background-color: #28A745; font-size: 11px; padding: 6px 12px;");
}

ClientGUI::~ClientGUI()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(1000);
    }
    delete ui;
}

// ========== ОБРАБОТЧИКИ ПОЛЗУНКОВ ==========
void ClientGUI::on_aSlider_valueChanged(int value)
{
    double doubleValue = value / 100.0;
    ui->aValueLabel->setText(QString::number(doubleValue, 'f', 2));

    // Блокируем сигналы спинбокса, чтобы избежать циклического вызова
    ui->a1SpinBox->blockSignals(true);
    ui->a1SpinBox->setValue(doubleValue);
    ui->a1SpinBox->blockSignals(false);

    autoUpdatePlot();
}

void ClientGUI::on_bSlider_valueChanged(int value)
{
    double doubleValue = value / 100.0;
    ui->bValueLabel->setText(QString::number(doubleValue, 'f', 2));

    ui->b1SpinBox->blockSignals(true);
    ui->b1SpinBox->setValue(doubleValue);
    ui->b1SpinBox->blockSignals(false);

    autoUpdatePlot();
}

void ClientGUI::on_cSlider_valueChanged(int value)
{
    double doubleValue = value / 100.0;
    ui->cValueLabel->setText(QString::number(doubleValue, 'f', 2));

    ui->c1SpinBox->blockSignals(true);
    ui->c1SpinBox->setValue(doubleValue);
    ui->c1SpinBox->blockSignals(false);

    autoUpdatePlot();
}

// ========== ОБРАБОТЧИКИ СПИНБОКСОВ (синхронизация с ползунками) ==========
void ClientGUI::on_aSpinBox_valueChanged(double value)
{
    ui->aSlider->blockSignals(true);
    ui->aSlider->setValue(static_cast<int>(value * 100));
    ui->aSlider->blockSignals(false);
    ui->aValueLabel->setText(QString::number(value, 'f', 2));

    autoUpdatePlot();
}

void ClientGUI::on_bSpinBox_valueChanged(double value)
{
    ui->bSlider->blockSignals(true);
    ui->bSlider->setValue(static_cast<int>(value * 100));
    ui->bSlider->blockSignals(false);
    ui->bValueLabel->setText(QString::number(value, 'f', 2));

    autoUpdatePlot();
}

void ClientGUI::on_cSpinBox_valueChanged(double value)
{
    ui->cSlider->blockSignals(true);
    ui->cSlider->setValue(static_cast<int>(value * 100));
    ui->cSlider->blockSignals(false);
    ui->cValueLabel->setText(QString::number(value, 'f', 2));

    autoUpdatePlot();
}

// ========== АВТОМАТИЧЕСКОЕ ОБНОВЛЕНИЕ ГРАФИКА ==========
void ClientGUI::autoUpdatePlot()
{
    if (!autoUpdateEnabled) return;
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) return;

    // Перезапускаем таймер для дебаунса
    if (updateTimer) {
        updateTimer->start();
    }
}

void ClientGUI::sendPlotRequest()
{
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) {
        ui->statusLabel->setText("Статус: не подключено к серверу");
        return;
    }

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

    // Берём параметры
    double a = ui->a1SpinBox->value();
    double b = ui->b1SpinBox->value();
    double c = ui->c1SpinBox->value();

    QString request = QString("plot_system|%1|%2|%3|%4|%5|%6")
                          .arg(a).arg(b).arg(c)
                          .arg(xStart).arg(xEnd).arg(step);

    socket->write((request + "\n").toUtf8());
    qDebug() << "Отправлен запрос:" << request;
}

void ClientGUI::setupPointsTable()
{
    // Настройка заголовков таблицы
    QStringList headers;
    headers << "X" << "Y";
    ui->pointsTable->setColumnCount(2);
    ui->pointsTable->setHorizontalHeaderLabels(headers);

    // Настройка внешнего вида таблицы
    ui->pointsTable->setAlternatingRowColors(true);
    ui->pointsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->pointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Автоматическое изменение размера столбцов
    ui->pointsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pointsTable->horizontalHeader()->setStretchLastSection(true);

    // Установка стиля таблицы
    ui->pointsTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: #2b2b2b;"
        "   alternate-background-color: #3a3a3a;"
        "   gridline-color: #555;"
        "   color: #ffffff;"
        "}"
        "QHeaderView::section {"
        "   background-color: #1e1e1e;"
        "   padding: 5px;"
        "   border: 1px solid #555;"
        "   font-weight: bold;"
        "   color: #ffffff;"
        "}"
        "QTableWidget::item {"
        "   padding: 3px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #4a90e2;"
        "}"
        );

    ui->pointsTable->setFont(QFont("Arial", 9));
}

void ClientGUI::updatePointsTable(const QVector<QPointF>& points)
{
    // Очищаем таблицу
    ui->pointsTable->clearContents();
    ui->pointsTable->setRowCount(0);

    if (points.isEmpty()) {
        ui->statusLabel->setText("Статус: нет данных для отображения в таблице");
        return;
    }

    // Заполняем таблицу данными
    ui->pointsTable->setRowCount(points.size());

    for (int i = 0; i < points.size(); ++i) {
        const QPointF& point = points[i];

        // Форматируем числа с точностью до 4 знаков после запятой
        QString xStr = QString::number(point.x(), 'f', 4);
        QString yStr = QString::number(point.y(), 'f', 4);

        QTableWidgetItem* xItem = new QTableWidgetItem(xStr);
        QTableWidgetItem* yItem = new QTableWidgetItem(yStr);

        // Выравнивание по правому краю для чисел
        xItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        yItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Устанавливаем цвет для особых значений
        if (std::isinf(point.y()) || std::isnan(point.y())) {
            yItem->setForeground(Qt::red);
            yItem->setText("undefined");
        } else if (std::abs(point.y()) > 1e6) {
            yItem->setForeground(QColor(255, 165, 0));
        }

        ui->pointsTable->setItem(i, 0, xItem);
        ui->pointsTable->setItem(i, 1, yItem);
    }

    ui->statusLabel->setText(QString("Статус: график построен. Отображено %1 точек").arg(points.size()));
    ui->pointsTable->scrollToTop();
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
    // Переключатель автообновления
    autoUpdateEnabled = !autoUpdateEnabled;

    if (autoUpdateEnabled) {
        ui->calculateButton->setText("🔄 Автообновление: ВКЛ");
        ui->calculateButton->setStyleSheet("background-color: #28A745; font-size: 11px; padding: 6px 12px;");
        ui->statusLabel->setText("Статус: автообновление включено. Изменяйте параметры — график обновится автоматически.");
        sendPlotRequest();  // Отправляем запрос сразу
    } else {
        ui->calculateButton->setText("⏸ Автообновление: ВЫКЛ");
        ui->calculateButton->setStyleSheet("background-color: #DC3545; font-size: 11px; padding: 6px 12px;");
        ui->statusLabel->setText("Статус: автообновление выключено. Нажмите кнопку снова для включения.");
    }
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

    // При подключении отправляем начальный запрос
    if (autoUpdateEnabled) {
        sendPlotRequest();
    }
}

QVector<ClientGUI::PlotSegment> ClientGUI::splitPointsBySegments(const QVector<QPointF>& points,
                                                                 double a, double b, double c)
{
    QVector<PlotSegment> segments;

    if (points.isEmpty()) return segments;

    PlotSegment segment1;
    segment1.color = "red";
    segment1.name = QString("f₁(x) = ∛x + %1, x < 0").arg(a, 0, 'g', 2);

    PlotSegment segment2;
    segment2.color = "blue";
    segment2.name = QString("f₂(x) = 1/x - %1, 0 < x < 1").arg(b, 0, 'g', 2);

    PlotSegment segment3;
    segment3.color = "green";
    segment3.name = QString("f₃(x) = x² - %1·x + 1, x ≥ 1").arg(c, 0, 'g', 2);

    for (const QPointF& point : points) {
        if (!std::isfinite(point.y())) continue;

        if (point.x() < 0) {
            segment1.points.append(point);
        } else if (point.x() > 0 && point.x() < 1) {
            segment2.points.append(point);
        } else if (point.x() >= 1) {
            segment3.points.append(point);
        }
    }

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

        QVector<PlotSegment> segments = splitPointsBySegments(allPoints, a, b, c);
        updateChart(segments);
        updatePointsTable(allPoints);
    } else {
        ui->statusLabel->setText("Статус: нет данных для построения графика");
        ui->pointsTable->clearContents();
        ui->pointsTable->setRowCount(0);
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

    if (ui->chartView->chart()) {
        QChart *oldChart = ui->chartView->chart();
        oldChart->removeAllSeries();
    }

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

    for (const PlotSegment &segment : segments) {
        QLineSeries *series = new QLineSeries();
        series->setName(segment.name);

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

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X");
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setLabelsColor(Qt::white);
    axisX->setLabelFormat("%.1f");

    double paddingX = (maxX - minX) * 0.05;
    if (paddingX <= 0 || !std::isfinite(paddingX)) paddingX = 1.0;
    axisX->setRange(minX - paddingX, maxX + paddingX);
    chart->addAxis(axisX, Qt::AlignBottom);

    if (minX <= 0 && maxX >= 0) {
        QLineSeries *boundaryLine1 = new QLineSeries();
        boundaryLine1->setName("x = 0 (граница участков)");
        boundaryLine1->setPen(QPen(QColor(255, 255, 255, 100), 1, Qt::DashLine));
        boundaryLine1->append(0, minY - 5);
        boundaryLine1->append(0, maxY + 5);
        chart->addSeries(boundaryLine1);
    }

    if (minX <= 1 && maxX >= 1) {
        QLineSeries *boundaryLine2 = new QLineSeries();
        boundaryLine2->setName("x = 1 (граница участков)");
        boundaryLine2->setPen(QPen(QColor(255, 255, 255, 100), 1, Qt::DashLine));
        boundaryLine2->append(1, minY - 5);
        boundaryLine2->append(1, maxY + 5);
        chart->addSeries(boundaryLine2);
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

    for (QAbstractSeries *abstractSeries : chart->series()) {
        if (QLineSeries *lineSeries = qobject_cast<QLineSeries*>(abstractSeries)) {
            lineSeries->attachAxis(axisX);
            lineSeries->attachAxis(axisY);
        }
    }

    QChart *oldChart = ui->chartView->chart();
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    if (oldChart) {
        oldChart->deleteLater();
    }

    qDebug() << "График обновлён с" << segments.size() << "сегментами";
}