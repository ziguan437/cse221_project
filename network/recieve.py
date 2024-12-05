import socket

def server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8090))
    server_socket.listen(1)
    for _ in range(100):
        connection, _ = server_socket.accept()
        connection.close()
    server_socket.close()

if __name__ == "__main__":
    server()


