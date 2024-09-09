import socket

# List of ESP32 IP addresses
ESP32_IPS = [
    "192.168.1.100",
    "192.168.1.101",
    "192.168.1.102"
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
# control_all_boards(0, 255, 0)  # Set RGB LED to green on all boards
set_rgb_color("192.168.4.3",0, 255, 0)