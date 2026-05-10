#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVector>
#include <QPointF>

QT_USE_NAMESPACE

    namespace Ui {
    class ClientGUI;
}


class ClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientGUI(QWidget *parent = nullptr);
    ~ClientGUI();

private slots:
    void on_connectButton_clicked();
    void on_calculateButton_clicked();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void on_connected();

    // Новые слоты для ползунков
    void on_aSlider_valueChanged(int value);
    void on_bSlider_valueChanged(int value);
    void on_cSlider_valueChanged(int value);

    // Новые слоты для спинбоксов
    void on_aSpinBox_valueChanged(double value);
    void on_bSpinBox_valueChanged(double value);
    void on_cSpinBox_valueChanged(double value);

    // Слот для автоматического обновления графика
    void autoUpdatePlot();

private:
    Ui::ClientGUI *ui;
    QTcpSocket *socket;

    struct PlotSegment {
        QString color;
        QString name;
        QVector<QPointF> points;
    };

    void setupPointsTable();
    void updatePointsTable(const QVector<QPointF>& points);
    QVector<PlotSegment> splitPointsBySegments(const QVector<QPointF>& points, double a, double b, double c);
    void processPlotData(const QJsonArray &points, bool isLimited, double originalStep, double adjustedStep, int maxPoints);
    void updateChart(const QVector<PlotSegment>& segments);
    void sendPlotRequest();  // Новый метод для отправки запроса

    QTimer *updateTimer;  // Таймер для дебаунса (задержка обновления)
    bool autoUpdateEnabled;  // Флаг автоматического обновления
};

#endif // CLIENTGUI_H