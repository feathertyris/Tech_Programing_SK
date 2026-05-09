from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QLineEdit,
    QPushButton, QLabel
)
from auth_service import register


class RegisterWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Register")
        self.resize(350, 300)

        layout = QVBoxLayout()

        self.username = QLineEdit()
        self.username.setPlaceholderText("Username")

        self.email = QLineEdit()
        self.email.setPlaceholderText("Email")

        self.password = QLineEdit()
        self.password.setPlaceholderText("Password")

        self.status = QLabel()

        btn = QPushButton("Register")
        btn.clicked.connect(self.handle_register)

        layout.addWidget(self.username)
        layout.addWidget(self.email)
        layout.addWidget(self.password)
        layout.addWidget(btn)
        layout.addWidget(self.status)

        self.setLayout(layout)

    def handle_register(self):
        success, message = register(
            self.username.text(),
            self.password.text(),
            self.email.text()
        )

        self.status.setText(message)