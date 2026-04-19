#ifndef RECOVERYDIALOG_H
#define RECOVERYDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RecoveryDialog;
}

class RecoveryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecoveryDialog(QWidget *parent = nullptr);
    ~RecoveryDialog();

private slots:
    void on_sendRecoveryButton_clicked();
    void onReadyRead();
    void displaySuccessMessage();

private:
    Ui::RecoveryDialog *ui;
    QTcpSocket *socket;
};

#endif // RECOVERYDIALOG_H
