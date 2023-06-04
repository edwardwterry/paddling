import socket
import serial

ser = serial.Serial('COM3', 9600)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_ip = '127.0.0.1'  # Replace with your server's IP address
server_port = 5555
# server_socket.bind((server_ip, server_port))
# server_socket.listen(1)

# print(f"Server listening on {server_ip}:{server_port}")

# server_socket.settimeout(5)
# client_socket, client_address = server_socket.accept()
# print(f"Connection from {client_address}")

while True:
    data = ser.readline().decode('utf-8').strip()
    print("Sending data: ", data)
    server_socket.sendto(data.encode() + b'\n', (server_ip, server_port))

# client_socket.close()
# server_socket.close()

