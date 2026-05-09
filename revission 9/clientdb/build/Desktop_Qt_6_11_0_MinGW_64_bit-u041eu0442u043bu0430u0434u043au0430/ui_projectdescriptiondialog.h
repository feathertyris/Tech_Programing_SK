/********************************************************************************
** Form generated from reading UI file 'projectdescriptiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTDESCRIPTIONDIALOG_H
#define UI_PROJECTDESCRIPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProjectDescriptionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QTableWidget *projectInfoTable;
    QGroupBox *functionGroupBox;
    QVBoxLayout *functionLayout;
    QTextEdit *functionDisplay;
    QPushButton *showGraphButton;

    void setupUi(QDialog *ProjectDescriptionDialog)
    {
        if (ProjectDescriptionDialog->objectName().isEmpty())
            ProjectDescriptionDialog->setObjectName("ProjectDescriptionDialog");
        ProjectDescriptionDialog->resize(1280, 720);
        ProjectDescriptionDialog->setStyleSheet(QString::fromUtf8("\n"
"QDialog {\n"
"background-color: #0F0F0F;\n"
"}\n"
"QLabel {\n"
"color: white;\n"
"font-size: 14px;\n"
"}\n"
"QTextEdit {\n"
"background-color: #2A2A2A;\n"
"border: 1px solid #404040;\n"
"padding: 10px;\n"
"border-radius: 8px;\n"
"color: white;\n"
"font-family: 'Consolas', 'Monaco', monospace;\n"
"font-size: 13px;\n"
"}\n"
"QTableWidget {\n"
"background-color: #2A2A2A;\n"
"border: 1px solid #404040;\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QTableWidget::item {\n"
"padding: 8px;\n"
"border: 1px solid #404040;\n"
"}\n"
"QTableWidget::item:selected {\n"
"background-color: #3A82F7;\n"
"}\n"
"QHeaderView::section {\n"
"background-color: #1A1A1A;\n"
"color: white;\n"
"padding: 8px;\n"
"border: none;\n"
"}\n"
"QGroupBox {\n"
"color: white;\n"
"font-weight: bold;\n"
"border: 1px solid #404040;\n"
"border-radius: 8px;\n"
"margin-top: 16px;\n"
"}\n"
"QGroupBox::title {\n"
"subcontrol-origin: margin;\n"
"left: 10px;\n"
"padding: 0 5px 0 5px;\n"
"}\n"
"QPushButton {\n"
"background-color: #3A82F7;\n"
"color:"
                        " white;\n"
"border: none;\n"
"padding: 12px 24px;\n"
"border-radius: 8px;\n"
"font-weight: bold;\n"
"font-size: 14px;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: #2C65BF;\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(ProjectDescriptionDialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setContentsMargins(25, 25, 25, 25);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(ProjectDescriptionDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"color: #3A82F7;\n"
"font-size: 18px;\n"
"font-weight: bold;\n"
"margin-bottom: 10px;\n"
"}\n"
""));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(ProjectDescriptionDialog);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"color: #B0B0B0;\n"
"font-size: 12px;\n"
"margin-bottom: 25px;\n"
"}\n"
""));
        subtitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(subtitleLabel);

        projectInfoTable = new QTableWidget(ProjectDescriptionDialog);
        if (projectInfoTable->columnCount() < 2)
            projectInfoTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        projectInfoTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        projectInfoTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        projectInfoTable->setObjectName("projectInfoTable");
        projectInfoTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        projectInfoTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        projectInfoTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(projectInfoTable);

        functionGroupBox = new QGroupBox(ProjectDescriptionDialog);
        functionGroupBox->setObjectName("functionGroupBox");
        functionLayout = new QVBoxLayout(functionGroupBox);
        functionLayout->setObjectName("functionLayout");
        functionDisplay = new QTextEdit(functionGroupBox);
        functionDisplay->setObjectName("functionDisplay");
        functionDisplay->setReadOnly(true);
        functionDisplay->setHtml(QString::fromUtf8("\n"
"<div style='line-height: 1.6;'>\n"
"<span style='font-size:16px; font-weight:600; color:#3A82F7; display:block; margin-bottom:15px;'>f(x) =</span>\n"
"<div style='background:#1A1A1A; border-radius:8px; padding:15px; margin-bottom:20px; text-align:center;'>\n"
"<div style='font-family:monospace; font-size:14px; color:white;'>\n"
"\342\216\247 \342\210\233x + a, \320\277\321\200\320\270 x < 0<br>\n"
"\342\216\252 1/x \342\210\222 b, \320\277\321\200\320\270 0 < x < 1<br>\n"
"\342\216\251 x\302\262 \342\210\222 c\302\267x + 1, \320\277\321\200\320\270 x \342\211\245 1\n"
"</div>\n"
"</div>\n"
"<div style='color:#FF6B6B; font-weight:600; margin-bottom:10px;'>\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213:</div>\n"
"<div style='margin-left:20px; margin-bottom:15px;'>\n"
"- <span style='color:#3A82F7;'>a</span> \342\200\224 \321\201\320\264\320\262\320\270\320\263 \320\272\321\203\320\261\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\272\320\276\321\200\320\275\321"
                        "\217<br>\n"
"- <span style='color:#3A82F7;'>b</span> \342\200\224 \321\201\320\264\320\262\320\270\320\263 \320\263\320\270\320\277\320\265\321\200\320\261\320\276\320\273\321\213<br>\n"
"- <span style='color:#3A82F7;'>c</span> \342\200\224 \320\272\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202 \320\272\320\262\320\260\320\264\321\200\320\260\321\202\320\270\321\207\320\275\320\276\320\271 \321\204\321\203\320\275\320\272\321\206\320\270\320\270\n"
"</div>\n"
"<div style='color:#FF6B6B; font-weight:600; margin-bottom:10px;'>\320\236\321\201\320\276\320\261\320\265\320\275\320\275\320\276\321\201\321\202\320\270:</div>\n"
"<div style='margin-left:20px;'>\n"
"- \320\240\320\260\320\267\321\200\321\213\320\262 \320\262 \321\202\320\276\321\207\320\272\320\265 x = 0 (\320\270\320\267\342\200\221\320\267\320\260 1/x)<br>\n"
"- \320\237\320\273\320\260\320\262\320\275\321\213\320\271 \320\277\320\265\321\200\320\265\321\205\320\276\320\264 \320\262 \321\202\320\276\321\207\320\272\320"
                        "\265 x = 1<br>\n"
"- \320\222\320\276\320\267\320\274\320\276\320\266\320\275\320\276\321\201\321\202\321\214 \320\264\320\270\320\275\320\260\320\274\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262 a, b, c<br>\n"
"- \320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \320\262\321\201\320\265\321\205 \321\202\321\200\321\221\321\205 \321\203\321\207\320\260\321\201\321\202\320\272\320\276\320\262 \321\200\320\260\320\267\320\275\321\213\320\274\320\270 \321\206\320\262\320\265\321\202\320\260\320\274\320\270\n"
"</div>\n"
"</div>\n"
""));

        functionLayout->addWidget(functionDisplay);


        verticalLayout->addWidget(functionGroupBox);

        showGraphButton = new QPushButton(ProjectDescriptionDialog);
        showGraphButton->setObjectName("showGraphButton");
        showGraphButton->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"background-color: #3A82F7;\n"
"color: white;\n"
"border: none;\n"
"padding: 12px 30px;\n"
"border-radius: 8px;\n"
"font-weight: bold;\n"
"font-size: 14px;\n"
"margin-top: 20px;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: #2C65BF;\n"
"}\n"
"QPushButton:pressed {\n"
"background-color: #2553A0;\n"
"}\n"
""));
        showGraphButton->setProperty("fixedHeight", QVariant(45));

        verticalLayout->addWidget(showGraphButton);


        retranslateUi(ProjectDescriptionDialog);

        QMetaObject::connectSlotsByName(ProjectDescriptionDialog);
    } // setupUi

    void retranslateUi(QDialog *ProjectDescriptionDialog)
    {
        ProjectDescriptionDialog->setWindowTitle(QCoreApplication::translate("ProjectDescriptionDialog", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\277\321\200\320\276\320\265\320\272\321\202\320\260 \342\200\224 \320\223\321\200\320\260\321\204\320\270\321\207\320\265\321\201\320\272\320\276\320\265 \320\276\321\202\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265 \320\262\320\265\321\202\320\262\321\217\321\211\320\265\320\271\321\201\321\217 \321\204\321\203\320\275\320\272\321\206\320\270\320\270", nullptr));
        titleLabel->setText(QCoreApplication::translate("ProjectDescriptionDialog", "\320\223\321\200\320\260\321\204\320\270\321\207\320\265\321\201\320\272\320\276\320\265 \320\276\321\202\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265 \320\262\320\265\321\202\320\262\321\217\321\211\320\265\320\271\321\201\321\217 \321\204\321\203\320\275\320\272\321\206\320\270\320\270", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("ProjectDescriptionDialog", "\320\232\320\273\320\270\320\265\320\275\321\202\342\200\221\321\201\320\265\321\200\320\262\320\265\321\200\320\275\320\276\320\265 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\265 \320\264\320\273\321\217 \320\277\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\270\321\217 \320\263\321\200\320\260\321\204\320\270\320\272\320\276\320\262 \320\272\321\203\321\201\320\276\321\207\320\275\320\276\342\200\221\320\267\320\260\320\264\320\260\320\275\320\275\321\213\321\205 \321\204\321\203\320\275\320\272\321\206\320\270\320\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem = projectInfoTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ProjectDescriptionDialog", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = projectInfoTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ProjectDescriptionDialog", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", nullptr));
        functionGroupBox->setTitle(QCoreApplication::translate("ProjectDescriptionDialog", "\320\234\320\260\321\202\320\265\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\204\321\203\320\275\320\272\321\206\320\270\321\217", nullptr));
        showGraphButton->setText(QCoreApplication::translate("ProjectDescriptionDialog", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\263\321\200\320\260\321\204\320\270\320\272 \321\204\321\203\320\275\320\272\321\206\320\270\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectDescriptionDialog: public Ui_ProjectDescriptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTDESCRIPTIONDIALOG_H
