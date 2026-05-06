#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonArray>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
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

private:
    Ui::ClientGUI *ui;
    QTcpSocket *socket;

    struct PlotSegment {
        QString color;
        QString name;
        QVector<QPointF> points;
    };

    void updateChart(const QVector<PlotSegment>& segments);
    void processPlotData(const QJsonArray &points,
                         bool isLimited = false,
                         double originalStep = 0.0,
                         double adjustedStep = 0.0,
                         int maxPoints = 0);

    // Разбить точки на сегменты по интервалам
    QVector<PlotSegment> splitPointsBySegments(const QVector<QPointF>& points,
                                               double a, double b, double c);
};

#endif // CLIENTGUI_H