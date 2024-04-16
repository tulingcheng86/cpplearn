#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const char *SERVER_IP = "127.0.0.1";
const int PORT = 8888;
const int BUFFER_SIZE = 1024;

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    int numCount, numbers[10], result;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket\n";
        return EXIT_FAILURE;
    }

    // Initialize server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported\n";
        return EXIT_FAILURE;
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server" << std::endl;

    // Input number count and numbers
    std::cout << "Enter number count: ";
    std::cin >> numCount;
    std::cout << "Enter " << numCount << " numbers: ";
    for (int i = 0; i < numCount; ++i) {
        std::cin >> numbers[i];
    }

    // Input operator
    char op;
    std::cout << "Enter operator (+, -, *): ";
    std::cin >> op;

    // Serialize numbers and operator
    int totalSize = numCount * sizeof(int) + sizeof(char);
    memcpy(buffer, numbers, numCount * sizeof(int));
    buffer[numCount * sizeof(int)] = op;

    // Send number count
    if (send(client_socket, &numCount, 1, 0) < 0) {
        std::cerr << "Send failed\n";
        return EXIT_FAILURE;
    }

    // Send numbers and operator
    if (send(client_socket, buffer, totalSize, 0) < 0) {
        std::cerr << "Send failed\n";
        return EXIT_FAILURE;
    }

    // Receive result
    if (recv(client_socket, &result, sizeof(result), 0) < 0) {
        std::cerr << "Receive failed\n";
        return EXIT_FAILURE;
    }

    // Convert result to host byte order
    result = ntohl(result);

    std::cout << "Result received from server: " << result << std::endl;

    // Close socket
    close(client_socket);

    return EXIT_SUCCESS;
}
