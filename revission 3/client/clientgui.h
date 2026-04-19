#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtCharts>
#include <QByteArray>

QT_USE_NAMESPACE

    namespace Ui {
    class ClientGUI;
}

class ClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientGUI(QWidget *parent = nullptr);
    void setUsername(const QString &username);

    ~ClientGUI();


private slots:
    void on_connectButton_clicked();
    void on_calculateButton_clicked();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void handleAuthResponse(const QByteArray &response);
    void sendAuthRequest();


private:
    void clearChart();
    void resetChart();
    void setupChart();
    QByteArray createRequest();
    void parseResponse(const QByteArray &response);
    void updateAxisRanges(); // Новая функция для динамического масштабирования

    Ui::ClientGUI *ui;
    QTcpSocket *mSocket;
    QChart *mChart = nullptr;
    QLineSeries *mSeries1 = nullptr; // Для f(x)
    QLineSeries *mSeries2 = nullptr; // Для g(x)
    QLineSeries *mSeries3 = nullptr; // Для h(x)
    QChartView *mChartView = nullptr;
    QByteArray mBuffer; // Буфер для накопления входящих данных
    int mPointCount = 0; // Счётчик построенных точек
    QString m_username;
};

#endif // CLIENTGUI_H
