import socket
import time

SOCKET_PATH = "/tmp/my_local_socket"

client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
client.connect(SOCKET_PATH)

# Add timestamp
send_time = time.time()
message = f"{send_time}"

client.sendall(message.encode())
print("Message sent.")

client.close()
