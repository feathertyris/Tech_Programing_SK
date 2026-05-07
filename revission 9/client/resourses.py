import os
import sys

def resource_path(path):
    base = getattr(sys, "_MEIPASS", os.path.abspath("."))
    full_path = os.path.join(base, path)
    print("RESOURCE:", full_path)
    return full_path