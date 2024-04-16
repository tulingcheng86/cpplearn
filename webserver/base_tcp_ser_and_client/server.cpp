#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 80801;
const int BUFFER_SIZE = 1024;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket\n";
        return EXIT_FAILURE;
    }

    // Bind socket
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error binding socket\n";
        return EXIT_FAILURE;
    }

    // Listen
    listen(server_socket, 5);

    // Accept incoming connections
    std::cout << "Server listening on port " << PORT << std::endl;

    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
        std::cerr << "Error accepting connection\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connection accepted from " << inet_ntoa(client_address.sin_addr) << std::endl;

    // Receive message from client
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_received < 0) {
        std::cerr << "Error reading from socket\n";
        return EXIT_FAILURE;
    }
    std::cout << "Received from client: " << buffer << std::endl;

    // Close sockets
    close(client_socket);
    close(server_socket);

    return EXIT_SUCCESS;
}
