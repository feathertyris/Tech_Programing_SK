import smtplib
import random
import string
from email.mime.text import MIMEText
from auth_service import send_raw


def generate_password(length=8):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))


def send_reset_email(email, username):
    new_password = generate_password()

    print("DEBUG: trying reset for", username, email)

    response = send_raw(f"reset|{username}|{email}|{new_password}")
    print("DEBUG: server response =", response)

    if not response or not response.startswith("RESET_SUCCESS"):
        print("DEBUG: reset failed on server")
        return False

    try:
        print("DEBUG: sending email...")

        msg = MIMEText(f"Your new password: {new_password}")
        msg["Subject"] = "Password Reset"
        msg["From"] = "your_email@gmail.com"
        msg["To"] = email

        with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
            server.login("harrypottermine@gmail.com", "dzwu tixg uyka jqgq")
            server.send_message(msg)

        print("DEBUG: email sent")
        return True

    except Exception as e:
        print("EMAIL ERROR:", e)
        return False