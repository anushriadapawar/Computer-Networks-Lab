#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_BUCKET_SIZE 10    // Maximum number of tokens in the bucket
#define TOKEN_RATE 1          // Rate at which tokens are generated (1 token per second)
#define MAX_BURST_SIZE 5      // Maximum burst size (maximum tokens used in one go)
#define TOTAL_PACKETS 20      // Total number of packets to simulate

int token_count = 0;          // Current number of tokens in the bucket
pthread_mutex_t lock;         // Mutex lock for thread synchronization

// Function to simulate token generation (adding tokens to the bucket)
void *generate_tokens(void *arg) {
    while (1) {
        sleep(1);  // Add a token every second

        pthread_mutex_lock(&lock);

        if (token_count < MAX_BUCKET_SIZE) {
            token_count++;  // Add a token to the bucket
            printf("Token generated: Token count = %d\n", token_count);
        }

        pthread_mutex_unlock(&lock);
    }
}

// Function to simulate packet transmission (use a token for each packet)
void *send_packet(void *arg) {
    while (1) {
        sleep(1);  // Try to send a packet every second

        pthread_mutex_lock(&lock);

        if (token_count > 0) {
            token_count--;  // Use one token for sending a packet
            printf("Packet sent: Token count = %d\n", token_count);
        } else {
            printf("No tokens available! Packet delayed.\n");
        }

        pthread_mutex_unlock(&lock);
    }
}

int main() {
    pthread_t token_thread, packet_thread;

    pthread_mutex_init(&lock, NULL);

    // Create thread for token generation
    pthread_create(&token_thread, NULL, generate_tokens, NULL);

    // Create thread for packet transmission
    pthread_create(&packet_thread, NULL, send_packet, NULL);

    // Join threads (though in this case they run indefinitely)
    pthread_join(token_thread, NULL);
    pthread_join(packet_thread, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
