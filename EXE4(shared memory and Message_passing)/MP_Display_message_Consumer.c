#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 256
#define MSG_KEY 1234

struct message_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    // Connect to or create the message queue using the same key
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        exit(1);
    }

    struct message_buffer msg;

    printf("=== CONSUMER PROCESS ===\n");
    printf("Waiting for messages from producer...\n\n");

    while (1) {
        // Block until a message of type 1 is received
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        // Display received message immediately
        printf("Consumer Received: \"%s\"\n", msg.msg_text);

        // Check for 'bye' message to delete queue and exit
        if (strcasecmp(msg.msg_text, "bye") == 0) {
            printf("\n[Consumer] 'bye' received.\n");

            // Delete the message queue resource from the kernel
            if (msgctl(msgid, IPC_RMID, NULL) == -1) {
                perror("msgctl IPC_RMID failed");
            } else {
                printf("[Consumer] Message queue successfully deleted. Closing channel.\n");
            }
            break;
        }
    }

    return 0;
}
