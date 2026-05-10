from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QLabel, QSlider,
    QHBoxLayout, QListWidget, QPushButton,
    QDoubleSpinBox, QTableWidget, QTableWidgetItem,
    QHeaderView, QGroupBox
)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap, QFont

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from resourses import resource_path
from api_client import get_function_data

import numpy as np
import os


class GraphWindow(QWidget):
    def __init__(self, on_logout=None):
        super().__init__()

        self.on_logout = on_logout
        
        # Параметры функции (начальные значения)
        self.a = 1.0
        self.b = 0.0
        self.c = 0.0
        
        # Параметры диапазона X (ограничение ±50)
        self.x_start = -10.0
        self.x_end = 10.0
        
        # Масштаб графика (по умолчанию 1.0)
        self.zoom = 1.0
        
        # Автообновление
        self.auto_update_enabled = True
        self.update_timer = QTimer()
        self.update_timer.setSingleShot(True)
        self.update_timer.setInterval(50)  # 50 мс для быстрого отклика
        self.update_timer.timeout.connect(self.plot)
        
        self.init_ui()
        
    def init_ui(self):
        main_layout = QVBoxLayout()
        
        # ===== TOP BAR =====
        top_bar = QHBoxLayout()
        
        # ===== ФОРМУЛА (ИЗ КАРТИНКИ) =====
        self.formula_label = QLabel()
        
        # Проверяем существование файла и загружаем
        formula_path = resource_path("formula.png")
        print(f"Попытка загрузить формулу из: {formula_path}")
        
        if os.path.exists(formula_path):
            pixmap = QPixmap(formula_path)
            if not pixmap.isNull():
                # Масштабируем картинку с сохранением пропорций
                scaled_pixmap = pixmap.scaled(350, 100, Qt.KeepAspectRatio, Qt.SmoothTransformation)
                self.formula_label.setPixmap(scaled_pixmap)
                print("Формула успешно загружена")
            else:
                print("Ошибка: не удалось загрузить formula.png (QPixmap isNull)")
                self.formula_label.setText("f(x) = { ∛x + a, x<0; 1/x + b, 0<x<1; x² - c·x + 1, x≥1 }")
                self.formula_label.setStyleSheet("color: white; font-size: 12px; font-weight: bold;")
        else:
            print(f"Ошибка: formula.png не найден по пути {formula_path}")
            self.formula_label.setText("f(x) = { ∛x + a, x<0; 1/x + b, 0<x<1; x² - c·x + 1, x≥1 }")
            self.formula_label.setStyleSheet("color: white; font-size: 12px; font-weight: bold;")
        
        self.formula_label.setFixedHeight(100)
        top_bar.addWidget(self.formula_label)
        
        # ===== ОПИСАНИЕ ФУНКЦИИ СПРАВА ОТ ФОТО =====
        description_widget = QWidget()
        description_layout = QVBoxLayout()
        description_layout.setContentsMargins(10, 0, 0, 0)
        
        function_desc = QLabel("Кусочно-заданная функция:")
        function_desc.setStyleSheet("color: #3A82F7; font-size: 13px; font-weight: bold;")
        
        piece1 = QLabel("• f₁(x) = ∛x + a, при x < 0")
        piece1.setStyleSheet("color: #3A82F7; font-size: 11px;")
        
        piece2 = QLabel("• f₂(x) = 1/x + b, при 0 < x < 1")
        piece2.setStyleSheet("color: #4CAF50; font-size: 11px;")
        
        piece3 = QLabel("• f₃(x) = x² - c·x + 1, при x ≥ 1")
        piece3.setStyleSheet("color: #FF5252; font-size: 11px;")
        
        description_layout.addWidget(function_desc)
        description_layout.addWidget(piece1)
        description_layout.addWidget(piece2)
        description_layout.addWidget(piece3)
        description_layout.addStretch()
        
        description_widget.setLayout(description_layout)
        top_bar.addWidget(description_widget)
        top_bar.addStretch()
        
        # ===== КНОПКА ВЫХОДА =====
        self.logout_btn = QPushButton("Выход")
        self.logout_btn.setFixedSize(80, 40)
        self.logout_btn.clicked.connect(self.handle_logout)
        self.logout_btn.setStyleSheet("""
            QPushButton {
                background-color: #dc3545;
                color: white;
                border-radius: 5px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #c82333;
            }
        """)
        
        top_bar.addWidget(self.logout_btn)
        
        # ===== КОНТЕНТ =====
        content_layout = QHBoxLayout()
        
        # ===== ЛЕВАЯ ПАНЕЛЬ =====
        left_panel = QVBoxLayout()
        left_container = QWidget()
        left_container.setFixedWidth(320)
        
        # Группа параметров функции
        params_group = QGroupBox("Параметры функции")
        params_layout = QVBoxLayout()
        
        # Параметр A
        a_layout = QHBoxLayout()
        a_layout.addWidget(QLabel("a:"))
        
        self.a_label = QLabel("1.00")
        self.a_label.setMinimumWidth(50)
        self.a_label.setAlignment(Qt.AlignRight)
        
        self.a_slider = QSlider(Qt.Horizontal)
        self.a_slider.setMinimum(-10)
        self.a_slider.setMaximum(10)
        self.a_slider.setValue(10)  # 1.0
        self.a_slider.setTickInterval(2)
        self.a_slider.setTickPosition(QSlider.TicksBelow)
        self.a_slider.valueChanged.connect(self.on_a_change)
        
        self.a_spinbox = QDoubleSpinBox()
        self.a_spinbox.setRange(-10.0, 10.0)
        self.a_spinbox.setSingleStep(0.1)
        self.a_spinbox.setValue(1.0)
        self.a_spinbox.setDecimals(2)
        self.a_spinbox.valueChanged.connect(self.on_a_spinbox_change)
        
        a_layout.addWidget(self.a_label)
        a_layout.addWidget(self.a_slider)
        a_layout.addWidget(self.a_spinbox)
        
        # Параметр B
        b_layout = QHBoxLayout()
        b_layout.addWidget(QLabel("b:"))
        
        self.b_label = QLabel("0.00")
        self.b_label.setMinimumWidth(50)
        self.b_label.setAlignment(Qt.AlignRight)
        
        self.b_slider = QSlider(Qt.Horizontal)
        self.b_slider.setMinimum(-10)
        self.b_slider.setMaximum(10)
        self.b_slider.setValue(0)
        self.b_slider.setTickInterval(2)
        self.b_slider.setTickPosition(QSlider.TicksBelow)
        self.b_slider.valueChanged.connect(self.on_b_change)
        
        self.b_spinbox = QDoubleSpinBox()
        self.b_spinbox.setRange(-10.0, 10.0)
        self.b_spinbox.setSingleStep(0.1)
        self.b_spinbox.setValue(0.0)
        self.b_spinbox.setDecimals(2)
        self.b_spinbox.valueChanged.connect(self.on_b_spinbox_change)
        
        b_layout.addWidget(self.b_label)
        b_layout.addWidget(self.b_slider)
        b_layout.addWidget(self.b_spinbox)
        
        # Параметр C
        c_layout = QHBoxLayout()
        c_layout.addWidget(QLabel("c:"))
        
        self.c_label = QLabel("0.00")
        self.c_label.setMinimumWidth(50)
        self.c_label.setAlignment(Qt.AlignRight)
        
        self.c_slider = QSlider(Qt.Horizontal)
        self.c_slider.setMinimum(-10)
        self.c_slider.setMaximum(10)
        self.c_slider.setValue(0)
        self.c_slider.setTickInterval(2)
        self.c_slider.setTickPosition(QSlider.TicksBelow)
        self.c_slider.valueChanged.connect(self.on_c_change)
        
        self.c_spinbox = QDoubleSpinBox()
        self.c_spinbox.setRange(-10.0, 10.0)
        self.c_spinbox.setSingleStep(0.1)
        self.c_spinbox.setValue(0.0)
        self.c_spinbox.setDecimals(2)
        self.c_spinbox.valueChanged.connect(self.on_c_spinbox_change)
        
        c_layout.addWidget(self.c_label)
        c_layout.addWidget(self.c_slider)
        c_layout.addWidget(self.c_spinbox)
        
        params_layout.addLayout(a_layout)
        params_layout.addLayout(b_layout)
        params_layout.addLayout(c_layout)
        params_group.setLayout(params_layout)
        
        # Группа диапазона X (ограничение ±50)
        x_range_group = QGroupBox("Диапазон X (от -50 до 50)")
        x_range_layout = QVBoxLayout()
        
        # Начало диапазона X
        x_start_layout = QVBoxLayout()
        x_start_label = QLabel("Начало (X min):")
        x_start_label.setStyleSheet("font-weight: bold; margin-top: 5px;")
        x_start_layout.addWidget(x_start_label)
        
        x_start_controls = QHBoxLayout()
        
        self.x_start_label_val = QLabel("-10.00")
        self.x_start_label_val.setMinimumWidth(50)
        self.x_start_label_val.setAlignment(Qt.AlignRight)
        self.x_start_label_val.setStyleSheet("font-weight: bold;")
        
        self.x_start_slider = QSlider(Qt.Horizontal)
        self.x_start_slider.setMinimum(-50)  # Ограничение -50
        self.x_start_slider.setMaximum(50)   # Ограничение 50
        self.x_start_slider.setValue(-10)
        self.x_start_slider.setTickInterval(10)
        self.x_start_slider.setTickPosition(QSlider.TicksBelow)
        self.x_start_slider.valueChanged.connect(self.on_x_start_change)
        
        self.x_start_spinbox = QDoubleSpinBox()
        self.x_start_spinbox.setRange(-50.0, 50.0)  # Ограничение -50 до 50
        self.x_start_spinbox.setSingleStep(0.5)
        self.x_start_spinbox.setValue(-10.0)
        self.x_start_spinbox.setDecimals(2)
        self.x_start_spinbox.valueChanged.connect(self.on_x_start_spinbox_change)
        
        x_start_controls.addWidget(self.x_start_label_val)
        x_start_controls.addWidget(self.x_start_slider)
        x_start_controls.addWidget(self.x_start_spinbox)
        
        x_start_layout.addLayout(x_start_controls)
        
        # Конец диапазона X
        x_end_layout = QVBoxLayout()
        x_end_label = QLabel("Конец (X max):")
        x_end_label.setStyleSheet("font-weight: bold; margin-top: 5px;")
        x_end_layout.addWidget(x_end_label)
        
        x_end_controls = QHBoxLayout()
        
        self.x_end_label_val = QLabel("10.00")
        self.x_end_label_val.setMinimumWidth(50)
        self.x_end_label_val.setAlignment(Qt.AlignRight)
        self.x_end_label_val.setStyleSheet("font-weight: bold;")
        
        self.x_end_slider = QSlider(Qt.Horizontal)
        self.x_end_slider.setMinimum(-50)  # Ограничение -50
        self.x_end_slider.setMaximum(50)   # Ограничение 50
        self.x_end_slider.setValue(10)
        self.x_end_slider.setTickInterval(10)
        self.x_end_slider.setTickPosition(QSlider.TicksBelow)
        self.x_end_slider.valueChanged.connect(self.on_x_end_change)
        
        self.x_end_spinbox = QDoubleSpinBox()
        self.x_end_spinbox.setRange(-50.0, 50.0)  # Ограничение -50 до 50
        self.x_end_spinbox.setSingleStep(0.5)
        self.x_end_spinbox.setValue(10.0)
        self.x_end_spinbox.setDecimals(2)
        self.x_end_spinbox.valueChanged.connect(self.on_x_end_spinbox_change)
        
        x_end_controls.addWidget(self.x_end_label_val)
        x_end_controls.addWidget(self.x_end_slider)
        x_end_controls.addWidget(self.x_end_spinbox)
        
        x_end_layout.addLayout(x_end_controls)
        
        x_range_layout.addLayout(x_start_layout)
        x_range_layout.addLayout(x_end_layout)
        x_range_group.setLayout(x_range_layout)
        
        # Группа масштаба графика
        zoom_group = QGroupBox("Масштаб графика")
        zoom_layout = QVBoxLayout()
        
        zoom_controls = QHBoxLayout()
        zoom_controls.addWidget(QLabel("Масштаб:"))
        
        self.zoom_label = QLabel("1.00")
        self.zoom_label.setMinimumWidth(50)
        self.zoom_label.setAlignment(Qt.AlignRight)
        self.zoom_label.setStyleSheet("font-weight: bold;")
        
        self.zoom_slider = QSlider(Qt.Horizontal)
        self.zoom_slider.setMinimum(10)   # 0.1x
        self.zoom_slider.setMaximum(200)  # 2.0x
        self.zoom_slider.setValue(100)    # 1.0x
        self.zoom_slider.setTickInterval(20)
        self.zoom_slider.setTickPosition(QSlider.TicksBelow)
        self.zoom_slider.valueChanged.connect(self.on_zoom_change)
        
        self.zoom_spinbox = QDoubleSpinBox()
        self.zoom_spinbox.setRange(0.1, 2.0)
        self.zoom_spinbox.setSingleStep(0.05)
        self.zoom_spinbox.setValue(1.0)
        self.zoom_spinbox.setDecimals(2)
        self.zoom_spinbox.valueChanged.connect(self.on_zoom_spinbox_change)
        
        zoom_controls.addWidget(self.zoom_label)
        zoom_controls.addWidget(self.zoom_slider)
        zoom_controls.addWidget(self.zoom_spinbox)
        
        zoom_layout.addLayout(zoom_controls)
        zoom_group.setLayout(zoom_layout)
        
        # Кнопка автообновления
        self.auto_update_btn = QPushButton("🔄 Автообновление: ВКЛ")
        self.auto_update_btn.clicked.connect(self.toggle_auto_update)
        self.auto_update_btn.setStyleSheet("""
            QPushButton {
                background-color: #28A745;
                color: white;
                font-size: 12px;
                padding: 8px 12px;
                border-radius: 5px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #218838;
            }
        """)
        
        # ===== ТАБЛИЦА ТОЧЕК (ограничена 20 значениями) =====
        points_group = QGroupBox("Точки функции (максимум 20 значений)")
        points_layout = QVBoxLayout()
        
        # Заголовки таблицы
        self.points_table = QTableWidget()
        self.points_table.setColumnCount(2)
        self.points_table.setHorizontalHeaderLabels(["X", "Y"])
        self.points_table.setAlternatingRowColors(True)
        self.points_table.setSelectionBehavior(QTableWidget.SelectRows)
        self.points_table.setSelectionMode(QTableWidget.SingleSelection)
        self.points_table.setEditTriggers(QTableWidget.NoEditTriggers)
        
        # Настройка внешнего вида таблицы
        self.points_table.setStyleSheet("""
            QTableWidget {
                background-color: #2b2b2b;
                alternate-background-color: #3a3a3a;
                gridline-color: #555;
                color: #ffffff;
            }
            QHeaderView::section {
                background-color: #1e1e1e;
                padding: 5px;
                border: 1px solid #555;
                font-weight: bold;
                color: #ffffff;
            }
            QTableWidget::item:selected {
                background-color: #4a90e2;
            }
        """)
        
        # Растягиваем столбцы
        header = self.points_table.horizontalHeader()
        header.setSectionResizeMode(QHeaderView.Stretch)
        
        points_layout.addWidget(self.points_table)
        points_group.setLayout(points_layout)
        
        # Собираем левую панель
        left_panel.addWidget(params_group)
        left_panel.addWidget(x_range_group)
        left_panel.addWidget(zoom_group)
        left_panel.addWidget(self.auto_update_btn)
        left_panel.addWidget(points_group)
        
        left_container.setLayout(left_panel)
        
        # ===== ГРАФИК =====
        self.figure = Figure(figsize=(8, 6), dpi=100)
        self.canvas = FigureCanvas(self.figure)
        
        content_layout.addWidget(left_container)
        content_layout.addWidget(self.canvas)
        
        self.canvas.setSizePolicy(
            self.canvas.sizePolicy().Expanding,
            self.canvas.sizePolicy().Expanding
        )
        
        # ===== СБОРКА =====
        main_layout.addLayout(top_bar)
        main_layout.addLayout(content_layout)
        
        self.setLayout(main_layout)
        
        # Начальная отрисовка
        self.plot()
    
    def handle_logout(self):
        if self.on_logout:
            self.on_logout()
    
    # ===== ОБРАБОТЧИКИ ПАРАМЕТРОВ =====
    def on_a_change(self, value):
        self.a = value
        self.a_label.setText(f"{self.a:.2f}")
        
        # Синхронизация со спинбоксом
        self.a_spinbox.blockSignals(True)
        self.a_spinbox.setValue(self.a)
        self.a_spinbox.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_a_spinbox_change(self, value):
        self.a = value
        self.a_label.setText(f"{self.a:.2f}")
        
        # Синхронизация со слайдером
        self.a_slider.blockSignals(True)
        self.a_slider.setValue(int(self.a))
        self.a_slider.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_b_change(self, value):
        self.b = value
        self.b_label.setText(f"{self.b:.2f}")
        
        self.b_spinbox.blockSignals(True)
        self.b_spinbox.setValue(self.b)
        self.b_spinbox.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_b_spinbox_change(self, value):
        self.b = value
        self.b_label.setText(f"{self.b:.2f}")
        
        self.b_slider.blockSignals(True)
        self.b_slider.setValue(int(self.b))
        self.b_slider.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_c_change(self, value):
        self.c = value
        self.c_label.setText(f"{self.c:.2f}")
        
        self.c_spinbox.blockSignals(True)
        self.c_spinbox.setValue(self.c)
        self.c_spinbox.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_c_spinbox_change(self, value):
        self.c = value
        self.c_label.setText(f"{self.c:.2f}")
        
        self.c_slider.blockSignals(True)
        self.c_slider.setValue(int(self.c))
        self.c_slider.blockSignals(False)
        
        self.auto_update_plot()
    
    # ===== ОБРАБОТЧИКИ ДИАПАЗОНА X =====
    def on_x_start_change(self, value):
        self.x_start = value
        self.x_start_label_val.setText(f"{self.x_start:.2f}")
        
        # Синхронизация со спинбоксом
        self.x_start_spinbox.blockSignals(True)
        self.x_start_spinbox.setValue(self.x_start)
        self.x_start_spinbox.blockSignals(False)
        
        # Проверка, чтобы начало не было больше конца
        if self.x_start >= self.x_end:
            self.x_end = min(self.x_start + 1, 50)
            self.on_x_end_change(int(self.x_end))
        
        self.auto_update_plot()
    
    def on_x_start_spinbox_change(self, value):
        self.x_start = value
        self.x_start_label_val.setText(f"{self.x_start:.2f}")
        
        # Синхронизация со слайдером
        self.x_start_slider.blockSignals(True)
        self.x_start_slider.setValue(int(self.x_start))
        self.x_start_slider.blockSignals(False)
        
        # Проверка, чтобы начало не было больше конца
        if self.x_start >= self.x_end:
            self.x_end = min(self.x_start + 1, 50)
            self.on_x_end_spinbox_change(self.x_end)
        
        self.auto_update_plot()
    
    def on_x_end_change(self, value):
        self.x_end = value
        self.x_end_label_val.setText(f"{self.x_end:.2f}")
        
        # Синхронизация со спинбоксом
        self.x_end_spinbox.blockSignals(True)
        self.x_end_spinbox.setValue(self.x_end)
        self.x_end_spinbox.blockSignals(False)
        
        # Проверка, чтобы конец не был меньше начала
        if self.x_end <= self.x_start:
            self.x_start = max(self.x_end - 1, -50)
            self.on_x_start_change(int(self.x_start))
        
        self.auto_update_plot()
    
    def on_x_end_spinbox_change(self, value):
        self.x_end = value
        self.x_end_label_val.setText(f"{self.x_end:.2f}")
        
        # Синхронизация со слайдером
        self.x_end_slider.blockSignals(True)
        self.x_end_slider.setValue(int(self.x_end))
        self.x_end_slider.blockSignals(False)
        
        # Проверка, чтобы конец не был меньше начала
        if self.x_end <= self.x_start:
            self.x_start = max(self.x_end - 1, -50)
            self.on_x_start_spinbox_change(self.x_start)
        
        self.auto_update_plot()
    
    # ===== ОБРАБОТЧИКИ МАСШТАБА =====
    def on_zoom_change(self, value):
        self.zoom = value / 100.0
        self.zoom_label.setText(f"{self.zoom:.2f}")
        
        # Синхронизация со спинбоксом
        self.zoom_spinbox.blockSignals(True)
        self.zoom_spinbox.setValue(self.zoom)
        self.zoom_spinbox.blockSignals(False)
        
        self.auto_update_plot()
    
    def on_zoom_spinbox_change(self, value):
        self.zoom = value
        self.zoom_label.setText(f"{self.zoom:.2f}")
        
        # Синхронизация со слайдером
        self.zoom_slider.blockSignals(True)
        self.zoom_slider.setValue(int(self.zoom * 100))
        self.zoom_slider.blockSignals(False)
        
        self.auto_update_plot()
    
    def auto_update_plot(self):
        """Автоматическое обновление графика с debounce"""
        if not self.auto_update_enabled:
            return
        
        # Перезапускаем таймер для debounce
        self.update_timer.start()
    
    def toggle_auto_update(self):
        """Включение/выключение автообновления"""
        self.auto_update_enabled = not self.auto_update_enabled
        
        if self.auto_update_enabled:
            self.auto_update_btn.setText("🔄 Автообновление: ВКЛ")
            self.auto_update_btn.setStyleSheet("""
                QPushButton {
                    background-color: #28A745;
                    color: white;
                    font-size: 12px;
                    padding: 8px 12px;
                    border-radius: 5px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #218838;
                }
            """)
            self.plot()  # Сразу обновляем
        else:
            self.auto_update_btn.setText("⏸ Автообновление: ВЫКЛ")
            self.auto_update_btn.setStyleSheet("""
                QPushButton {
                    background-color: #DC3545;
                    color: white;
                    font-size: 12px;
                    padding: 8px 12px;
                    border-radius: 5px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #c82333;
                }
            """)
    
    # ===== ГРАФИК =====
    def plot(self):
        # Получаем данные из API
        x_f, y_f, _, _, _, _ = get_function_data(self.a, self.b, self.c)
        
        if not x_f:
            return
        
        x = np.array(x_f)
        y = np.array(y_f)
        
        # Применяем масштаб к Y значениям
        y = y * self.zoom
        
        # Фильтруем по диапазону X
        mask = (x >= self.x_start) & (x <= self.x_end)
        x_filtered = x[mask]
        y_filtered = y[mask]
        
        # ===== ТАБЛИЦА ТОЧЕК (только 20 значений) =====
        self.points_table.setRowCount(0)
        
        # Выбираем равномерно 20 точек из всего диапазона
        if len(x_filtered) > 0:
            # Берем максимум 20 точек
            num_points = min(20, len(x_filtered))
            step_index = max(1, len(x_filtered) // num_points)
            
            for i in range(0, len(x_filtered), step_index):
                if self.points_table.rowCount() >= 20:
                    break
                    
                row = self.points_table.rowCount()
                self.points_table.insertRow(row)
                
                x_item = QTableWidgetItem(f"{x_filtered[i]:.4f}")
                y_item = QTableWidgetItem(f"{y_filtered[i]:.4f}")
                
                x_item.setTextAlignment(Qt.AlignRight | Qt.AlignVCenter)
                y_item.setTextAlignment(Qt.AlignRight | Qt.AlignVCenter)
                
                # Проверка на бесконечность
                if np.isinf(y_filtered[i]) or np.isnan(y_filtered[i]):
                    y_item.setText("undefined")
                    y_item.setForeground(Qt.red)
                
                self.points_table.setItem(row, 0, x_item)
                self.points_table.setItem(row, 1, y_item)
        
        # ===== ГРАФИК =====
        self.figure.clear()
        ax = self.figure.add_subplot(111)
        
        # Темная тема
        ax.set_facecolor("#121212")
        self.figure.patch.set_facecolor("#121212")
        
        # Куски функции
        mask1 = (x_filtered < 0) & (x_filtered >= self.x_start)
        mask2 = (x_filtered > 0) & (x_filtered < 1) & (x_filtered <= self.x_end)
        mask3 = (x_filtered >= 1) & (x_filtered <= self.x_end)
        
        if np.any(mask1):
            ax.plot(x_filtered[mask1], y_filtered[mask1], color="#3A82F7", 
                   label=f"∛x + {self.a:.2f}", linewidth=2)
        
        if np.any(mask2):
            ax.plot(x_filtered[mask2], y_filtered[mask2], color="#4CAF50", 
                   label=f"1/x + {self.b:.2f}", linewidth=2)
        
        if np.any(mask3):
            ax.plot(x_filtered[mask3], y_filtered[mask3], color="#FF5252", 
                   label=f"x² - {self.c:.2f}·x + 1", linewidth=2)
        
        # Вертикальные линии границ (если в диапазоне)
        if self.x_start <= 0 <= self.x_end:
            ax.axvline(x=0, color='white', linestyle='--', alpha=0.3, linewidth=1)
        
        if self.x_start <= 1 <= self.x_end:
            ax.axvline(x=1, color='white', linestyle='--', alpha=0.3, linewidth=1)
        
        # Легенда
        ax.legend(loc="upper left", facecolor="#121212", edgecolor="#555", labelcolor='white')
        
        # Сетка
        ax.grid(color="#2C2C2C", alpha=0.5)
        
        # Стиль осей
        ax.tick_params(colors='white', labelsize=9)
        for spine in ax.spines.values():
            spine.set_color('#555')
        
        # Подписи осей
        ax.set_xlabel('X', color='white', fontsize=10)
        ax.set_ylabel('Y', color='white', fontsize=10)
        
        # Установка диапазона X
        ax.set_xlim(self.x_start, self.x_end)
        
        # Автоматический диапазон Y с учетом масштаба
        y_finite = y_filtered[np.isfinite(y_filtered)]
        if len(y_finite) > 0:
            y_min, y_max = np.min(y_finite), np.max(y_finite)
            if np.isfinite(y_min) and np.isfinite(y_max):
                y_padding = (y_max - y_min) * 0.1
                if y_padding == 0:
                    y_padding = 1
                ax.set_ylim(y_min - y_padding, y_max + y_padding)
        
        self.canvas.draw()