#include "projectdescriptiondialog.h"
#include "ui_projectdescriptiondialog.h"
#include <QMessageBox>

ProjectDescriptionDialog::ProjectDescriptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDescriptionDialog),
    clientGUI(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("Описание проекта — Графическое отображение ветвящейся функции");
    resize(1280, 720);

    // Настраиваем таблицу с общей информацией
    setupProjectInfoTable();

    // Настраиваем отображение математической функции
    setupMathFunctionDisplay();

    // Подключаем сигнал нажатия кнопки к слоту
    connect(ui->showGraphButton, &QPushButton::clicked, this, &ProjectDescriptionDialog::on_showGraphButton_clicked);
}

ProjectDescriptionDialog::~ProjectDescriptionDialog()
{
    delete ui;
    if (clientGUI) {
        delete clientGUI;
        clientGUI = nullptr;
    }
}

void ProjectDescriptionDialog::setupProjectInfoTable()
{
    QStringList parameters = {
        "Название проекта",
        "Тип проекта",
        "Назначение",
        "Дисциплина",
        "Группа"
    };

    QStringList values = {
        "Графическое отображение ветвящейся функции в рамках клиент‑серверного проекта",
        "Клиент‑серверное приложение",
        "Построение графиков кусочно‑заданных математических функций с возможностью динамического изменения параметров",
        "Технологии и методы программирования",
        "251-371"
    };

    ui->projectInfoTable->setRowCount(parameters.size());
    ui->projectInfoTable->setColumnCount(2);
    ui->projectInfoTable->setHorizontalHeaderLabels({"Параметр", "Значение"});

    for (int i = 0; i < parameters.size(); ++i) {
        ui->projectInfoTable->setItem(i, 0, new QTableWidgetItem(parameters[i]));
        ui->projectInfoTable->setItem(i, 1, new QTableWidgetItem(values[i]));
    }

    ui->projectInfoTable->resizeColumnsToContents();
    ui->projectInfoTable->horizontalHeader()->setStretchLastSection(true);
    ui->projectInfoTable->verticalHeader()->setVisible(false);
    ui->projectInfoTable->setShowGrid(false);
}

void ProjectDescriptionDialog::setupMathFunctionDisplay()
{
    QString functionText =
        "<div style='line-height: 1.6; font-family: Arial, sans-serif; color: white;'>"
        "<h2 style='font-size:18px; font-weight:600; margin-bottom:20px; border-bottom:2px solid #444; padding-bottom:10px;'>Кусочная функция f(x)</h2>"

        "<table style='background:#1A1A1A; border-radius:8px; width:100%; border-collapse:collapse; margin-bottom:25px; box-shadow:0 2px 8px rgba(0,0,0,0.3);'>"
        "  <thead>"
        "    <tr style='background:#2D2D2D;'>"
        "      <th style='padding:15px 20px; text-align:left; font-weight:bold; font-size:14px; border-bottom:2px solid #444;'>№</th>"
        "      <th style='padding:15px 20px; text-align:left; font-weight:bold; font-size:14px; border-bottom:2px solid #444;'>Выражение</th>"
        "      <th style='padding:15px 20px; text-align:left; font-weight:bold; font-size:14px; border-bottom:2px solid #444;'>Область определения</th>"
        "    </tr>"
        "  </thead>"
        "  <tbody>"
        "    <tr style='border-bottom:1px solid #2D2D2D;'>"
        "      <td style='padding:12px 20px; font-weight:bold;'>1</td>"
        "      <td style='padding:12px 20px; font-family:monospace; font-size:14px;'>∛x + a</td>"
        "      <td style='padding:12px 20px; font-size:14px;'>x &lt; 0</td>"
        "    </tr>"
        "    <tr style='border-bottom:1px solid #2D2D2D;'>"
        "      <td style='padding:12px 20px; font-weight:bold;'>2</td>"
        "      <td style='padding:12px 20px; font-family:monospace; font-size:14px;'>1/x − b</td>"
        "      <td style='padding:12px 20px; font-size:14px;'>0 &lt; x &lt; 1</td>"
        "    </tr>"
        "    <tr>"
        "      <td style='padding:12px 20px; font-weight:bold;'>3</td>"
        "      <td style='padding:12px 20px; font-family:monospace; font-size:14px;'>x² − c·x + 1</td>"
        "      <td style='padding:12px 20px; font-size:14px;'>x &ge; 1</td>"
        "    </tr>"
        "  </tbody>"
        "</table>"

        "<div style='font-weight:600; font-size:16px; margin-bottom:10px;'>Параметры:</div>"
        "<ul style='margin-left:25px; margin-top:5px; margin-bottom:20px;'>"
        "  <li>a — сдвиг кубического корня</li>"
        "  <li>b — сдвиг гиперболы</li>"
        "  <li>c — коэффициент квадратичной функции</li>"
        "</ul>"

        "<div style='font-weight:600; font-size:16px; margin-bottom:10px;'>Особенности:</div>"
        "<ul style='margin-left:25px; margin-top:5px;'>"
        "  <li>Разрыв в точке x = 0 (из‑за 1/x)</li>"
        "  <li>Плавный переход в точке x = 1</li>"
        "  <li>Возможность динамического изменения параметров a, b, c</li>"
        "  <li>Визуализация всех трёх участков разными цветами</li>"
        "</ul>"
        "</div>";

    ui->functionDisplay->setHtml(functionText);
}



void ProjectDescriptionDialog::on_showGraphButton_clicked()
{
    // Создаём новый экземпляр ClientGUI
    clientGUI = new ClientGUI();

    // Показываем окно с графиком
    clientGUI->show();

    // Закрываем текущее окно (ProjectDescriptionDialog)
    this->close();
}
