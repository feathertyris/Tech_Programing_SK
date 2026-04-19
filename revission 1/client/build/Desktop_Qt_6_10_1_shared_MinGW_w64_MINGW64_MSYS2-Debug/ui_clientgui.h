/********************************************************************************
** Form generated from reading UI file 'clientgui.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTGUI_H
#define UI_CLIENTGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientGUI
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxConnection;
    QHBoxLayout *horizontalLayoutConnection;
    QLabel *labelHost;
    QLineEdit *hostEdit;
    QLabel *labelPort;
    QLineEdit *portEdit;
    QPushButton *connectButton;
    QGroupBox *groupBoxFunction;
    QFormLayout *formLayoutFunction;
    QLabel *labelA;
    QDoubleSpinBox *aSpinBox;
    QLabel *labelB;
    QDoubleSpinBox *bSpinBox;
    QLabel *labelC;
    QDoubleSpinBox *cSpinBox;
    QLabel *labelXStart;
    QDoubleSpinBox *xStartSpinBox;
    QLabel *labelXEnd;
    QDoubleSpinBox *xEndSpinBox;
    QLabel *labelXStep;
    QDoubleSpinBox *xStepSpinBox;
    QPushButton *calculateButton;
    QGroupBox *groupBoxChart;
    QVBoxLayout *verticalLayoutChart;
    QWidget *chartContainer;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ClientGUI)
    {
        if (ClientGUI->objectName().isEmpty())
            ClientGUI->setObjectName("ClientGUI");
        ClientGUI->resize(800, 600);
        actionExit = new QAction(ClientGUI);
        actionExit->setObjectName("actionExit");
        centralWidget = new QWidget(ClientGUI);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBoxConnection = new QGroupBox(centralWidget);
        groupBoxConnection->setObjectName("groupBoxConnection");
        horizontalLayoutConnection = new QHBoxLayout(groupBoxConnection);
        horizontalLayoutConnection->setObjectName("horizontalLayoutConnection");
        labelHost = new QLabel(groupBoxConnection);
        labelHost->setObjectName("labelHost");

        horizontalLayoutConnection->addWidget(labelHost);

        hostEdit = new QLineEdit(groupBoxConnection);
        hostEdit->setObjectName("hostEdit");

        horizontalLayoutConnection->addWidget(hostEdit);

        labelPort = new QLabel(groupBoxConnection);
        labelPort->setObjectName("labelPort");

        horizontalLayoutConnection->addWidget(labelPort);

        portEdit = new QLineEdit(groupBoxConnection);
        portEdit->setObjectName("portEdit");

        horizontalLayoutConnection->addWidget(portEdit);

        connectButton = new QPushButton(groupBoxConnection);
        connectButton->setObjectName("connectButton");

        horizontalLayoutConnection->addWidget(connectButton);


        verticalLayout->addWidget(groupBoxConnection);

        groupBoxFunction = new QGroupBox(centralWidget);
        groupBoxFunction->setObjectName("groupBoxFunction");
        formLayoutFunction = new QFormLayout(groupBoxFunction);
        formLayoutFunction->setObjectName("formLayoutFunction");
        labelA = new QLabel(groupBoxFunction);
        labelA->setObjectName("labelA");

        formLayoutFunction->setWidget(0, QFormLayout::ItemRole::LabelRole, labelA);

        aSpinBox = new QDoubleSpinBox(groupBoxFunction);
        aSpinBox->setObjectName("aSpinBox");
        aSpinBox->setDecimals(2);
        aSpinBox->setMinimum(-100.000000000000000);
        aSpinBox->setMaximum(100.000000000000000);
        aSpinBox->setValue(1.000000000000000);

        formLayoutFunction->setWidget(0, QFormLayout::ItemRole::FieldRole, aSpinBox);

        labelB = new QLabel(groupBoxFunction);
        labelB->setObjectName("labelB");

        formLayoutFunction->setWidget(1, QFormLayout::ItemRole::LabelRole, labelB);

        bSpinBox = new QDoubleSpinBox(groupBoxFunction);
        bSpinBox->setObjectName("bSpinBox");
        bSpinBox->setDecimals(2);
        bSpinBox->setMinimum(-100.000000000000000);
        bSpinBox->setMaximum(100.000000000000000);
        bSpinBox->setValue(0.000000000000000);

        formLayoutFunction->setWidget(1, QFormLayout::ItemRole::FieldRole, bSpinBox);

        labelC = new QLabel(groupBoxFunction);
        labelC->setObjectName("labelC");

        formLayoutFunction->setWidget(2, QFormLayout::ItemRole::LabelRole, labelC);

        cSpinBox = new QDoubleSpinBox(groupBoxFunction);
        cSpinBox->setObjectName("cSpinBox");
        cSpinBox->setDecimals(2);
        cSpinBox->setMinimum(-100.000000000000000);
        cSpinBox->setMaximum(100.000000000000000);
        cSpinBox->setValue(0.000000000000000);

        formLayoutFunction->setWidget(2, QFormLayout::ItemRole::FieldRole, cSpinBox);

        labelXStart = new QLabel(groupBoxFunction);
        labelXStart->setObjectName("labelXStart");

        formLayoutFunction->setWidget(3, QFormLayout::ItemRole::LabelRole, labelXStart);

        xStartSpinBox = new QDoubleSpinBox(groupBoxFunction);
        xStartSpinBox->setObjectName("xStartSpinBox");
        xStartSpinBox->setDecimals(1);
        xStartSpinBox->setMinimum(-100.000000000000000);
        xStartSpinBox->setMaximum(0.000000000000000);
        xStartSpinBox->setValue(-10.000000000000000);

        formLayoutFunction->setWidget(3, QFormLayout::ItemRole::FieldRole, xStartSpinBox);

        labelXEnd = new QLabel(groupBoxFunction);
        labelXEnd->setObjectName("labelXEnd");

        formLayoutFunction->setWidget(4, QFormLayout::ItemRole::LabelRole, labelXEnd);

        xEndSpinBox = new QDoubleSpinBox(groupBoxFunction);
        xEndSpinBox->setObjectName("xEndSpinBox");
        xEndSpinBox->setDecimals(1);
        xEndSpinBox->setMinimum(0.000000000000000);
        xEndSpinBox->setMaximum(100.000000000000000);
        xEndSpinBox->setValue(10.000000000000000);

        formLayoutFunction->setWidget(4, QFormLayout::ItemRole::FieldRole, xEndSpinBox);

        labelXStep = new QLabel(groupBoxFunction);
        labelXStep->setObjectName("labelXStep");

        formLayoutFunction->setWidget(5, QFormLayout::ItemRole::LabelRole, labelXStep);

        xStepSpinBox = new QDoubleSpinBox(groupBoxFunction);
        xStepSpinBox->setObjectName("xStepSpinBox");
        xStepSpinBox->setDecimals(2);
        xStepSpinBox->setMinimum(0.010000000000000);
        xStepSpinBox->setMaximum(10.000000000000000);
        xStepSpinBox->setValue(0.100000000000000);

        formLayoutFunction->setWidget(5, QFormLayout::ItemRole::FieldRole, xStepSpinBox);


        verticalLayout->addWidget(groupBoxFunction);

        calculateButton = new QPushButton(centralWidget);
        calculateButton->setObjectName("calculateButton");

        verticalLayout->addWidget(calculateButton);

        groupBoxChart = new QGroupBox(centralWidget);
        groupBoxChart->setObjectName("groupBoxChart");
        verticalLayoutChart = new QVBoxLayout(groupBoxChart);
        verticalLayoutChart->setObjectName("verticalLayoutChart");
        chartContainer = new QWidget(groupBoxChart);
        chartContainer->setObjectName("chartContainer");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chartContainer->sizePolicy().hasHeightForWidth());
        chartContainer->setSizePolicy(sizePolicy);

        verticalLayoutChart->addWidget(chartContainer);


        verticalLayout->addWidget(groupBoxChart);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; padding: 5px; border: 1px solid #ccc;"));

        verticalLayout->addWidget(statusLabel);

        ClientGUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ClientGUI);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        ClientGUI->setMenuBar(menuBar);
        statusBar = new QStatusBar(ClientGUI);
        statusBar->setObjectName("statusBar");
        ClientGUI->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(ClientGUI);
        QObject::connect(actionExit, &QAction::triggered, ClientGUI, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(ClientGUI);
    } // setupUi

    void retranslateUi(QMainWindow *ClientGUI)
    {
        ClientGUI->setWindowTitle(QCoreApplication::translate("ClientGUI", "\320\232\320\273\320\270\320\265\320\275\321\202 \320\277\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\270\321\217 \320\263\321\200\320\260\321\204\320\270\320\272\320\276\320\262", nullptr));
        actionExit->setText(QCoreApplication::translate("ClientGUI", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        groupBoxConnection->setTitle(QCoreApplication::translate("ClientGUI", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        labelHost->setText(QCoreApplication::translate("ClientGUI", "\320\245\320\276\321\201\321\202:", nullptr));
        hostEdit->setText(QCoreApplication::translate("ClientGUI", "localhost", nullptr));
        labelPort->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\321\200\321\202:", nullptr));
        portEdit->setText(QCoreApplication::translate("ClientGUI", "33333", nullptr));
        connectButton->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", nullptr));
        groupBoxFunction->setTitle(QCoreApplication::translate("ClientGUI", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \321\204\321\203\320\275\320\272\321\206\320\270\320\270", nullptr));
        labelA->setText(QCoreApplication::translate("ClientGUI", "a:", nullptr));
        labelB->setText(QCoreApplication::translate("ClientGUI", "b:", nullptr));
        labelC->setText(QCoreApplication::translate("ClientGUI", "c:", nullptr));
        labelXStart->setText(QCoreApplication::translate("ClientGUI", "X \320\275\320\260\321\207\320\260\320\273\320\276:", nullptr));
        labelXEnd->setText(QCoreApplication::translate("ClientGUI", "X \320\272\320\276\320\275\320\265\321\206:", nullptr));
        labelXStep->setText(QCoreApplication::translate("ClientGUI", "\320\250\320\260\320\263 X:", nullptr));
        calculateButton->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214 \320\263\321\200\320\260\321\204\320\270\320\272\320\270", nullptr));
        groupBoxChart->setTitle(QCoreApplication::translate("ClientGUI", "\320\223\321\200\320\260\321\204\320\270\320\272 \321\204\321\203\320\275\320\272\321\206\320\270\320\271", nullptr));
        statusLabel->setText(QCoreApplication::translate("ClientGUI", "\320\223\320\276\321\202\320\276\320\262 \320\272 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\216", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ClientGUI", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientGUI: public Ui_ClientGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTGUI_H
