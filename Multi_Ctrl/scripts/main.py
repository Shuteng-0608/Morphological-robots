import json
import socket

# List of ESP32 IP addresses
ESP32_IPS = [
    "192.168.4.1",
    "192.168.4.2",
    "192.168.4.3",
    "192.168.4.4",
]

ID_IP_table = {
    1: '192.168.4.2',
    2: '192.168.4.2',
    3: '192.168.4.2',
    4: '192.168.4.3',
    5: '192.168.4.3',
    6: '192.168.4.3',
}

IP_IDList_table = {
    '192.168.4.2': [1, 2, 3],
    '192.168.4.3': [4, 5, 6]
}


UDP_PORT = 12345

def send_udp_message(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(message.encode(), (ip, port))
    sock.close()

def query_ip_by_id(ID):
    return ID_IP_table.get(ID, "ID not found")

def query_id_list_by_ip(IP):
    return IP_IDList_table.get(IP, "IP not found")

def set_rgb_color(ID, r, g, b):
    ip = query_ip_by_id(ID)
    message = {
        'Cmd': "RGB",
        'ID': ID,
        'R': r,
        'G': g,
        'B': b
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

def position_up(ID):
    ip = query_ip_by_id(ID)
    message = {
        'Cmd': "Position+",
        'ID': ID
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

def position_down(ID):
    ip = query_ip_by_id(ID)
    message = {
        'Cmd': "Position-",
        'ID': ID
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

def write_pos_ex(ID, pos, vel, acc):
    ip = query_ip_by_id(ID)
    message = {
        'Cmd': "WritePosEx",
        'ID': ID,
        'pos': pos,
        'vel': vel,
        'acc': acc
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

def reg_write_pos_ex(ID, pos, vel, acc):
    ip = query_ip_by_id(ID)
    message = {
        'Cmd': "RegWritePosEx",
        'ID': ID,
        'pos': pos,
        'vel': vel,
        'acc': acc
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

def sync_write_pos_ex(ip, pos_list, vel_list, acc_list):
    message = {
        'Cmd': "SyncWritePosEx",
        'ID_list': query_id_list_by_ip(ip),
        'IDNum': len(query_id_list_by_ip(ip)),
        'pos_list': pos_list,
        'vel_list': vel_list,
        'acc_list': acc_list
    }
    json_message = json.dumps(message)
    send_udp_message(ip, UDP_PORT, json_message)

# Example usage
write_pos_ex(1, 2000, 4000, 100)
# control_all_boards(0, 255, 0)  # Set RGB LED to green on all boards
# position_up("192.168.4.2", 1)
# position_down("192.168.4.2", 2)
# set_rgb_color("192.168.4.2",0, 255, 0)
