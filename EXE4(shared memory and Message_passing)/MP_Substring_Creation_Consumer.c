#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY 5678
#define MAX_TEXT 256

struct message_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    // 1. Connect to the message queue
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        exit(1);
    }

    struct message_buffer msg;

    printf("=== CONSUMER PROCESS ===\n");
    printf("Waiting for string from Producer...\n\n");

    // 2. Receive message from queue
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("[Consumer] Full Received String: \"%s\"\n", msg.msg_text);

    // 3. Extract and display substring of length 4
    char substring[5];
    strncpy(substring, msg.msg_text, 4);
    substring[4] = '\0'; // Ensure null-termination

    printf("[Consumer] Substring (First 4 characters): \"%s\"\n", substring);

    // 4. Clean up message queue resource
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID failed");
    } else {
        printf("[Consumer] Message queue deleted successfully.\n");
    }

    return 0;
}
