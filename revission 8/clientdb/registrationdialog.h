#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

private slots:
    void on_registerButton_clicked();
    void onReadyRead();
    void displaySuccessMessage();

signals:
    void registrationSuccess();

private:
    Ui::RegistrationDialog *ui;
    QTcpSocket *socket;
};

#endif // REGISTRATIONDIALOG_H
