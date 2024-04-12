import socket

# 创建 socket 对象
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 获取服务器的主机名和端口号
host = socket.gethostname()
port = 12345

# 连接服务器
client_socket.connect((host, port))

# 发送数据给服务器
message = "Hello, server!"
client_socket.sendall(message.encode())

# 接收服务器的响应
response = client_socket.recv(1024).decode()
print("Received response from server: ", response)

# 关闭连接
client_socket.close()

