from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QLabel, QSlider,
    QHBoxLayout, QListWidget, QPushButton
)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from resourses import resource_path
from PyQt5.QtGui import QPixmap

from api_client import get_function_data

import numpy as np


class GraphWindow(QWidget):
    def __init__(self, on_logout=None):
        super().__init__()

        self.on_logout = on_logout

        self.a = 1
        self.b = 1
        self.c = 1

        main_layout = QVBoxLayout()

        # ===== TOP BAR =====
        top_bar = QHBoxLayout()

        # ===== ФОРМУЛА (ИЗ КАРТИНКИ) =====
        self.formula_label = QLabel()

        pixmap = QPixmap(resource_path("formula.png"))# 👈 твоя картинка
        if pixmap.isNull():
            print("Ошибка: formula.png не найден")

        self.formula_label.setPixmap(pixmap)
        self.formula_label.setScaledContents(True)
        self.formula_label.setFixedHeight(120)

        top_bar.addWidget(self.formula_label)
        top_bar.addStretch()

        # ===== КРЕСТИК =====
        self.logout_btn = QPushButton("✕")
        self.logout_btn.setFixedSize(40, 40)
        self.logout_btn.clicked.connect(self.handle_logout)

        top_bar.addWidget(self.logout_btn)

        # ===== КОНТЕНТ =====
        content_layout = QHBoxLayout()

        # ===== ЛЕВАЯ ПАНЕЛЬ =====
        left_panel = QVBoxLayout()
        left_container = QWidget()
        left_container.setFixedWidth(180)

        self.a_label = QLabel("a = 1")
        self.b_label = QLabel("b = 1")
        self.c_label = QLabel("c = 1")

        self.a_slider = self.create_slider(self.on_a_change)
        self.b_slider = self.create_slider(self.on_b_change)
        self.c_slider = self.create_slider(self.on_c_change)

        # ===== СПИСОК ТОЧЕК =====
        self.points_list = QListWidget()
        self.points_list.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        header = QLabel("   x              y")
        header.setStyleSheet("font-weight: bold;")

        left_panel.addWidget(self.a_label)
        left_panel.addWidget(self.a_slider)

        left_panel.addWidget(self.b_label)
        left_panel.addWidget(self.b_slider)

        left_panel.addWidget(self.c_label)
        left_panel.addWidget(self.c_slider)

        left_panel.addWidget(header)
        left_panel.addWidget(self.points_list)

        left_container.setLayout(left_panel)

        # ===== ГРАФИК =====
        self.figure = Figure()
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


        self.plot()


    # ===== СЛАЙДЕР =====
    def create_slider(self, callback):
        slider = QSlider(Qt.Horizontal)
        slider.setMinimum(-50)
        slider.setMaximum(50)
        slider.setValue(10)
        slider.valueChanged.connect(callback)
        return slider

    def handle_logout(self):
        if self.on_logout:
            self.on_logout()

    # ===== ОБРАБОТКА =====
    def on_a_change(self, value):
        self.a = value / 10
        self.a_label.setText(f"a = {self.a:.1f}")
        self.plot()

    def on_b_change(self, value):
        self.b = value / 10
        self.b_label.setText(f"b = {self.b:.1f}")
        self.plot()

    def on_c_change(self, value):
        self.c = value / 10
        self.c_label.setText(f"c = {self.c:.1f}")
        self.plot()

    # ===== ГРАФИК =====
    def plot(self):
        x_f, y_f, _, _, _, _ = get_function_data(self.a, self.b, self.c)

        if not x_f:
            return

        x = np.array(x_f)
        y = np.array(y_f)

        # ===== СПИСОК ТОЧЕК =====
        self.points_list.clear()

        for xi, yi in zip(x, y):
            self.points_list.addItem(f"{xi:>6.2f}   {yi:>6.2f}")

        # ===== ГРАФИК =====
        self.figure.clear()
        ax = self.figure.add_subplot(111)

        ax.set_facecolor("#121212")
        self.figure.patch.set_facecolor("#121212")

        # убираем лишние отступы
        self.figure.subplots_adjust(
            left=0.08,
            right=0.98,
            top=0.98,
            bottom=0.08
        )

        # куски функции
        mask1 = x < 0
        mask2 = (x > 0) & (x < 1)
        mask3 = x >= 1

        ax.plot(x[mask1], y[mask1], color="#3A82F7", label="∛x")
        ax.plot(x[mask2], y[mask2], color="#4CAF50", label="1/x")
        ax.plot(x[mask3], y[mask3], color="#FF5252", label="x² - 2x + 1")

        # легенда
        ax.legend(loc="upper left")

        # сетка
        ax.grid(color="#2C2C2C")

        # стиль осей
        ax.tick_params(colors='white')
        for spine in ax.spines.values():
            spine.set_color('#555')

        # 👇 ВАЖНО: сохраняем пропорции
        ax.set_aspect('auto')

        self.canvas.draw()