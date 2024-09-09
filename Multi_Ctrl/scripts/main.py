import socket

# List of ESP32 IP addresses
ESP32_IPS = [
    "192.168.4.1", # AP
    "192.168.4.2", # PC
    "192.168.4.3", 
    "192.168.4.4",
]
UDP_PORT = 12345

def send_udp_message(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(message.encode(), (ip, port))
    sock.close()

def set_rgb_color(ip, r, g, b):
    message = f"RGB {r},{g},{b}"
    send_udp_message(ip, UDP_PORT, message)

def control_all_boards(r, g, b):
    for ip in ESP32_IPS:
        set_rgb_color(ip, r, g, b)

# Example usage
control_all_boards(0, 0, 255)  # Set RGB LED on all boards
# set_rgb_color("192.168.4.4",0, 255, 0)
