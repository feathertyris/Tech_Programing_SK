from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QStackedWidget, QLabel, QHBoxLayout, QFrame
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
from resourses import resource_path
from graph_window import GraphWindow


class InfoScreen(QWidget):
    def __init__(self):
        super().__init__()

        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(40, 20, 40, 20)
        main_layout.setSpacing(15)

        # ===== ЗАГОЛОВОК =====
        title = QLabel("Задание на курсовой проект")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("font-size: 20px; font-weight: bold; color: white;")

        # ===== ОПИСАНИЕ =====
        description = QLabel(
            "Разработать приложение для вычисления значений кусочной функции "
            "на интервале [-10; 10] с последующим отображением графика.\n\n"
            "При изменении параметров a, b, c график обновляется автоматически."
        )
        description.setWordWrap(True)
        description.setMaximumWidth(800)
        description.setStyleSheet("""
            background-color: #1A1A1A;
            border-radius: 10px;
            padding: 12px;
        """)

        desc_container = QHBoxLayout()
        desc_container.addStretch()
        desc_container.addWidget(description)
        desc_container.addStretch()

        # ===== НИЖНИЙ БЛОК =====
        bottom_layout = QHBoxLayout()
        bottom_layout.setSpacing(20)

        # ===== ФУНКЦИЯ (КАРТИНКА) =====
        func_frame = QFrame()
        func_frame.setFixedWidth(350)
        func_frame.setStyleSheet("""
            QFrame {
                background-color: #1A1A1A;
                border-radius: 12px;
                padding: 12px;
            }
        """)

        func_layout = QVBoxLayout()

        func_title = QLabel("Функция")
        func_title.setStyleSheet("font-weight: bold; color: white;")

        func_image = QLabel()
        pixmap = QPixmap(resource_path("ui/formula.png"))

        if pixmap.isNull():
            print("Ошибка: formula.png не найден")

        func_image.setPixmap(pixmap)
        func_image.setScaledContents(True)
        func_image.setFixedHeight(120)

        func_layout.addWidget(func_title)
        func_layout.addWidget(func_image)

        func_frame.setLayout(func_layout)

        # ===== ОСОБЕННОСТИ =====
        features_frame = QFrame()
        features_frame.setStyleSheet("""
            QFrame {
                background-color: #1A1A1A;
                border-radius: 12px;
                padding: 12px;
            }
        """)

        features_layout = QVBoxLayout()

        features_title = QLabel("Особенности")
        features_title.setStyleSheet("font-weight: bold; color: white;")

        features = QLabel(
            "• ∛x определена при всех x < 0\n"
            "• 1/x имеет разрыв при x = 0\n"
            "• на интервале (0, 1) функция убывает\n"
            "• x² - 2x + 1 = (x - 1)² ≥ 0\n"
            "• минимум достигается при x = 1"
        )
        features.setWordWrap(True)

        features_layout.addWidget(features_title)
        features_layout.addWidget(features)

        features_frame.setLayout(features_layout)

        bottom_layout.addStretch()
        bottom_layout.addWidget(func_frame)
        bottom_layout.addWidget(features_frame)
        bottom_layout.addStretch()

        # ===== СБОРКА =====
        main_layout.addWidget(title)
        main_layout.addLayout(desc_container)
        main_layout.addLayout(bottom_layout)
        main_layout.addStretch()

        self.setLayout(main_layout)


class MainWindow(QWidget):
    def __init__(self, on_logout):
        super().__init__()

        self.setWindowTitle("Course Project")
        self.resize(1100, 700)

        layout = QVBoxLayout()

        self.stack = QStackedWidget()

        self.info_screen = InfoScreen()
        self.graph_screen = GraphWindow(on_logout)

        self.stack.addWidget(self.info_screen)
        self.stack.addWidget(self.graph_screen)

        layout.addWidget(self.stack)

        self.setLayout(layout)