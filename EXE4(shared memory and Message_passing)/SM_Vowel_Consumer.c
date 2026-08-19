#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared.h"

int is_vowel(char ch) {
    ch = tolower((unsigned char)ch);
    return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

int main() {
    int shmid = shmget(SHM_KEY, sizeof(struct shared_buffer), 0666);
    if (shmid < 0) {
        perror("[Consumer] shmget failed. Is the Producer running?");
        exit(1);
    }

    struct shared_buffer *shm = (struct shared_buffer *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("[Consumer] shmat failed");
        exit(1);
    }

    printf("[Consumer] Waiting for Producer to enter data...\n");

    while (shm->is_ready == 0) {
        usleep(1000);
    }

    printf("[Consumer] String read from shared memory: \"%s\"\n", shm->text);
    printf("[Consumer] Vowels found: ");

    int i,vowel_count = 0;
    for (i = 0; shm->text[i] != '\0'; i++) {
        if (is_vowel(shm->text[i])) {
            printf("%c ", shm->text[i]);
            vowel_count++;
        }
    }

    if (vowel_count == 0) {
        printf("None");
    }
    printf("\n");

    shm->is_ready = 2;

    shmdt(shm);
    printf("[Consumer] Finished processing. Exiting.\n");

    return 0;
}
