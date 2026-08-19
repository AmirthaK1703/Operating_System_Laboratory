#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX 10

// Structure to hold matrix data in shared memory
struct SharedData {
    int rows;
    int cols;
    int mat1[MAX][MAX];
    int mat2[MAX][MAX];
    int is_ready; // Flag: 0 = waiting for P1, 1 = ready for P2
};

int main() {
    // 1. Allocate shared memory segment
    int shmid = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // 2. Attach shared memory segment
    struct SharedData *shm_ptr = (struct SharedData *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize sync flag
    shm_ptr->is_ready = 0;

    // 3. Create Child Process (P2) from Parent (P1)
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {
        // --- PROCESS 1: PARENT (P1) ---
        printf("[P1 - Parent] Enter dimensions for matrices (Rows and Columns, max %d): ", MAX);
        if (scanf("%d %d", &shm_ptr->rows, &shm_ptr->cols) != 2 ||
            shm_ptr->rows > MAX || shm_ptr->cols > MAX || shm_ptr->rows <= 0 || shm_ptr->cols <= 0) {
            printf("[P1 - Parent] Invalid dimensions. Exiting.\n");
            shmctl(shmid, IPC_RMID, NULL);
            exit(1);
        }

        // Input Matrix 1
        printf("\n[P1 - Parent] Enter elements for Matrix 1 (%dx%d):\n", shm_ptr->rows, shm_ptr->cols);
        int i,j;
        for (i = 0; i < shm_ptr->rows; i++) {
            for (j = 0; j < shm_ptr->cols; j++) {
                scanf("%d", &shm_ptr->mat1[i][j]);
            }
        }

        // Input Matrix 2
        printf("\n[P1 - Parent] Enter elements for Matrix 2 (%dx%d):\n", shm_ptr->rows, shm_ptr->cols);
        for ( i = 0; i < shm_ptr->rows; i++) {
            for ( j = 0; j < shm_ptr->cols; j++) {
                scanf("%d", &shm_ptr->mat2[i][j]);
            }
        }

        // Signal process P2 that data entry is complete
        shm_ptr->is_ready = 1;

        // Wait for child process P2 to finish reading and calculating
        wait(NULL);

        // Cleanup shared memory
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        printf("\n[P1 - Parent] Shared memory cleaned up. Exiting.\n");

    } else {
        // --- PROCESS 2: CHILD (P2) ---
        // Busy-wait until P1 writes data into shared memory
        while (shm_ptr->is_ready == 0) {
            usleep(1000); // 1ms delay per check
        }

        printf("\n[P2 - Child] Reading matrices from shared memory...");
        printf("\n[P2 - Child] Calculating Sum of Matrices:\n\n");

        int i,j,sum[MAX][MAX];
        for (i = 0; i < shm_ptr->rows; i++) {
            for ( j = 0; j < shm_ptr->cols; j++) {
                sum[i][j] = shm_ptr->mat1[i][j] + shm_ptr->mat2[i][j];
            }
        }

        // Display the resulting sum matrix
        for ( i = 0; i < shm_ptr->rows; i++) {
            for ( j = 0; j < shm_ptr->cols; j++) {
                printf("%d\t", sum[i][j]);
            }
            printf("\n");
        }

        // Detach shared memory and terminate
        shmdt(shm_ptr);
        exit(0);
    }

    return 0;
}
