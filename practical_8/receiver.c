#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *receive_packet(void *arg) {
    int packet_id = *((int *)arg);
    // Simulate packet reception
    printf("Receiver: Receiving packet %d\n", packet_id);
    sleep(1);  // Simulate processing delay
    printf("Receiver: Packet %d processed, sending ACK\n", packet_id);
    return NULL;
}

int main() {
    int total_packets = 10;
    for (int i = 0; i < total_packets; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, receive_packet, (void *)&i);
        pthread_detach(thread);  // Detach so we don't need to join each one
        usleep(100);  // Simulate network delay between receiving packets
    }
    // Wait for all the receiver threads to finish (if needed)
    sleep(5);  // Just for simulation
    return 0;
}
