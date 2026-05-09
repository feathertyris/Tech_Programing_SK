from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLineEdit, QPushButton, QLabel, QFrame, QHBoxLayout
from PyQt5.QtCore import Qt

from auth_service import login, register
from email_service import send_reset_email


class LoginWindow(QWidget):
    def __init__(self, on_success):
        super().__init__()
        self.on_success = on_success

        self.is_login_mode = True

        # ===== ГЛАВНЫЙ ЛЕЙАУТ (центрирование) =====
        main_layout = QVBoxLayout()

        center_layout = QHBoxLayout()
        center_layout.addStretch()

        self.card = QFrame()
        self.card.setFixedWidth(400)  # 👈 фикс ширина (ключевое)
        self.card.setStyleSheet("""
            QFrame {
                background-color: #1A1A1A;
                border-radius: 16px;
                padding: 30px;
            }
        """)

        self.layout = QVBoxLayout()

        # ===== ЗАГОЛОВОК =====
        self.title = QLabel("")
        self.title.setAlignment(Qt.AlignCenter)
        self.title.setStyleSheet("font-size: 24px; font-weight: bold; color: white;")

        # ===== ПОЛЯ =====
        self.username = QLineEdit()
        self.username.setPlaceholderText("Username")

        self.email = QLineEdit()
        self.email.setPlaceholderText("Email")

        self.password = QLineEdit()
        self.password.setPlaceholderText("Password")
        self.password.setEchoMode(QLineEdit.Password)

        # ===== СТАТУС =====
        self.status = QLabel("")
        self.status.setAlignment(Qt.AlignCenter)
        self.status.setWordWrap(True)
        self.status.setStyleSheet("color: #FF6B6B;")

        # ===== КНОПКИ =====
        self.main_btn = QPushButton()
        self.main_btn.clicked.connect(self.handle_main_action)

        self.switch_btn = QPushButton()
        self.switch_btn.setStyleSheet("""
            QPushButton {
                background-color: transparent;
                color: #3A82F7;
            }
            QPushButton:hover {
                text-decoration: underline;
            }
        """)
        self.switch_btn.clicked.connect(self.toggle_mode)

        self.reset_btn = QPushButton("Forgot password?")
        self.reset_btn.clicked.connect(self.reset_password)

        # ===== СБОРКА =====
        self.layout.addWidget(self.title)
        self.layout.addWidget(self.username)
        self.layout.addWidget(self.email)
        self.layout.addWidget(self.password)
        self.layout.addWidget(self.main_btn)
        self.layout.addWidget(self.reset_btn)
        self.layout.addWidget(self.switch_btn)
        self.layout.addWidget(self.status)

        self.card.setLayout(self.layout)

        center_layout.addWidget(self.card)
        center_layout.addStretch()

        main_layout.addStretch()
        main_layout.addLayout(center_layout)
        main_layout.addStretch()

        self.setLayout(main_layout)

        self.update_ui()

    def toggle_mode(self):
        self.is_login_mode = not self.is_login_mode
        self.status.setText("")
        self.update_ui()

    def update_ui(self):
        if self.is_login_mode:
            self.title.setText("Welcome Back")
            self.main_btn.setText("Login")
            self.switch_btn.setText("No account? Register")

            self.email.hide()
            self.reset_btn.show()
        else:
            self.title.setText("Create Account")
            self.main_btn.setText("Register")
            self.switch_btn.setText("Already have account? Login")

            self.email.show()
            self.reset_btn.hide()

    def handle_main_action(self):
        username = self.username.text()
        password = self.password.text()

        if self.is_login_mode:
            if login(username, password):
                self.on_success()
            else:
                self.status.setText("Invalid credentials")
        else:
            email = self.email.text()
            success, message = register(username, password, email)
            self.status.setText(message)

            if success:
                self.toggle_mode()

    def reset_password(self):
        username = self.username.text()
        email = self.email.text()

        if not email:
            self.email.show()
            self.email.setPlaceholderText("Enter your email for recovery")
            self.status.setText("Enter your email and press again")
            return

        if send_reset_email(email, username):
            self.status.setText("New password sent to your email")
        else:
            self.status.setText("Failed to reset password")