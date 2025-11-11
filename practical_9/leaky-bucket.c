#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep()

int main() {
    int bucket_size, output_rate, input_packets, interval;
    int bucket_content = 0;

    printf("Enter bucket size (in packets): ");
    scanf("%d", &bucket_size);

    printf("Enter output (leak) rate (packets/sec): ");
    scanf("%d", &output_rate);

    printf("Enter number of input intervals to simulate: ");
    scanf("%d", &interval);

    for (int i = 0; i < interval; i++) {
        // Random number of incoming packets (simulate bursty traffic)
        input_packets = rand() % 10;  // random 0–9 packets per second
        printf("\nInterval %d: %d packets arrived.\n", i + 1, input_packets);

        // Add incoming packets to the bucket
        if (bucket_content + input_packets > bucket_size) {
            int dropped = (bucket_content + input_packets) - bucket_size;
            bucket_content = bucket_size;
            printf("Bucket overflow! %d packet(s) dropped.\n", dropped);
        } else {
            bucket_content += input_packets;
        }

        // Leak (transmit) packets
        printf("Transmitting %d packet(s).\n", 
               (bucket_content < output_rate) ? bucket_content : output_rate);

        if (bucket_content < output_rate)
            bucket_content = 0;
        else
            bucket_content -= output_rate;

        printf("Packets remaining in bucket: %d\n", bucket_content);

        sleep(1); // wait 1 second to simulate time passing
    }

    printf("\nSimulation complete.\n");
    return 0;
}

/*
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
*/
