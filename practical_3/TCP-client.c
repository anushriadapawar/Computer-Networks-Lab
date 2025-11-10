#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Localhost (server running on the same machine)
#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char *message = "Hello from client!";
    char buffer[1024];

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", SERVER_IP, PORT);

    // Step 4: Send message to the server
    send(sockfd, message, strlen(message), 0);
    printf("Message sent: %s\n", message);

    // Step 5: Receive response from the server
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Received response from server: %s\n", buffer);
    }

    // Step 6: Close the socket
    close(sockfd);
    printf("Client socket closed\n");

    return 0;
}
