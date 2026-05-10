from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QStackedWidget, QLabel, QHBoxLayout, QFrame
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
from resourses import resource_path
from graph_window import GraphWindow


class InfoScreen(QWidget):
    def __init__(self):
        super().__init__()

        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(60, 40, 60, 40)
        main_layout.setSpacing(25)

        # ===== ЗАГОЛОВОК =====
        title = QLabel("Задание на курсовой проект")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("""
            QLabel {
                font-size: 26px;
                font-weight: bold;
                color: white;
                margin-bottom: 20px;
            }
        """)

        # ===== ОПИСАНИЕ (РАСТЯНУТОЕ НА ВСЮ ШИРИНУ) =====
        description = QLabel(
            "Разработать приложение для вычисления значений кусочной функции "
            "на интервале [-10; 10] с последующим отображением графика.\n\n"
            "При изменении параметров a, b, c график обновляется автоматически."
        )
        description.setWordWrap(True)
        description.setStyleSheet("""
            QLabel {
                background-color: #1A1A1A;
                border-radius: 16px;
                padding: 20px;
                font-size: 18px;
                line-height: 1.6;
            }
        """)

        # ===== НИЖНИЙ БЛОК =====
        bottom_layout = QHBoxLayout()
        bottom_layout.setSpacing(30)

        # ===== ФУНКЦИЯ (РАСТЯНУТАЯ) =====
        func_frame = QFrame()
        func_frame.setMinimumWidth(450)
        func_frame.setStyleSheet("""
            QFrame {
                background-color: #1A1A1A;
                border-radius: 16px;
                padding: 20px;
            }
        """)

        func_layout = QVBoxLayout()
        func_layout.setSpacing(15)

        func_title = QLabel("Функция")
        func_title.setStyleSheet("""
            QLabel {
                font-size: 22px;
                font-weight: bold;
                color: white;
                margin-bottom: 5px;
            }
        """)

        func_image = QLabel()
        pixmap = QPixmap(resource_path("ui/formula.png"))

        if pixmap.isNull():
            print("Ошибка: formula.png не найден")
            func_image.setText("""
                f(x) = ⎧ ∛x + a, при x < 0
                       ⎨ 1/x - b, при 0 < x < 1
                       ⎩ x² - c·x + 1, при x ≥ 1
            """)
            func_image.setStyleSheet("""
                QLabel {
                    font-family: monospace;
                    font-size: 16px;
                    color: #E0E0E0;
                    padding: 20px;
                    background-color: #0F0F0F;
                    border-radius: 12px;
                }
            """)
        else:
            func_image.setPixmap(pixmap)
            func_image.setScaledContents(True)
            func_image.setMinimumHeight(150)
            func_image.setMinimumWidth(400)

        func_layout.addWidget(func_title)
        func_layout.addWidget(func_image)

        func_frame.setLayout(func_layout)

        # ===== ОСОБЕННОСТИ (РАСТЯНУТЫЕ) =====
        features_frame = QFrame()
        features_frame.setMinimumWidth(450)
        features_frame.setStyleSheet("""
            QFrame {
                background-color: #1A1A1A;
                border-radius: 16px;
                padding: 20px;
            }
        """)

        features_layout = QVBoxLayout()
        features_layout.setSpacing(15)
        features_layout.setContentsMargins(0, 0, 0, 0)

        features_title = QLabel("Особенности")
        features_title.setStyleSheet("""
            QLabel {
                font-size: 22px;
                font-weight: bold;
                color: white;
                margin-bottom: 5px;
            }
        """)

        features_text = QLabel(
            "• ∛x + a определена при всех x < 0\n"
            "• 1/x - b имеет разрыв при x = 0\n"
            "• на интервале (0, 1) функция убывает\n"
            "• x² - cx + 1 = (x - 1)² ≥ 0\n"
            "• минимум достигается при x = 1"
        )
        features_text.setWordWrap(True)
        features_text.setAlignment(Qt.AlignTop)
        features_text.setStyleSheet("""
            QLabel {
                font-size: 18px;
                line-height: 1.8;
                padding: 0px;
                margin: 0px;
            }
        """)

        features_layout.addWidget(features_title)
        features_layout.addWidget(features_text)
        features_layout.addStretch()

        features_frame.setLayout(features_layout)

        # Растягиваем оба блока
        bottom_layout.addWidget(func_frame)
        bottom_layout.addWidget(features_frame)

        # ===== СБОРКА =====
        main_layout.addWidget(title)
        main_layout.addWidget(description)
        main_layout.addLayout(bottom_layout)
        main_layout.addStretch()

        self.setLayout(main_layout)


class MainWindow(QWidget):
    def __init__(self, on_logout):
        super().__init__()

        self.setWindowTitle("Course Project")
        self.resize(1280, 720)  # Установлено разрешение 1280×720

        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)

        # ===== STACKED WIDGET =====
        self.stack = QStackedWidget()

        self.info_screen = InfoScreen()
        self.graph_screen = GraphWindow(on_logout)

        self.stack.addWidget(self.info_screen)  # 0 - Информация
        self.stack.addWidget(self.graph_screen)  # 1 - График

        # ===== СБОРКА =====
        layout.addWidget(self.stack)

        self.setLayout(layout)
        
        # Установка начальной страницы
        self.stack.setCurrentIndex(0)