/********************************************************************************
** Form generated from reading UI file 'registrationdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATIONDIALOG_H
#define UI_REGISTRATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegistrationDialog
{
public:
    QVBoxLayout *mainLayout;
    QFrame *cardFrame;
    QVBoxLayout *cardLayout;
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QPushButton *registerButton;
    QLabel *statusLabel;

    void setupUi(QDialog *RegistrationDialog)
    {
        if (RegistrationDialog->objectName().isEmpty())
            RegistrationDialog->setObjectName("RegistrationDialog");
        RegistrationDialog->resize(450, 520);
        RegistrationDialog->setStyleSheet(QString::fromUtf8("\n"
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
        mainLayout = new QVBoxLayout(RegistrationDialog);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setAlignment(Qt::AlignCenter);
        cardFrame = new QFrame(RegistrationDialog);
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
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: white;"));

        cardLayout->addWidget(titleLabel);

        usernameEdit = new QLineEdit(cardFrame);
        usernameEdit->setObjectName("usernameEdit");

        cardLayout->addWidget(usernameEdit);

        emailEdit = new QLineEdit(cardFrame);
        emailEdit->setObjectName("emailEdit");

        cardLayout->addWidget(emailEdit);

        passwordEdit = new QLineEdit(cardFrame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        cardLayout->addWidget(passwordEdit);

        registerButton = new QPushButton(cardFrame);
        registerButton->setObjectName("registerButton");

        cardLayout->addWidget(registerButton);

        statusLabel = new QLabel(cardFrame);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);
        statusLabel->setStyleSheet(QString::fromUtf8("color: \\#FF6B6B;"));

        cardLayout->addWidget(statusLabel);


        mainLayout->addWidget(cardFrame);


        retranslateUi(RegistrationDialog);

        QMetaObject::connectSlotsByName(RegistrationDialog);
    } // setupUi

    void retranslateUi(QDialog *RegistrationDialog)
    {
        RegistrationDialog->setWindowTitle(QCoreApplication::translate("RegistrationDialog", "Register", nullptr));
        titleLabel->setText(QCoreApplication::translate("RegistrationDialog", "Create Account", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("RegistrationDialog", "Username", nullptr));
        emailEdit->setPlaceholderText(QCoreApplication::translate("RegistrationDialog", "Email", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("RegistrationDialog", "Password", nullptr));
        registerButton->setText(QCoreApplication::translate("RegistrationDialog", "Register", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RegistrationDialog: public Ui_RegistrationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATIONDIALOG_H
