import socket
import time
import os

SOCKET_PATH = "/tmp/my_local_socket"

# Clean up
if os.path.exists(SOCKET_PATH):
    os.remove(SOCKET_PATH)

server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server.bind(SOCKET_PATH)
server.listen(1)

print("Waiting for connection...")
conn, _ = server.accept()

data = conn.recv(1024)
if data:
    parts = data.decode()
    print("Sent time:", parts)
    print(f"Elapsed time: {time.time()}")

conn.close()
server.close()
os.remove(SOCKET_PATH)
