/********************************************************************************
** Form generated from reading UI file 'recoverydialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECOVERYDIALOG_H
#define UI_RECOVERYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RecoveryDialog
{
public:
    QVBoxLayout *mainLayout;
    QFrame *cardFrame;
    QVBoxLayout *cardLayout;
    QLabel *titleLabel;
    QLabel *instructionLabel;
    QLineEdit *emailEdit;
    QPushButton *sendRecoveryButton;
    QLabel *statusLabel;

    void setupUi(QDialog *RecoveryDialog)
    {
        if (RecoveryDialog->objectName().isEmpty())
            RecoveryDialog->setObjectName("RecoveryDialog");
        RecoveryDialog->resize(450, 520);
        RecoveryDialog->setStyleSheet(QString::fromUtf8("\n"
"    QDialog {\n"
"        background-color: \\#0F0F0F;\n"
"    }\n"
"    QLabel {\n"
"        color: white;\n"
"        font-size: 14px;\n"
"    }\n"
"    QLineEdit {\n"
"        background-color: \\#2A2A2A;\n"
"        border: 1px solid \\#404040;\n"
"        padding: 10px;\n"
"        border-radius: 8px;\n"
"        color: white;\n"
"    }\n"
"    QPushButton {\n"
"        background-color: \\#3A82F7;\n"
"        color: white;\n"
"        border: none;\n"
"        padding: 12px 24px;\n"
"        border-radius: 8px;\n"
"        font-weight: bold;\n"
"    }\n"
"    QPushButton:hover {\n"
"        background-color: \\#2C65BF;\n"
"    }\n"
"   "));
        mainLayout = new QVBoxLayout(RecoveryDialog);
        mainLayout->setObjectName("mainLayout");
        cardFrame = new QFrame(RecoveryDialog);
        cardFrame->setObjectName("cardFrame");
        cardFrame->setStyleSheet(QString::fromUtf8("\n"
"       QFrame {\n"
"           background-color: \\#1A1A1A;\n"
"           border-radius: 16px;\n"
"           padding: 30px;\n"
"       }\n"
"      "));
        cardLayout = new QVBoxLayout(cardFrame);
        cardLayout->setObjectName("cardLayout");
        titleLabel = new QLabel(cardFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: white;"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        cardLayout->addWidget(titleLabel);

        instructionLabel = new QLabel(cardFrame);
        instructionLabel->setObjectName("instructionLabel");
        instructionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        instructionLabel->setWordWrap(true);

        cardLayout->addWidget(instructionLabel);

        emailEdit = new QLineEdit(cardFrame);
        emailEdit->setObjectName("emailEdit");

        cardLayout->addWidget(emailEdit);

        sendRecoveryButton = new QPushButton(cardFrame);
        sendRecoveryButton->setObjectName("sendRecoveryButton");

        cardLayout->addWidget(sendRecoveryButton);

        statusLabel = new QLabel(cardFrame);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("color: \\#FF6B6B;"));
        statusLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        statusLabel->setWordWrap(true);

        cardLayout->addWidget(statusLabel);


        mainLayout->addWidget(cardFrame);


        retranslateUi(RecoveryDialog);

        QMetaObject::connectSlotsByName(RecoveryDialog);
    } // setupUi

    void retranslateUi(QDialog *RecoveryDialog)
    {
        RecoveryDialog->setWindowTitle(QCoreApplication::translate("RecoveryDialog", "Password Recovery", nullptr));
        titleLabel->setText(QCoreApplication::translate("RecoveryDialog", "Password Recovery", nullptr));
        instructionLabel->setText(QCoreApplication::translate("RecoveryDialog", "Enter your email to receive password reset instructions:", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("RecoveryDialog", "Email", nullptr));
        sendRecoveryButton->setText(QCoreApplication::translate("RecoveryDialog", "Send Recovery Email", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RecoveryDialog: public Ui_RecoveryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECOVERYDIALOG_H
