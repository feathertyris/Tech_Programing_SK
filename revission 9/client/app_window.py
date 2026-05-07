from PyQt5.QtWidgets import QWidget, QVBoxLayout, QStackedWidget, QPushButton, QHBoxLayout
from ui.title_page import TitlePage
from ui.main_window import MainWindow
from ui.login_window import LoginWindow


class AppWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Course Project")
        self.resize(1100, 700)

        layout = QVBoxLayout()

        # ===== STACK =====
        self.stack = QStackedWidget()

        self.title_page = TitlePage()
        self.main_page = MainWindow(self.on_logout)
        self.login_page = LoginWindow(self.on_login_success)

        self.stack.addWidget(self.title_page)  # 0
        self.stack.addWidget(self.main_page)   # 1
        self.stack.addWidget(self.login_page)  # 2

        # ===== НАВИГАЦИЯ =====
        self.nav_layout = QHBoxLayout()

        self.prev_btn = QPushButton("←")
        self.next_btn = QPushButton("→")

        self.prev_btn.clicked.connect(self.go_prev)
        self.next_btn.clicked.connect(self.go_next)

        self.nav_layout.addWidget(self.prev_btn)
        self.nav_layout.addStretch()
        self.nav_layout.addWidget(self.next_btn)

        layout.addWidget(self.stack)
        layout.addLayout(self.nav_layout)

        self.setLayout(layout)

        self.update_buttons()

    # ===== НАВИГАЦИЯ =====
    def go_prev(self):
        index = self.stack.currentIndex()
        if index > 0:
            self.stack.setCurrentIndex(index - 1)
        self.update_buttons()

    def go_next(self):
        index = self.stack.currentIndex()
        if index < self.stack.count() - 1:
            self.stack.setCurrentIndex(index + 1)
        self.update_buttons()

    def update_buttons(self):
        index = self.stack.currentIndex()

        # ===== TITLE =====
        if index == 0:
            self.prev_btn.hide()
            self.next_btn.show()
            self.show_nav()

        # ===== MAIN =====
        elif index == 1:
            # если внутри main открыт график → скрываем стрелки
            if self.main_page.stack.currentIndex() == 1:
                self.hide_nav()
            else:
                self.prev_btn.show()
                self.next_btn.show()
                self.show_nav()

        # ===== LOGIN =====
        elif index == 2:
            self.prev_btn.show()
            self.next_btn.hide()
            self.show_nav()

    # ===== ПОКАЗ/СКРЫТИЕ =====
    def hide_nav(self):
        self.prev_btn.hide()
        self.next_btn.hide()

    def show_nav(self):
        self.nav_layout.setSpacing(10)
        # сами кнопки уже управляются выше

    # ===== LOGIN =====
    def on_login_success(self):
        print("Logged in!")

        self.stack.setCurrentIndex(1)
        self.main_page.stack.setCurrentIndex(1)  # график

        self.update_buttons()

    # ===== LOGOUT =====
    def on_logout(self):
        print("Logged out")

        self.stack.setCurrentIndex(0)
        self.main_page.stack.setCurrentIndex(0)

        self.update_buttons()