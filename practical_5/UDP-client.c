#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Localhost (server running on the same machine)
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char *message = "Hello from UDP client!";
    char buffer[BUFFER_SIZE];

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Client socket created successfully\n");

    // Step 2: Define server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Use the server's IP

    // Step 3: Send message to the server
    ssize_t bytes_sent = sendto(sockfd, message, strlen(message), 0,
                                 (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        perror("Send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server: %s\n", message);

    // Step 4: Receive response from the server
    socklen_t server_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr*)&server_addr, &server_len);
    if (bytes_received == -1) {
        perror("Receive failed");
    } else {
        buffer[bytes_received] = '\0';  // Null-terminate the received message
        printf("Received response from server: %s\n", buffer);
    }

    // Step 5: Close the socket
    close(sockfd);
    printf("Client socket closed\n");

    return 0;
}
