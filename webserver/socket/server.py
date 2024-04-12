import socket

# 创建 socket 对象
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 获取本地主机名和端口号
host = socket.gethostname()
port = 12345

# 绑定端口
server_socket.bind((host, port))

# 设置最大连接数，此处为1
server_socket.listen(1)

# 等待客户端连接
print("Waiting for incoming connection...")
client_socket, addr = server_socket.accept()
print("Connection from: ", addr)

# 接收客户端发送的数据
while True:
    data = client_socket.recv(1024).decode()
    if not data:
        break
    print("Received message from client: ", data)

    # 发送响应给客户端
    client_socket.sendall(data.encode())

# 关闭连接
client_socket.close()

