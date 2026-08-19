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
    // Connect to or create the message queue using a shared key
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        exit(1);
    }

    struct message_buffer msg;
    msg.msg_type = 1; // Message type channel

    printf("=== PRODUCER PROCESS ===\n");
    printf("Enter messages to send to consumer (type 'bye' to stop):\n\n");

    while (1) {
        printf("Producer > ");
        if (fgets(msg.msg_text, sizeof(msg.msg_text), stdin) != NULL) {
            msg.msg_text[strcspn(msg.msg_text, "\n")] = '\0';
        }

        // Send message to the queue
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            break;
        }

        // Terminate producer loop if message is 'bye'
        if (strcasecmp(msg.msg_text, "bye") == 0) {
            printf("[Producer] 'bye' sent. Exiting producer.\n");
            break;
        }
    }

    return 0;
}

