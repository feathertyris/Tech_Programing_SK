import socket
import json

HOST = "127.0.0.1"
PORT = 33333


def get_function_data(a=1, b=1, c=1):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))

            # ✅ ВАЖНО: читаем приветствие сервера
            s.recv(4096)

            message = f"plot|{a}|{b}|{c}\n"
            s.sendall(message.encode())

            data = b""
            while True:
                part = s.recv(4096)
                if not part:
                    break
                data += part
                if b"\n" in part:
                    break

        print("RAW RESPONSE:", data)  # 👈 добавь для отладки

        response = json.loads(data.decode())

        x, y = [], []

        for p in response["points"]:
            x.append(p["x"])
            y.append(p["y"])

        return x, y, [], [], [], []

    except Exception as e:
        print("Graph error:", e)
        return [], [], [], [], [], []