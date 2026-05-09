from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLabel, QHBoxLayout
from PyQt5.QtCore import Qt


class TitlePage(QWidget):
    def __init__(self):
        super().__init__()

        layout = QVBoxLayout()
        layout.setSpacing(8)

        # ===== ВЕРХНИЙ БЛОК =====
        def make_label(text, bold=False):
            lbl = QLabel(text)
            lbl.setAlignment(Qt.AlignCenter)
            if bold:
                lbl.setStyleSheet("font-weight: bold; color: white;")
            return lbl

        layout.addStretch()

        layout.addWidget(make_label("Министерство науки и высшего образования Российской Федерации"))
        layout.addWidget(make_label("Федеральное государственное автономное образовательное учреждение высшего образования"))
        layout.addWidget(make_label("МОСКОВСКИЙ ПОЛИТЕХНИЧЕСКИЙ УНИВЕРСИТЕТ (МОСКОВСКИЙ ПОЛИТЕХ)", True))

        layout.addSpacing(20)

        layout.addWidget(make_label("КУРСОВОЙ ПРОЕКТ", True))
        layout.addWidget(make_label('По курсу "Технологии и методы программирования"'))

        layout.addSpacing(20)

        layout.addWidget(make_label("ТЕМА", True))
        layout.addWidget(make_label("Название"))

        layout.addStretch()

        # ===== ПРАВЫЙ НИЖНИЙ УГОЛ =====
        right_block = QVBoxLayout()
        right_block.setAlignment(Qt.AlignRight)

        right_block.addWidget(QLabel("Выполнили:"))
        right_block.addWidget(QLabel("Карпов Александр Владимирович 251-371"))
        right_block.addWidget(QLabel("Судник Владислав Владимирович 251-371"))
        right_block.addWidget(QLabel("Проверила:"))
        right_block.addWidget(QLabel("Киреева Галина Ивановна"))
        right_block.addWidget(QLabel("к.т.н доцент"))

        bottom = QHBoxLayout()
        bottom.addStretch()
        bottom.addLayout(right_block)

        layout.addLayout(bottom)

        layout.addSpacing(10)

        # ===== МОСКВА =====
        moscow = QLabel("Москва, 2026")
        moscow.setAlignment(Qt.AlignCenter)
        layout.addWidget(moscow)

        self.setLayout(layout)