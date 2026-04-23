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
    resize(800, 600);

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
        "<div style='line-height: 1.6;'>"
        "<span style='font-size:16px; font-weight:600; color:#3A82F7; display:block; margin-bottom:15px;'>f(x) =</span>"
        "<div style='background:#1A1A1A; border-radius:8px; padding:15px; margin-bottom:20px; text-align:center;'>"
        "<div style='font-family:monospace; font-size:14px; color:white;'>"
        "⎧ ∛x + a,      при x < 0<br>"
        "⎪ 1/x − b,     при 0 < x < 1<br>"
        "⎩ x² − c·x + 1, при x ≥ 1"
        "</div>"
        "</div>"
        "<div style='color:#FF6B6B; font-weight:600; margin-bottom:10px;'>Параметры:</div>"
        "<div style='margin-left:20px; margin-bottom:15px;'>"
        "- <span style='color:#3A82F7;'>a</span> — сдвиг кубического корня<br>"
        "- <span style='color:#3A82F7;'>b</span> — сдвиг гиперболы<br>"
        "- <span style='color:#3A82F7;'>c</span> — коэффициент квадратичной функции"
        "</div>"
        "<div style='color:#FF6B6B; font-weight:600; margin-bottom:10px;'>Особенности:</div>"
        "<div style='margin-left:20px;'>"
        "- Разрыв в точке x = 0 (из‑за 1/x)<br>"
        "- Плавный переход в точке x = 1<br>"
        "- Возможность динамического изменения параметров a, b, c<br>"
        "- Визуализация всех трёх участков разными цветами"
        "</div>"
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
