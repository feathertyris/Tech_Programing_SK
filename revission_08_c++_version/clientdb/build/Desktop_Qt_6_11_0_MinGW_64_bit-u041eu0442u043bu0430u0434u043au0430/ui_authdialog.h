/********************************************************************************
** Form generated from reading UI file 'authdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHDIALOG_H
#define UI_AUTHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AuthDialog
{
public:
    QVBoxLayout *mainLayout;
    QFrame *cardFrame;
    QVBoxLayout *cardLayout;
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *recoveryButton;
    QLabel *statusLabel;

    void setupUi(QDialog *AuthDialog)
    {
        if (AuthDialog->objectName().isEmpty())
            AuthDialog->setObjectName("AuthDialog");
        AuthDialog->resize(450, 520);
        AuthDialog->setStyleSheet(QString::fromUtf8("\n"
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
"    QPushButton#recoveryButton {\n"
"        background-color: transparent;\n"
"        color: \\#3A82F7;\n"
"    }\n"
"    QPushButton#recoveryButton:hover {\n"
"        text-decoration: underline;\n"
"    }\n"
"   "));
        mainLayout = new QVBoxLayout(AuthDialog);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setAlignment(Qt::AlignCenter);
        cardFrame = new QFrame(AuthDialog);
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

        passwordEdit = new QLineEdit(cardFrame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        cardLayout->addWidget(passwordEdit);

        loginButton = new QPushButton(cardFrame);
        loginButton->setObjectName("loginButton");

        cardLayout->addWidget(loginButton);

        registerButton = new QPushButton(cardFrame);
        registerButton->setObjectName("registerButton");

        cardLayout->addWidget(registerButton);

        recoveryButton = new QPushButton(cardFrame);
        recoveryButton->setObjectName("recoveryButton");

        cardLayout->addWidget(recoveryButton);

        statusLabel = new QLabel(cardFrame);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);
        statusLabel->setStyleSheet(QString::fromUtf8("color: \\#FF6B6B;"));

        cardLayout->addWidget(statusLabel);


        mainLayout->addWidget(cardFrame);


        retranslateUi(AuthDialog);

        QMetaObject::connectSlotsByName(AuthDialog);
    } // setupUi

    void retranslateUi(QDialog *AuthDialog)
    {
        AuthDialog->setWindowTitle(QCoreApplication::translate("AuthDialog", "Auth", nullptr));
        titleLabel->setText(QCoreApplication::translate("AuthDialog", "Welcome Back", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("AuthDialog", "Username", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("AuthDialog", "Password", nullptr));
        loginButton->setText(QCoreApplication::translate("AuthDialog", "Login", nullptr));
        registerButton->setText(QCoreApplication::translate("AuthDialog", "No account? Register", nullptr));
        recoveryButton->setText(QCoreApplication::translate("AuthDialog", "Forgot password?", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AuthDialog: public Ui_AuthDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHDIALOG_H
