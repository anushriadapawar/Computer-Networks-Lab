#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int capacity;       // Maximum number of tokens
    int tokens;         // Current number of tokens
    int rate;           // Tokens added per second
} TokenBucket;

// Initialize the token bucket
void initBucket(TokenBucket *bucket, int capacity, int rate) {
    bucket->capacity = capacity;
    bucket->tokens = capacity;  // start full
    bucket->rate = rate;
}

// Refill tokens based on elapsed time
void refillTokens(TokenBucket *bucket, int elapsed_seconds) {
    bucket->tokens += bucket->rate * elapsed_seconds;
    if (bucket->tokens > bucket->capacity)
        bucket->tokens = bucket->capacity;
}

// Try to consume tokens to send a packet
int sendPacket(TokenBucket *bucket, int packet_size) {
    if (bucket->tokens >= packet_size) {
        bucket->tokens -= packet_size;
        return 1; // packet sent
    } else {
        return 0; // not enough tokens, packet dropped
    }
}

int main() {
    TokenBucket bucket;
    int capacity = 10;  // max tokens
    int rate = 2;       // tokens added per second
    int packet_size = 3; // tokens needed per packet

    initBucket(&bucket, capacity, rate);

    srand(time(0));

    for (int t = 0; t < 20; t++) {
        // simulate time passing by 1 second
        sleep(1);
        refillTokens(&bucket, 1);

        printf("Time %d sec: Tokens available = %d\n", t+1, bucket.tokens);

        // Try to send a random number of packets
        int packets_to_send = rand() % 3 + 1; // 1 to 3 packets
        for (int i = 0; i < packets_to_send; i++) {
            if (sendPacket(&bucket, packet_size))
                printf("  Packet sent!\n");
            else
                printf("  Not enough tokens, packet dropped.\n");
        }
    }

    return 0;
}


/*
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
*/
