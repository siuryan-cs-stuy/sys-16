#include "pipe_networking.h"

void camelCase(char *);

int main() {

    int to_client;
    int from_client;
    char message[BUFFER_SIZE];

    while (1) {
        from_client = server_handshake( &to_client );

        while(read(from_client, message, sizeof(message))) {
            printf("[client] %s\n", message);

            camelCase(message);
            write(to_client, message, sizeof(message));
        }

        printf("[client] exited\n");
    }

    return 0;
}

void camelCase(char *message) {
    unsigned int i;
    for (i = 0; i < strlen(message); i++) {
        if (i % 2 == 0) {
            message[i] = toupper(message[i]);
        } else {
            message[i] = tolower(message[i]);
        }
    }
}
