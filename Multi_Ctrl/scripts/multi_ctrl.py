import socket

# List of ESP32 IP addresses
ESP32_IPS = [
    "192.168.4.1",
    "192.168.4.2",
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

def send_cmd_message(ip, cmdT, cmdI, cmdA, cmdB):
    msg = f"CMD {cmdT},{cmdI},{cmdA},{cmdB}"
    send_udp_message(ip, UDP_PORT, msg)

def position_up(ip):
    message = f"Position+"
    send_udp_message(ip, UDP_PORT, message)

def position_down(ip):
    message = f"Position-"
    send_udp_message(ip, UDP_PORT, message)

def control_all_boards(r, g, b):
    for ip in ESP32_IPS:
        set_rgb_color(ip, r, g, b)
        send_cmd_message(ip, 0, 0, 0, 0)

# Example usage
# control_all_boards(0, 255, 0)  # Set RGB LED to green on all boards
send_cmd_message("192.168.4.2", 1, 5, 0, 0) # position +
send_cmd_message("192.168.4.2", 1, 6, 0, 0) # position -
# position_up("192.168.4.2")
# position_down("192.168.4.2")
# set_rgb_color("192.168.4.2",0, 255, 0)
