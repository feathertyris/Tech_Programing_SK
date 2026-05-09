import sys

from PyQt5.QtWidgets import QApplication
from app_window import AppWindow



def main():
    app = QApplication(sys.argv)

    # ===== ГЛОБАЛЬНЫЙ СТИЛЬ =====
    app.setStyleSheet("""
        QWidget {
            background-color: #121212;
            color: #E0E0E0;
            font-family: Segoe UI, Arial;
            font-size: 14px;
        }

        QLineEdit {
            background-color: #1E1E1E;
            border: 1px solid #2C2C2C;
            border-radius: 8px;
            padding: 10px;
        }

        QLineEdit:focus {
            border: 1px solid #3A82F7;
        }

        QPushButton {
            background-color: #3A82F7;
            border: none;
            border-radius: 8px;
            padding: 8px 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #5A9BFF;
        }

        QPushButton:pressed {
            background-color: #2F6FE0;
        }

        QLabel {
            color: #A0A0A0;
        }
    """)

    # ===== ЗАПУСК ПРИЛОЖЕНИЯ =====
    window = AppWindow()
    window.show()

    sys.exit(app.exec_())


if __name__ == "__main__":
    main()