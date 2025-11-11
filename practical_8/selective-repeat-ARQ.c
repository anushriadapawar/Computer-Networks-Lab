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
    bool sent;
    bool acked;
    int timer;
} Frame;

bool simulate_frame_loss() {
    // 20% chance of frame loss
    return rand() % 10 < 2;
}

bool simulate_ack_loss() {
    // 20% chance of ACK loss
    return rand() % 10 < 2;
}

void send_frame(Frame f) {
    printf("Sender: Sending Frame %d\n", f.frame_no);
}

void receive_frame(Frame *f) {
    if (simulate_frame_loss()) {
        printf("Receiver: Frame %d lost. No ACK.\n", f->frame_no);
        return;
    }
    printf("Receiver: Frame %d received successfully.\n", f->frame_no);
    if (simulate_ack_loss()) {
        printf("Receiver: ACK for Frame %d lost.\n", f->frame_no);
        return;
    }
    printf("Receiver: Sending ACK for Frame %d\n", f->frame_no);
    f->acked = true;
}

int main() {
    srand(time(NULL));

    Frame frames[TOTAL_FRAMES];
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frames[i].frame_no = i + 1;
        frames[i].sent = false;
        frames[i].acked = false;
        frames[i].timer = 0;
    }

    int base = 0;
    int next_seq_num = 0;
    int time_step = 0;

    while (base < TOTAL_FRAMES) {
        // Send new frames within window
        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < TOTAL_FRAMES) {
            if (!frames[next_seq_num].sent) {
                send_frame(frames[next_seq_num]);
                frames[next_seq_num].sent = true;
                frames[next_seq_num].timer = 0;
            }
            next_seq_num++;
        }

        // Receiver side simulation
        for (int i = base; i < next_seq_num; i++) {
            if (frames[i].sent && !frames[i].acked) {
                receive_frame(&frames[i]);
            }
        }

        // Update timers and check for timeouts
        for (int i = base; i < next_seq_num; i++) {
            if (frames[i].sent && !frames[i].acked) {
                frames[i].timer++;
                if (frames[i].timer >= TIMEOUT) {
                    printf("Timeout for Frame %d. Retransmitting...\n", frames[i].frame_no);
                    send_frame(frames[i]);
                    frames[i].timer = 0;
                }
            }
        }

        // Slide the window
        while (base < TOTAL_FRAMES && frames[base].acked) {
            base++;
        }

        time_step++;
        sleep(1);
        printf("------------------------------------\n");
    }

    printf("All frames sent and acknowledged successfully!\n");
    return 0;
}
