#include <stdio.h>
#define INFINITY 999
#define N 4 // Number of routers/nodes

void printTable(int distance[N][N]) {
    printf("Distance Table:\n");
    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(distance[i][j] == INFINITY)
                printf("INF\t");
            else
                printf("%d\t", distance[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Example network cost matrix
    int distance[N][N] = {
        {0, 1, 3, INFINITY},
        {1, 0, 1, 7},
        {3, 1, 0, 2},
        {INFINITY, 7, 2, 0}
    };

    // Distance vector algorithm (Bellman-Ford)
    int updated;
    do {
        updated = 0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                for(int k=0;k<N;k++) {
                    if(distance[i][k] + distance[k][j] < distance[i][j]) {
                        distance[i][j] = distance[i][k] + distance[k][j];
                        updated = 1;
                    }
                }
            }
        }
    } while(updated);

    // Print final distance table
    printTable(distance);

    return 0;
}


/*
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NODES 5      // Maximum number of nodes in the network
#define INF INT_MAX      // Infinite distance (used to represent unreachable nodes)

// Distance table for each node (each node has a table for every other node)
int dist[MAX_NODES][MAX_NODES];

// Neighbor table to store the neighbors of each node
int neighbors[MAX_NODES][MAX_NODES];

// Mutex for thread synchronization
pthread_mutex_t lock;

void initialize_network() {
    // Set the initial distance table (initial distances to direct neighbors)
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (i == j) {
                dist[i][j] = 0;  // Distance to itself is 0
            } else if (neighbors[i][j] != 0) {
                dist[i][j] = neighbors[i][j];  // Set the distance to the direct neighbors
            } else {
                dist[i][j] = INF;  // Distance to other nodes is initially infinite
            }
        }
    }
}

void print_distance_table(int node) {
    printf("Distance Table for Node %d:\n", node);
    for (int i = 0; i < MAX_NODES; i++) {
        if (dist[node][i] == INF) {
            printf("To Node %d: INF\n", i);
        } else {
            printf("To Node %d: %d\n", i, dist[node][i]);
        }
    }
    printf("\n");
}

// Function to update the distance table of a node based on the received distance vector
void update_distance_table(int node, int *received_table) {
    pthread_mutex_lock(&lock);

    // For each destination node, check if the new route through the neighbor is shorter
    for (int i = 0; i < MAX_NODES; i++) {
        if (dist[node][i] > received_table[i]) {
            dist[node][i] = received_table[i];  // Update the distance table
        }
    }

    pthread_mutex_unlock(&lock);
}

// Function to simulate the sending of the distance vector to the neighbors
void *send_distance_vector(void *arg) {
    int node = *(int *)arg;
    int received_table[MAX_NODES];

    // Simulate sending the distance vector to all neighbors
    printf("Node %d is sending its distance vector to neighbors...\n", node);

    // Copy current node's distance table as the vector to send
    for (int i = 0; i < MAX_NODES; i++) {
        received_table[i] = dist[node][i];
    }

    // Simulate receiving the distance vectors from neighbors
    for (int i = 0; i < MAX_NODES; i++) {
        if (neighbors[node][i] != 0) {  // If node i is a neighbor
            printf("Node %d receiving distance vector from Node %d...\n", node, i);
            update_distance_table(node, received_table);
        }
    }

    return NULL;
}

int main() {
    // Initialize neighbors table
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (i == j) {
                neighbors[i][j] = 0;
            } else {
                // Manually define neighbors (distance between nodes)
                if ((i == 0 && j == 1) || (i == 1 && j == 0)) neighbors[i][j] = 1;
                if ((i == 1 && j == 2) || (i == 2 && j == 1)) neighbors[i][j] = 2;
                if ((i == 2 && j == 3) || (i == 3 && j == 2)) neighbors[i][j] = 3;
                if ((i == 3 && j == 4) || (i == 4 && j == 3)) neighbors[i][j] = 4;
            }
        }
    }

    // Initialize network
    initialize_network();

    pthread_mutex_init(&lock, NULL);

    pthread_t threads[MAX_NODES];

    // Create a thread for each node to simulate the exchange of distance vectors
    for (int i = 0; i < MAX_NODES; i++) {
        int *node_id = malloc(sizeof(int));
        *node_id = i;
        pthread_create(&threads[i], NULL, send_distance_vector, node_id);
    }

    // Wait for all threads to complete
    for (int i = 0; i < MAX_NODES; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final distance tables for each node
    for (int i = 0; i < MAX_NODES; i++) {
        print_distance_table(i);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
*/
