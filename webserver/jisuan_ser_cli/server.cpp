#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 8888;
const int BUFFER_SIZE = 1024;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];
    int numCount, result = 0;

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
    listen(server_socket, 1);

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connections
    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
        std::cerr << "Error accepting connection\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connection accepted from " << inet_ntoa(client_address.sin_addr) << std::endl;

    // Receive number count
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(client_socket, buffer, 1, 0) < 0) {
        std::cerr << "Error receiving number count\n";
        return EXIT_FAILURE;
    }
    numCount = buffer[0];

    // Receive numbers and operator
    int totalSize = numCount * sizeof(int) + sizeof(char);
    if (recv(client_socket, buffer, totalSize, 0) < 0) {
        std::cerr << "Error receiving numbers and operator\n";
        return EXIT_FAILURE;
    }

    // Deserialize numbers and operator
    int *numbers = reinterpret_cast<int*>(buffer);
    char op = buffer[numCount * sizeof(int)];

    // Perform calculation
    for (int i = 0; i < numCount; ++i) {
        if (op == '+') {
            result += numbers[i];
        } else if (op == '-') {
            result -= numbers[i];
        } else if (op == '*') {
            if (i == 0) {
                result = numbers[i];
            } else {
                result *= numbers[i];
            }
        }
    }

    // Convert result to network byte order
    result = htonl(result);

    // Send result back to client
    if (send(client_socket, &result, sizeof(result), 0) < 0) {
        std::cerr << "Error sending result\n";
        return EXIT_FAILURE;
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    std::cout << "Result sent to client: " << result << std::endl;

    return EXIT_SUCCESS;
}
