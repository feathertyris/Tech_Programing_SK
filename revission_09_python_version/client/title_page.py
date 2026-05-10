from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLabel, QHBoxLayout
from PyQt5.QtCore import Qt


class TitlePage(QWidget):
    def __init__(self):
        super().__init__()

        layout = QVBoxLayout()
        layout.setSpacing(12)

        # ===== ВЕРХНИЙ БЛОК =====
        def make_label(text, bold=False):
            lbl = QLabel(text)
            lbl.setAlignment(Qt.AlignCenter)
            if bold:
                lbl.setStyleSheet("font-weight: bold; color: white; font-size: 22px;")
            else:
                lbl.setStyleSheet("font-size: 20px;")
            return lbl

        layout.addStretch()

        layout.addWidget(make_label("Министерство науки и высшего образования Российской Федерации"))
        layout.addWidget(make_label("Федеральное государственное автономное образовательное учреждение высшего образования"))
        layout.addWidget(make_label("МОСКОВСКИЙ ПОЛИТЕХНИЧЕСКИЙ УНИВЕРСИТЕТ (МОСКОВСКИЙ ПОЛИТЕХ)", True))

        layout.addSpacing(30)

        layout.addWidget(make_label("КУРСОВОЙ ПРОЕКТ", True))
        layout.addWidget(make_label('По курсу "Технологии и методы программирования"'))

        layout.addSpacing(30)

        layout.addWidget(make_label("ТЕМА", True))
        layout.addWidget(make_label("Графическое отображение ветвящийся функции в рамках клиент-серверного проекта"))

        layout.addStretch()

        # ===== ПРАВЫЙ НИЖНИЙ УГОЛ =====
        right_block = QVBoxLayout()
        right_block.setAlignment(Qt.AlignRight)

        # Функция для создания label в правом блоке
        def make_right_label(text, bold=False):
            lbl = QLabel(text)
            if bold:
                lbl.setStyleSheet("font-size: 18px; font-weight: bold;")
            else:
                lbl.setStyleSheet("font-size: 18px;")
            return lbl

        right_block.addWidget(make_right_label("Выполнили:", True))
        right_block.addWidget(make_right_label("Карпов Александр Владимирович 251-371"))
        right_block.addWidget(make_right_label("Судник Владислав Владимирович 251-371"))
        right_block.addWidget(make_right_label("Проверила:", True))
        right_block.addWidget(make_right_label("Киреева Галина Ивановна"))
        right_block.addWidget(make_right_label("к.т.н доцент"))

        bottom = QHBoxLayout()
        bottom.addStretch()
        bottom.addLayout(right_block)

        layout.addLayout(bottom)

        layout.addSpacing(20)

        # ===== МОСКВА =====
        moscow = QLabel("Москва, 2026")
        moscow.setAlignment(Qt.AlignCenter)
        moscow.setStyleSheet("font-size: 20px; font-weight: bold;")
        layout.addWidget(moscow)

        self.setLayout(layout)