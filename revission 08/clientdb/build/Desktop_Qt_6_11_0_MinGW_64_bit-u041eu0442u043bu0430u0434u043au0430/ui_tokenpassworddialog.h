/********************************************************************************
** Form generated from reading UI file 'tokenpassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOKENPASSWORDDIALOG_H
#define UI_TOKENPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TokenPasswordDialog
{
public:
    QVBoxLayout *mainLayout;
    QFrame *cardFrame;
    QVBoxLayout *cardLayout;
    QLabel *titleLabel;
    QLabel *emailLabel;
    QLineEdit *codeEdit;
    QLineEdit *newPasswordEdit;
    QPushButton *resetPasswordButton;
    QLabel *statusLabel;

    void setupUi(QDialog *TokenPasswordDialog)
    {
        if (TokenPasswordDialog->objectName().isEmpty())
            TokenPasswordDialog->setObjectName("TokenPasswordDialog");
        TokenPasswordDialog->resize(450, 520);
        TokenPasswordDialog->setStyleSheet(QString::fromUtf8("\n"
"    QDialog {\n"
"        background-color: #0F0F0F;\n"
"    }\n"
"    QLabel {\n"
"        color: white;\n"
"        font-size: 14px;\n"
"    }\n"
"    QLineEdit {\n"
"        background-color: #2A2A2A;\n"
"        border: 1px solid #404040;\n"
"        padding: 10px;\n"
"        border-radius: 8px;\n"
"        color: white;\n"
"    }\n"
"    QPushButton {\n"
"        background-color: #3A82F7;\n"
"        color: white;\n"
"        border: none;\n"
"        padding: 12px 24px;\n"
"        border-radius: 8px;\n"
"        font-weight: bold;\n"
"    }\n"
"    QPushButton:hover {\n"
"        background-color: #2C65BF;\n"
"    }\n"
"   "));
        mainLayout = new QVBoxLayout(TokenPasswordDialog);
        mainLayout->setObjectName("mainLayout");
        cardFrame = new QFrame(TokenPasswordDialog);
        cardFrame->setObjectName("cardFrame");
        cardFrame->setStyleSheet(QString::fromUtf8("\n"
"       QFrame {\n"
"           background-color: #1A1A1A;\n"
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

        emailLabel = new QLabel(cardFrame);
        emailLabel->setObjectName("emailLabel");
        emailLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        emailLabel->setWordWrap(true);

        cardLayout->addWidget(emailLabel);

        codeEdit = new QLineEdit(cardFrame);
        codeEdit->setObjectName("codeEdit");
        codeEdit->setMaxLength(6);

        cardLayout->addWidget(codeEdit);

        newPasswordEdit = new QLineEdit(cardFrame);
        newPasswordEdit->setObjectName("newPasswordEdit");
        newPasswordEdit->setEchoMode(QLineEdit::Password);

        cardLayout->addWidget(newPasswordEdit);

        resetPasswordButton = new QPushButton(cardFrame);
        resetPasswordButton->setObjectName("resetPasswordButton");

        cardLayout->addWidget(resetPasswordButton);

        statusLabel = new QLabel(cardFrame);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("color: #FF6B6B;"));
        statusLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        statusLabel->setWordWrap(true);

        cardLayout->addWidget(statusLabel);


        mainLayout->addWidget(cardFrame);


        retranslateUi(TokenPasswordDialog);

        QMetaObject::connectSlotsByName(TokenPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *TokenPasswordDialog)
    {
        TokenPasswordDialog->setWindowTitle(QCoreApplication::translate("TokenPasswordDialog", "Reset Password", nullptr));
        titleLabel->setText(QCoreApplication::translate("TokenPasswordDialog", "\320\222\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\270\320\265 \320\277\320\260\321\200\320\276\320\273\321\217", nullptr));
        emailLabel->setText(QCoreApplication::translate("TokenPasswordDialog", "Email: ", nullptr));
        codeEdit->setPlaceholderText(QCoreApplication::translate("TokenPasswordDialog", "6\342\200\221\320\267\320\275\320\260\321\207\320\275\321\213\320\271 \320\272\320\276\320\264 \320\270\320\267 \320\277\320\270\321\201\321\214\320\274\320\260", nullptr));
        codeEdit->setInputMask(QCoreApplication::translate("TokenPasswordDialog", "999999", nullptr));
        newPasswordEdit->setPlaceholderText(QCoreApplication::translate("TokenPasswordDialog", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\260\321\200\320\276\320\273\321\214 (\320\274\320\270\320\275\320\270\320\274\321\203\320\274 6 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262)", nullptr));
        resetPasswordButton->setText(QCoreApplication::translate("TokenPasswordDialog", "\320\241\320\261\321\200\320\276\321\201\320\270\321\202\321\214 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TokenPasswordDialog: public Ui_TokenPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOKENPASSWORDDIALOG_H
