#include "clientgui.h"
#include "ui_clientgui.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ClientGUI::ClientGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientGUI)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Настройки для лучшего контроля над графиком
    ui->aSpinBox->setRange(-100.0, 100.0);
    ui->bSpinBox->setRange(-100.0, 100.0);
    ui->cSpinBox->setRange(-100.0, 100.0);
    ui->xStepSpinBox->setSingleStep(0.01);  // Мелкий шаг изменения
    ui->xStepSpinBox->setValue(0.05);

    connect(socket, &QAbstractSocket::errorOccurred,
            this, &ClientGUI::onSocketError);
    connect(socket, &QTcpSocket::readyRead, this, &ClientGUI::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &ClientGUI::on_connected);
}

ClientGUI::~ClientGUI()
{
    delete ui;
}

void ClientGUI::on_connectButton_clicked()
{
    QString host = ui->hostEdit->text();
    bool ok;
    int port = ui->portEdit->text().toInt(&ok);
    if (!ok) {
        port = 33333;
    }
    socket->connectToHost(host, port);
    ui->statusLabel->setText("Статус: подключение...");
}

void ClientGUI::on_calculateButton_clicked()
{
    double a = ui->aSpinBox->value();
    double b = ui->bSpinBox->value();
    double c = ui->cSpinBox->value();

    // Используем корректные имена из UI-файла
    double xStart = ui->xStartSpinBox->value();
    double xEnd = ui->xEndSpinBox->value();
    double step = ui->xStepSpinBox->value();  // Было: stepSpinBox, стало: xStepSpinBox

    if (step <= 0) {
        qDebug() << "Ошибка: шаг должен быть положительным";
        ui->statusLabel->setText("Ошибка: шаг должен быть положительным");
        return;
    }

    QString request = QString("plot_system|%1|%2|%3|%4|%5|%6")
                          .arg(a).arg(b).arg(c)
                          .arg(xStart).arg(xEnd).arg(step);

    socket->write((request + "\n").toUtf8());
    ui->statusLabel->setText("Статус: запрос отправлен");
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
        if (root["type"].toString() == "PLOT_DATA") {
            QJsonArray points = root["points"].toArray();
            bool isLimited = root["is_limited"].toBool();
            double originalStep = 0.0, adjustedStep = 0.0;
            int maxPoints = 0;

            if (isLimited) {
                originalStep = root["original_step"].toDouble();
                adjustedStep = root["adjusted_step"].toDouble();
                maxPoints = root["max_points"].toInt();
            }
            processPlotData(points, isLimited, originalStep, adjustedStep, maxPoints);
            qDebug() << "Данные графика успешно получены и обработаны. Точек:" << points.size();
        } else if (root["type"].toString() == "PLOT_FAILED") {
            QString errorMessage = root["error"].toString();
            ui->statusLabel->setText("Статус: ошибка построения графика — " + errorMessage);
            qDebug() << "Ошибка от сервера:" << errorMessage;
        }
    }
}










void ClientGUI::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    ui->statusLabel->setText("Статус: ошибка подключения к серверу");
}

void ClientGUI::on_connected()
{
    ui->statusLabel->setText("Статус: подключено к серверу");
    ui->calculateButton->setEnabled(true);
}

void ClientGUI::updateChart(const QVector<QPointF> &points)
{
    if (points.isEmpty()) {
        qDebug() << "Нет данных для построения графика";
        return;
    }

    // Очищаем предыдущий график
    if (ui->chartView->chart()) {
        delete ui->chartView->chart();
    }

    QChart *chart = new QChart();
    chart->setTitle("График функции");

    QLineSeries *series = new QLineSeries();
    for (const QPointF &point : points) {
        series->append(point);
    }
    chart->addSeries(series);

    // Настройка осей
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X");
    axisX->setLabelFormat("%.1f");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y");
    axisY->setLabelFormat("%.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Привязка к chartView
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    qDebug() << "График обновлён с" << points.size() << "точками";
}


void ClientGUI::processPlotData(const QJsonArray &points,
                                bool isLimited,
                                double originalStep,
                                double adjustedStep,
                                int maxPoints)
{
    QVector<QPointF> data;
    double maxY = 1000.0;  // Максимальное допустимое значение Y

    for (const QJsonValue &pointValue : points) {
        QJsonObject pointObj = pointValue.toObject();
        double x = pointObj["x"].toDouble();
        double y = pointObj["y"].toDouble();

        // Фильтрация экстремальных значений
        if (!std::isinf(y) && !std::isnan(y) && std::abs(y) <= maxY) {
            data.append(QPointF(x, y));
        } else {
            qDebug() << "Пропущена точка с экстремальным Y:" << x << "," << y;
        }
    }

    updateChart(data);

    QString statusText = QString("Статус: построено %1 точек графика").arg(data.size());
    if (isLimited) {
        statusText += QString(" (ограничено до %1 точек, шаг изменён с %2 на %3)")
                          .arg(maxPoints).arg(originalStep).arg(adjustedStep);
    }
    ui->statusLabel->setText(statusText);
}



