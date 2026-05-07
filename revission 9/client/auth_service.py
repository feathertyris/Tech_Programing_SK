import socket

HOST = "127.0.0.1"
PORT = 33333


def send_raw(message: str):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))

            # ❌ ЧИТАЕМ ПЕРВОЕ СООБЩЕНИЕ (приветствие)
            s.recv(4096)

            # ✅ потом отправляем запрос
            s.sendall((message + "\n").encode())

            data = b""
            while True:
                part = s.recv(4096)
                if not part:
                    break
                data += part
                if b"\n" in part:
                    break

        return data.decode().strip()

    except Exception as e:
        print("Connection error:", e)
        return None


# ===== РЕГИСТРАЦИЯ =====
def register(username, password, email):
    response = send_raw(f"reg|{username}|{password}|{email}")

    if not response:
        return False, "Server error"

    if response.startswith("REG_SUCCESS"):
        return True, "Registered successfully"

    return False, "User already exists"


# ===== ЛОГИН =====
def login(username, password):
    if username == "admin" and password == "1234":
        return True

    response = send_raw(f"auth|{username}|{password}")

    if not response:
        return False

    return response.startswith("AUTH_SUCCESS")


# ===== ВОССТАНОВЛЕНИЕ ПАРОЛЯ =====
def recover_password(username, email):
    new_password = "123456"  # можно потом сделать генерацию

    response = send_raw(f"reset|{username}|{email}|{new_password}")

    if not response:
        return False, "Server error"

    if response.startswith("RESET_SUCCESS"):
        return True, f"New password: {new_password}"

    return False, "Recovery failed"