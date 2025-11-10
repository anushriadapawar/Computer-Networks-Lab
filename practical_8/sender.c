#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PACKET_SIZE 1024
#define WINDOW_SIZE 4  // Maximum number of unacknowledged packets in the window
#define TOTAL_PACKETS 10  // Total number of packets to be sent

int ack_received[WINDOW_SIZE] = {0};  // Array to track ACKs for the packets in the window
int packets_sent = 0;  // Number of packets sent
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *send_packet(void *arg) {
    int packet_id = *((int *)arg);
    // Simulate sending a packet
    printf("Sender: Sending packet %d\n", packet_id);
    sleep(1);  // Simulate network delay
    printf("Sender: Packet %d sent\n", packet_id);

    // Simulate waiting for acknowledgment
    usleep(100);
    pthread_mutex_lock(&mutex);
    ack_received[packet_id % WINDOW_SIZE] = 1;  // Mark ACK received for the packet
    pthread_mutex_unlock(&mutex);
    printf("Sender: ACK received for packet %d\n", packet_id);
    return NULL;
}

void *sliding_window(void *arg) {
    while (packets_sent < TOTAL_PACKETS) {
        // Slide window and send new packets
        for (int i = 0; i < WINDOW_SIZE && packets_sent < TOTAL_PACKETS; i++) {
            int packet_id = packets_sent++;
            if (ack_received[i] == 0) {  // If no ACK yet, send the packet
                pthread_t thread;
                pthread_create(&thread, NULL, send_packet, (void *)&packet_id);
                pthread_detach(thread);
                sleep(1);  // Simulate the interval between sending packets
            }
        }

        // Wait for ACKs (simulating a very simple mechanism)
        sleep(1);  // Wait for the ACKs to be processed
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < WINDOW_SIZE; i++) {
            if (ack_received[i]) {
                printf("Window: Moving forward, packet %d acknowledged.\n", i + 1);
                ack_received[i] = 0;  // Reset the ACK slot
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t window_thread;
    pthread_create(&window_thread, NULL, sliding_window, NULL);
    pthread_join(window_thread, NULL);
    return 0;
}
