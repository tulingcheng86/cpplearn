#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const char *SERVER_IP = "127.0.0.1";
const int PORT =80801;
const int BUFFER_SIZE=1024;

int main()
{
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = "Hello, server!";

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

    // Send message to server
    if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
        std::cerr << "Send failed\n";
        return EXIT_FAILURE;
    }


    std::cout << "Message sent to server: " << buffer << std::endl;

    // Close socket
    close(client_socket);

    return EXIT_SUCCESS;










}