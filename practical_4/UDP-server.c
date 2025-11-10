#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080  // Port number to bind the server
#define BUFFER_SIZE 1024  // Size of the buffer for data

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Step 1: Create socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Server socket created successfully\n");

    // Step 2: Bind socket to an address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections on any network interface
    server_addr.sin_port = htons(PORT);  // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Binding successful on port %d\n", PORT);

    // Step 3: Receive data from the client
    client_len = sizeof(client_addr);
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0,
                                          (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received == -1) {
            perror("Receive failed");
        } else {
            printf("Received data: %s\n", buffer);
            printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        }

        // Step 4: Send response back to the client
        const char *response = "Hello from UDP server!";
        ssize_t bytes_sent = sendto(server_fd, response, strlen(response), 0,
                                     (struct sockaddr*)&client_addr, client_len);
        if (bytes_sent == -1) {
            perror("Send failed");
        } else {
            printf("Response sent to client\n");
        }
    }

    // Step 5: Close the server socket
    close(server_fd);
    printf("Server socket closed\n");

    return 0;
}
