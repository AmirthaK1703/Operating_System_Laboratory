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
    // 1. Create or connect to the message queue
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        exit(1);
    }

    struct message_buffer msg;
    msg.msg_type = 1; // Message channel 1

    printf("=== PRODUCER PROCESS ===\n");

    // 2. Prompt user until input length > 10
    while (1) {
        printf("Enter a string (length must be > 10): ");
        if (fgets(msg.msg_text, sizeof(msg.msg_text), stdin) != NULL) {
            msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0'; // Remove trailing newline
        }

        size_t len = strlen(msg.msg_text);
        if (len > 10) {
            break;
        } else {
            printf("Error: Length is %zu. String must be strictly greater than 10 characters. Try again.\n\n", len);
        }
    }

    // 3. Send message to queue
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("[Producer] Message successfully sent to queue.\n");
    return 0;
}

