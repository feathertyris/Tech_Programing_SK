#ifndef TOKENPASSWORDDIALOG_H
#define TOKENPASSWORDDIALOG_H


#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class TokenPasswordDialog;
}

class TokenPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TokenPasswordDialog(const QString &email, QWidget *parent = nullptr);
    ~TokenPasswordDialog();

signals:
    void passwordResetSuccess();

private slots:
    void on_resetPasswordButton_clicked();
    void onReadyRead();
    void displaySuccessMessage();
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::TokenPasswordDialog *ui;
    QTcpSocket *socket;
    QString recoveredEmail;
};

#endif // TOKENPASSWORDDIALOG_H
