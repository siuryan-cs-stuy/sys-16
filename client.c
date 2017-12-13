#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while(1) {
    char input[256];
    char server_response[256];
    printf(">> ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1] = 0;

    write(to_server, input, sizeof(input));
    read(from_server, server_response, sizeof(server_response));
    printf("[server] %s", server_response);
    printf("\n");
  }
}
