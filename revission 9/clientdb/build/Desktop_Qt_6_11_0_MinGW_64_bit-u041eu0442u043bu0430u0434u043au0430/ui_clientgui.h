/********************************************************************************
** Form generated from reading UI file 'clientgui.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTGUI_H
#define UI_CLIENTGUI_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientGUI
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxConnection;
    QHBoxLayout *horizontalLayoutConnection;
    QLabel *labelHost;
    QLineEdit *hostEdit;
    QLabel *labelPort;
    QLineEdit *portEdit;
    QPushButton *connectButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBoxFunction1;
    QVBoxLayout *verticalLayout1;
    QHBoxLayout *aLayout;
    QLabel *labelASlider;
    QSlider *aSlider;
    QLabel *aValueLabel;
    QDoubleSpinBox *a1SpinBox;
    QHBoxLayout *bLayout;
    QLabel *labelBSlider;
    QSlider *bSlider;
    QLabel *bValueLabel;
    QDoubleSpinBox *b1SpinBox;
    QHBoxLayout *cLayout;
    QLabel *labelCSlider;
    QSlider *cSlider;
    QLabel *cValueLabel;
    QDoubleSpinBox *c1SpinBox;
    QGroupBox *groupBoxXSettings;
    QHBoxLayout *xSettingsLayout;
    QLabel *labelXStart;
    QDoubleSpinBox *xStartSpinBox;
    QLabel *labelXEnd;
    QDoubleSpinBox *xEndSpinBox;
    QLabel *labelXStep;
    QDoubleSpinBox *xStepSpinBox;
    QSpacerItem *horizontalSpacer2;
    QHBoxLayout *buttonsLayout;
    QPushButton *calculateTripleButton;
    QPushButton *calculateButton;
    QSpacerItem *horizontalSpacer3;
    QLabel *statusLabel;
    QGroupBox *groupBoxChart;
    QVBoxLayout *verticalLayoutChart;
    QChartView *chartView;
    QGroupBox *groupBoxPointsTable;
    QVBoxLayout *verticalLayoutTable;
    QTableWidget *pointsTable;

    void setupUi(QMainWindow *ClientGUI)
    {
        if (ClientGUI->objectName().isEmpty())
            ClientGUI->setObjectName("ClientGUI");
        ClientGUI->resize(1400, 900);
        ClientGUI->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow {\n"
"        background-color: #0F0F0F;\n"
"    }\n"
"    QLabel {\n"
"        color: white;\n"
"        font-size: 11px;\n"
"    }\n"
"    QLineEdit, QDoubleSpinBox {\n"
"        background-color: #2A2A2A;\n"
"        border: 1px solid #404040;\n"
"        padding: 3px 5px;\n"
"        border-radius: 4px;\n"
"        color: white;\n"
"        font-size: 10px;\n"
"    }\n"
"    QSlider::groove:horizontal {\n"
"        height: 4px;\n"
"        background: #404040;\n"
"        border-radius: 2px;\n"
"    }\n"
"    QSlider::handle:horizontal {\n"
"        background: #3A82F7;\n"
"        width: 12px;\n"
"        height: 12px;\n"
"        margin: -4px 0;\n"
"        border-radius: 6px;\n"
"    }\n"
"    QSlider::handle:horizontal:hover {\n"
"        background: #2C65BF;\n"
"    }\n"
"    QSlider::sub-page:horizontal {\n"
"        background: #3A82F7;\n"
"        border-radius: 2px;\n"
"    }\n"
"    QGroupBox {\n"
"        color: white;\n"
"        font-weight: bold;\n"
"        border: 1px s"
                        "olid #404040;\n"
"        border-radius: 6px;\n"
"        margin-top: 12px;\n"
"    }\n"
"    QGroupBox::title {\n"
"        subcontrol-origin: margin;\n"
"        left: 8px;\n"
"        padding: 0 5px 0 5px;\n"
"    }\n"
"    QPushButton {\n"
"        background-color: #3A82F7;\n"
"        color: white;\n"
"        border: none;\n"
"        padding: 6px 12px;\n"
"        border-radius: 5px;\n"
"        font-weight: bold;\n"
"        font-size: 11px;\n"
"    }\n"
"    QPushButton:hover {\n"
"        background-color: #2C65BF;\n"
"    }\n"
"    QPushButton#calculateTripleButton {\n"
"        background-color: #28A745;\n"
"    }\n"
"    QPushButton#calculateTripleButton:hover {\n"
"        background-color: #218838;\n"
"    }\n"
"    QLabel#statusLabel {\n"
"        color: #FF6B6B;\n"
"        background-color: #1A1A1A;\n"
"        padding: 5px;\n"
"        border-radius: 4px;\n"
"        border: 1px solid #404040;\n"
"        font-size: 10px;\n"
"    }\n"
"    QGroupBox#groupBoxFunction1 {\n"
"        border-co"
                        "lor: #FF4444;\n"
"    }\n"
"    QGroupBox#groupBoxFunction1::title {\n"
"        color: #FF4444;\n"
"    }\n"
"    QGroupBox#groupBoxFunction2 {\n"
"        border-color: #4444FF;\n"
"    }\n"
"    QGroupBox#groupBoxFunction2::title {\n"
"        color: #4444FF;\n"
"    }\n"
"    QGroupBox#groupBoxFunction3 {\n"
"        border-color: #44FF44;\n"
"    }\n"
"    QGroupBox#groupBoxFunction3::title {\n"
"        color: #44FF44;\n"
"    }\n"
"   "));
        centralWidget = new QWidget(ClientGUI);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName("verticalLayout");
        groupBoxConnection = new QGroupBox(centralWidget);
        groupBoxConnection->setObjectName("groupBoxConnection");
        horizontalLayoutConnection = new QHBoxLayout(groupBoxConnection);
        horizontalLayoutConnection->setSpacing(5);
        horizontalLayoutConnection->setObjectName("horizontalLayoutConnection");
        labelHost = new QLabel(groupBoxConnection);
        labelHost->setObjectName("labelHost");

        horizontalLayoutConnection->addWidget(labelHost);

        hostEdit = new QLineEdit(groupBoxConnection);
        hostEdit->setObjectName("hostEdit");
        hostEdit->setMaximumSize(QSize(120, 16777215));

        horizontalLayoutConnection->addWidget(hostEdit);

        labelPort = new QLabel(groupBoxConnection);
        labelPort->setObjectName("labelPort");

        horizontalLayoutConnection->addWidget(labelPort);

        portEdit = new QLineEdit(groupBoxConnection);
        portEdit->setObjectName("portEdit");
        portEdit->setMaximumSize(QSize(60, 16777215));

        horizontalLayoutConnection->addWidget(portEdit);

        connectButton = new QPushButton(groupBoxConnection);
        connectButton->setObjectName("connectButton");
        connectButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayoutConnection->addWidget(connectButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutConnection->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBoxConnection);

        groupBoxFunction1 = new QGroupBox(centralWidget);
        groupBoxFunction1->setObjectName("groupBoxFunction1");
        verticalLayout1 = new QVBoxLayout(groupBoxFunction1);
        verticalLayout1->setSpacing(3);
        verticalLayout1->setObjectName("verticalLayout1");
        aLayout = new QHBoxLayout();
        aLayout->setObjectName("aLayout");
        labelASlider = new QLabel(groupBoxFunction1);
        labelASlider->setObjectName("labelASlider");
        labelASlider->setMinimumWidth(20);

        aLayout->addWidget(labelASlider);

        aSlider = new QSlider(groupBoxFunction1);
        aSlider->setObjectName("aSlider");
        aSlider->setOrientation(Qt::Horizontal);
        aSlider->setMinimum(-100);
        aSlider->setMaximum(100);
        aSlider->setValue(100);
        aSlider->setTickInterval(20);
        aSlider->setTickPosition(QSlider::TicksBelow);

        aLayout->addWidget(aSlider);

        aValueLabel = new QLabel(groupBoxFunction1);
        aValueLabel->setObjectName("aValueLabel");
        aValueLabel->setMinimumWidth(35);
        aValueLabel->setAlignment(Qt::AlignRight);

        aLayout->addWidget(aValueLabel);

        a1SpinBox = new QDoubleSpinBox(groupBoxFunction1);
        a1SpinBox->setObjectName("a1SpinBox");
        a1SpinBox->setDecimals(2);
        a1SpinBox->setMinimum(-100.000000000000000);
        a1SpinBox->setMaximum(100.000000000000000);
        a1SpinBox->setValue(1.000000000000000);
        a1SpinBox->setMinimumWidth(60);

        aLayout->addWidget(a1SpinBox);


        verticalLayout1->addLayout(aLayout);

        bLayout = new QHBoxLayout();
        bLayout->setObjectName("bLayout");
        labelBSlider = new QLabel(groupBoxFunction1);
        labelBSlider->setObjectName("labelBSlider");
        labelBSlider->setMinimumWidth(20);

        bLayout->addWidget(labelBSlider);

        bSlider = new QSlider(groupBoxFunction1);
        bSlider->setObjectName("bSlider");
        bSlider->setOrientation(Qt::Horizontal);
        bSlider->setMinimum(-100);
        bSlider->setMaximum(100);
        bSlider->setValue(0);
        bSlider->setTickInterval(20);
        bSlider->setTickPosition(QSlider::TicksBelow);

        bLayout->addWidget(bSlider);

        bValueLabel = new QLabel(groupBoxFunction1);
        bValueLabel->setObjectName("bValueLabel");
        bValueLabel->setMinimumWidth(35);
        bValueLabel->setAlignment(Qt::AlignRight);

        bLayout->addWidget(bValueLabel);

        b1SpinBox = new QDoubleSpinBox(groupBoxFunction1);
        b1SpinBox->setObjectName("b1SpinBox");
        b1SpinBox->setDecimals(2);
        b1SpinBox->setMinimum(-100.000000000000000);
        b1SpinBox->setMaximum(100.000000000000000);
        b1SpinBox->setValue(0.000000000000000);
        b1SpinBox->setMinimumWidth(60);

        bLayout->addWidget(b1SpinBox);


        verticalLayout1->addLayout(bLayout);

        cLayout = new QHBoxLayout();
        cLayout->setObjectName("cLayout");
        labelCSlider = new QLabel(groupBoxFunction1);
        labelCSlider->setObjectName("labelCSlider");
        labelCSlider->setMinimumWidth(20);

        cLayout->addWidget(labelCSlider);

        cSlider = new QSlider(groupBoxFunction1);
        cSlider->setObjectName("cSlider");
        cSlider->setOrientation(Qt::Horizontal);
        cSlider->setMinimum(-100);
        cSlider->setMaximum(100);
        cSlider->setValue(0);
        cSlider->setTickInterval(20);
        cSlider->setTickPosition(QSlider::TicksBelow);

        cLayout->addWidget(cSlider);

        cValueLabel = new QLabel(groupBoxFunction1);
        cValueLabel->setObjectName("cValueLabel");
        cValueLabel->setMinimumWidth(35);
        cValueLabel->setAlignment(Qt::AlignRight);

        cLayout->addWidget(cValueLabel);

        c1SpinBox = new QDoubleSpinBox(groupBoxFunction1);
        c1SpinBox->setObjectName("c1SpinBox");
        c1SpinBox->setDecimals(2);
        c1SpinBox->setMinimum(-100.000000000000000);
        c1SpinBox->setMaximum(100.000000000000000);
        c1SpinBox->setValue(0.000000000000000);
        c1SpinBox->setMinimumWidth(60);

        cLayout->addWidget(c1SpinBox);


        verticalLayout1->addLayout(cLayout);


        verticalLayout->addWidget(groupBoxFunction1);

        groupBoxXSettings = new QGroupBox(centralWidget);
        groupBoxXSettings->setObjectName("groupBoxXSettings");
        xSettingsLayout = new QHBoxLayout(groupBoxXSettings);
        xSettingsLayout->setSpacing(5);
        xSettingsLayout->setObjectName("xSettingsLayout");
        labelXStart = new QLabel(groupBoxXSettings);
        labelXStart->setObjectName("labelXStart");

        xSettingsLayout->addWidget(labelXStart);

        xStartSpinBox = new QDoubleSpinBox(groupBoxXSettings);
        xStartSpinBox->setObjectName("xStartSpinBox");
        xStartSpinBox->setDecimals(1);
        xStartSpinBox->setMinimum(-100.000000000000000);
        xStartSpinBox->setMaximum(100.000000000000000);
        xStartSpinBox->setValue(-10.000000000000000);
        xStartSpinBox->setMinimumWidth(60);

        xSettingsLayout->addWidget(xStartSpinBox);

        labelXEnd = new QLabel(groupBoxXSettings);
        labelXEnd->setObjectName("labelXEnd");

        xSettingsLayout->addWidget(labelXEnd);

        xEndSpinBox = new QDoubleSpinBox(groupBoxXSettings);
        xEndSpinBox->setObjectName("xEndSpinBox");
        xEndSpinBox->setDecimals(1);
        xEndSpinBox->setMinimum(-100.000000000000000);
        xEndSpinBox->setMaximum(100.000000000000000);
        xEndSpinBox->setValue(10.000000000000000);
        xEndSpinBox->setMinimumWidth(60);

        xSettingsLayout->addWidget(xEndSpinBox);

        labelXStep = new QLabel(groupBoxXSettings);
        labelXStep->setObjectName("labelXStep");

        xSettingsLayout->addWidget(labelXStep);

        xStepSpinBox = new QDoubleSpinBox(groupBoxXSettings);
        xStepSpinBox->setObjectName("xStepSpinBox");
        xStepSpinBox->setDecimals(3);
        xStepSpinBox->setMinimum(0.010000000000000);
        xStepSpinBox->setMaximum(10.000000000000000);
        xStepSpinBox->setValue(0.050000000000000);
        xStepSpinBox->setMinimumWidth(60);

        xSettingsLayout->addWidget(xStepSpinBox);

        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        xSettingsLayout->addItem(horizontalSpacer2);


        verticalLayout->addWidget(groupBoxXSettings);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(10);
        buttonsLayout->setObjectName("buttonsLayout");
        calculateTripleButton = new QPushButton(centralWidget);
        calculateTripleButton->setObjectName("calculateTripleButton");
        calculateTripleButton->setVisible(false);

        buttonsLayout->addWidget(calculateTripleButton);

        calculateButton = new QPushButton(centralWidget);
        calculateButton->setObjectName("calculateButton");

        buttonsLayout->addWidget(calculateButton);

        horizontalSpacer3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(horizontalSpacer3);


        verticalLayout->addLayout(buttonsLayout);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setMaximumHeight(25);

        verticalLayout->addWidget(statusLabel);

        groupBoxChart = new QGroupBox(centralWidget);
        groupBoxChart->setObjectName("groupBoxChart");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBoxChart->sizePolicy().hasHeightForWidth());
        groupBoxChart->setSizePolicy(sizePolicy);
        verticalLayoutChart = new QVBoxLayout(groupBoxChart);
        verticalLayoutChart->setContentsMargins(2, 2, 2, 2);
        verticalLayoutChart->setObjectName("verticalLayoutChart");
        chartView = new QChartView(groupBoxChart);
        chartView->setObjectName("chartView");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
        chartView->setSizePolicy(sizePolicy1);
        chartView->setMinimumHeight(350);
        chartView->setRenderHints(QPainter::Antialiasing);

        verticalLayoutChart->addWidget(chartView);


        verticalLayout->addWidget(groupBoxChart);

        groupBoxPointsTable = new QGroupBox(centralWidget);
        groupBoxPointsTable->setObjectName("groupBoxPointsTable");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBoxPointsTable->sizePolicy().hasHeightForWidth());
        groupBoxPointsTable->setSizePolicy(sizePolicy2);
        verticalLayoutTable = new QVBoxLayout(groupBoxPointsTable);
        verticalLayoutTable->setContentsMargins(2, 2, 2, 2);
        verticalLayoutTable->setObjectName("verticalLayoutTable");
        pointsTable = new QTableWidget(groupBoxPointsTable);
        pointsTable->setObjectName("pointsTable");
        pointsTable->setMinimumHeight(100);
        pointsTable->setMaximumHeight(120);
        pointsTable->setAlternatingRowColors(true);
        pointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
        pointsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        pointsTable->setShowGrid(true);
        pointsTable->setStyleSheet(QString::fromUtf8("\n"
"           QTableWidget {\n"
"               background-color: #2b2b2b;\n"
"               alternate-background-color: #3a3a3a;\n"
"               gridline-color: #555;\n"
"               color: #ffffff;\n"
"               font-size: 9px;\n"
"           }\n"
"           QHeaderView::section {\n"
"               background-color: #1e1e1e;\n"
"               padding: 2px;\n"
"               border: 1px solid #555;\n"
"               font-weight: bold;\n"
"               color: #ffffff;\n"
"           }\n"
"           QTableWidget::item {\n"
"               padding: 2px;\n"
"           }\n"
"           QTableWidget::item:selected {\n"
"               background-color: #4a90e2;\n"
"           }\n"
"          "));

        verticalLayoutTable->addWidget(pointsTable);


        verticalLayout->addWidget(groupBoxPointsTable);

        verticalLayout->setStretch(5, 1);
        ClientGUI->setCentralWidget(centralWidget);

        retranslateUi(ClientGUI);

        QMetaObject::connectSlotsByName(ClientGUI);
    } // setupUi

    void retranslateUi(QMainWindow *ClientGUI)
    {
        ClientGUI->setWindowTitle(QCoreApplication::translate("ClientGUI", "\320\232\320\273\320\270\320\265\320\275\321\202 \320\277\320\276\321\201\321\202\321\200\320\276\320\265\320\275\320\270\321\217 \320\263\321\200\320\260\321\204\320\270\320\272\320\276\320\262 \342\200\224 \320\232\321\203\321\201\320\276\321\207\320\275\320\276-\320\267\320\260\320\264\320\260\320\275\320\275\320\260\321\217 \321\204\321\203\320\275\320\272\321\206\320\270\321\217", nullptr));
        groupBoxConnection->setTitle(QCoreApplication::translate("ClientGUI", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265", nullptr));
        labelHost->setText(QCoreApplication::translate("ClientGUI", "\320\245\320\276\321\201\321\202:", nullptr));
        hostEdit->setText(QCoreApplication::translate("ClientGUI", "localhost", nullptr));
        labelPort->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\321\200\321\202:", nullptr));
        portEdit->setText(QCoreApplication::translate("ClientGUI", "33333", nullptr));
        connectButton->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", nullptr));
        groupBoxFunction1->setTitle(QCoreApplication::translate("ClientGUI", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \321\204\321\203\320\275\320\272\321\206\320\270\320\270 (\320\277\320\276\320\273\320\267\321\203\320\275\320\272\320\270 -100..100)", nullptr));
        labelASlider->setText(QCoreApplication::translate("ClientGUI", "a =", nullptr));
        aValueLabel->setText(QCoreApplication::translate("ClientGUI", "1.00", nullptr));
        labelBSlider->setText(QCoreApplication::translate("ClientGUI", "b =", nullptr));
        bValueLabel->setText(QCoreApplication::translate("ClientGUI", "0.00", nullptr));
        labelCSlider->setText(QCoreApplication::translate("ClientGUI", "c =", nullptr));
        cValueLabel->setText(QCoreApplication::translate("ClientGUI", "0.00", nullptr));
        groupBoxXSettings->setTitle(QCoreApplication::translate("ClientGUI", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 X", nullptr));
        labelXStart->setText(QCoreApplication::translate("ClientGUI", "X min:", nullptr));
        labelXEnd->setText(QCoreApplication::translate("ClientGUI", "X max:", nullptr));
        labelXStep->setText(QCoreApplication::translate("ClientGUI", "\320\250\320\260\320\263:", nullptr));
        calculateTripleButton->setText(QCoreApplication::translate("ClientGUI", "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214 \321\202\321\200\320\270 \320\263\321\200\320\260\321\204\320\270\320\272\320\260", nullptr));
        calculateButton->setText(QCoreApplication::translate("ClientGUI", "\360\237\224\204 \320\220\320\262\321\202\320\276\320\276\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\320\265: \320\222\320\232\320\233", nullptr));
        calculateButton->setStyleSheet(QCoreApplication::translate("ClientGUI", "background-color: #28A745; font-size: 11px; padding: 6px 12px;", nullptr));
        statusLabel->setText(QCoreApplication::translate("ClientGUI", "\320\241\321\202\320\260\321\202\321\203\321\201: \320\275\320\265 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\276", nullptr));
        groupBoxChart->setTitle(QCoreApplication::translate("ClientGUI", "\320\223\321\200\320\260\321\204\320\270\320\272 \321\204\321\203\320\275\320\272\321\206\320\270\320\270", nullptr));
        groupBoxPointsTable->setTitle(QCoreApplication::translate("ClientGUI", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260 \320\272\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientGUI: public Ui_ClientGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTGUI_H
