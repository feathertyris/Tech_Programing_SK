#ifndef RECOVERYDIALOG_H
#define RECOVERYDIALOG_H

#include <QDialog>

namespace Ui {
class RecoveryDialog;
}

class RecoveryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecoveryDialog(QWidget *parent = nullptr);
    QString getUsername() const;
    QString getEmail() const;
    QString getNewPassword() const;

    ~RecoveryDialog();

private slots:
    void onRecoveryClicked();
    void clearStatus();

private:
    void sendRecoveryRequest(const QString &username, const QString &email);
    bool isValidEmail(const QString &email) const;
    void showError(const QString &message);
    void showSuccess(const QString &message);

    Ui::RecoveryDialog *ui;
};

#endif // RECOVERYDIALOG_H
