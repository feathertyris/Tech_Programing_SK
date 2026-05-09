#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtCharts>

QT_USE_NAMESPACE

    namespace Ui {
    class ClientGUI;
}

class ClientGUI : public QMainWindow
{
    Q_OBJECT  // Обязательно для слотов и сигналов

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
    void updateChart(const QVector<QPointF> &points);
    void processPlotData(const QJsonArray &points);
};

#endif // CLIENTGUI_H
