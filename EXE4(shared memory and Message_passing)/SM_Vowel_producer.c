#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared.h"

int main() {
    int shmid = shmget(SHM_KEY, sizeof(struct shared_buffer), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("[Producer] shmget failed");
        exit(1);
    }

    struct shared_buffer *shm = (struct shared_buffer *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("[Producer] shmat failed");
        exit(1);
    }

    shm->is_ready = 0;

    printf("[Producer] Enter a string: ");
    if (fgets(shm->text, BUFFER_SIZE, stdin) != NULL) {
        shm->text[strcspn(shm->text, "\n")] = '\0';
    }

    shm->is_ready = 1;
    printf("[Producer] String written to shared memory. Waiting for Consumer...\n");

    while (shm->is_ready != 2) {
        usleep(1000);
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    printf("[Producer] Shared memory deallocated. Exiting.\n");

    return 0;
}
