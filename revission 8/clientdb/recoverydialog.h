#ifndef RECOVERYDIALOG_H
#define RECOVERYDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RecoveryDialog;
}

class RecoveryDialog : public QDialog
{
    Q_OBJECT  // обязательно для MOC

public:
    explicit RecoveryDialog(QWidget *parent = nullptr);
    ~RecoveryDialog();  // объявление деструктора

private slots:
    void on_sendRecoveryButton_clicked();
    void onReadyRead();

private:
    Ui::RecoveryDialog *ui;
    QTcpSocket *socket;
};

#endif // RECOVERYDIALOG_H
