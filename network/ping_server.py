import socket

HOST = '127.0.0.1'  
PORT = 8090  
skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
skt.bind((HOST, PORT))
skt.listen(1)
while True:
    conn, addr = skt.accept()
    print('Connected by {}'.format(addr))

    while True:
        data = conn.recv(64)
        if not data:
            break
        conn.send(data)

    conn.close()
print('connect closed')