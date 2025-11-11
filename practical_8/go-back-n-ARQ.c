#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 10
#define WINDOW_SIZE 4
#define TIMEOUT 3

typedef struct {
    int frame_no;
    bool acked;
} Frame;

void send_frame(Frame f) {
    printf("Sender: Sending Frame %d\n", f.frame_no);
}

bool simulate_ack_loss() {
    // 20% chance of ACK loss
    return rand() % 10 < 2;
}

bool simulate_frame_loss() {
    // 20% chance of frame loss
    return rand() % 10 < 2;
}

int main() {
    srand(time(NULL));
    Frame frames[TOTAL_FRAMES];
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frames[i].frame_no = i + 1;
        frames[i].acked = false;
    }

    int base = 0; // first frame in window
    int next_seq_num = 0;
    int time_counter = 0;

    while (base < TOTAL_FRAMES) {
        // Send frames in window
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < TOTAL_FRAMES) {
            send_frame(frames[next_seq_num]);
            next_seq_num++;
        }

        // Simulate receiver ACK
        for (int i = base; i < next_seq_num; i++) {
            if (frames[i].acked)
                continue;

            if (simulate_frame_loss()) {
                printf("Receiver: Frame %d lost, no ACK sent\n", frames[i].frame_no);
                continue;
            }

            if (simulate_ack_loss()) {
                printf("Receiver: ACK for Frame %d lost\n", frames[i].frame_no);
                continue;
            }

            printf("Receiver: Frame %d received successfully. Sending ACK %d\n",
                   frames[i].frame_no, frames[i].frame_no);
            frames[i].acked = true;
        }

        // Slide window
        while (base < TOTAL_FRAMES && frames[base].acked) {
            base++;
        }

        // Timeout check
        time_counter++;
        if (time_counter >= TIMEOUT) {
            // If any frame in window not acked, retransmit
            for (int i = base; i < next_seq_num; i++) {
                if (!frames[i].acked) {
                    printf("Timeout! Resending from Frame %d\n", frames[i].frame_no);
                    next_seq_num = base; // resend from base
                    break;
                }
            }
            time_counter = 0;
        }

        sleep(1);
        printf("------------------------------------\n");
    }

    printf("All frames sent and acknowledged successfully!\n");
    return 0;
}
