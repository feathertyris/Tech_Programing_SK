#ifndef PROJECTDESCRIPTIONDIALOG_H
#define PROJECTDESCRIPTIONDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "clientgui.h"

namespace Ui {
class ProjectDescriptionDialog;
}

class ProjectDescriptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectDescriptionDialog(QWidget *parent = nullptr);
    ~ProjectDescriptionDialog();

private slots:
    void on_showGraphButton_clicked();

private:
    Ui::ProjectDescriptionDialog *ui;
    ClientGUI *clientGUI;
    void setupProjectInfoTable();
    void setupMathFunctionDisplay();
};

#endif // PROJECTDESCRIPTIONDIALOG_H
