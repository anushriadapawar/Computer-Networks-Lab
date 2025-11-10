#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_BUCKET_SIZE 10    // Maximum capacity of the leaky bucket
#define LEAK_RATE 1           // Rate at which packets leak from the bucket (1 packet per second)
#define TOTAL_PACKETS 20      // Total number of packets to simulate

int bucket_size = 0;           // Current number of packets in the bucket
pthread_mutex_t lock;          // Mutex lock for thread synchronization

// Function to simulate packet arrival and insertion into the bucket
void *arrive_packet(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        if (bucket_size < MAX_BUCKET_SIZE) {
            bucket_size++;  // Add packet to the bucket
            printf("Packet arrived: Bucket size = %d\n", bucket_size);
        } else {
            printf("Bucket overflow! Packet discarded.\n");
        }

        pthread_mutex_unlock(&lock);
        sleep(1);  // Simulate packet arrival interval
    }
}

// Function to simulate packet leakage (output at a constant rate)
void *leak_packet(void *arg) {
    while (1) {
        sleep(1);  // Leak at a fixed rate of 1 packet per second

        pthread_mutex_lock(&lock);

        if (bucket_size > 0) {
            bucket_size--;  // Leak one packet out of the bucket
            printf("Packet leaked: Bucket size = %d\n", bucket_size);
        }

        pthread_mutex_unlock(&lock);
    }
}

int main() {
    pthread_t arrival_thread, leak_thread;

    pthread_mutex_init(&lock, NULL);

    // Create thread for packet arrival
    pthread_create(&arrival_thread, NULL, arrive_packet, NULL);
    
    // Create thread for packet leakage
    pthread_create(&leak_thread, NULL, leak_packet, NULL);

    // Join threads (though in this case they run indefinitely)
    pthread_join(arrival_thread, NULL);
    pthread_join(leak_thread, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
