#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while(1) {
    char message[256];
    read(from_client, message, sizeof(message));
    printf("[client] %s\n", message);

    strcat(message, "testing");
    write(to_client, message, sizeof(message));
  }

  return 0;
}
