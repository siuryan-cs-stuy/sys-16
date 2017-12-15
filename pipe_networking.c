#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

    //Step 1.1
    mkfifo("wkp", 0644);
    printf("[server] created wkp\n");
    int wkp = open("wkp", O_RDONLY);
    printf("[server] opened wkp\n");
    char pipe[64];

    //Step 2.3
    read(wkp, &pipe, sizeof(pipe));
    printf("[server] pipe: %s\n", pipe);
    remove("wkp");
    printf("[server] WKP removed\n");

    //Step 2.4
    *to_client = open(pipe, O_WRONLY, 0644);
    printf("[server] writing %s\n", ACK);
    write(*to_client, ACK, sizeof(int));

    char response[256];
    int err;
    err = read(wkp, response, sizeof(response));
    to_client = &wkp;
    if (err == -1) {
        printf("[server] Error: didn't receive client message\n");
        return 1;
    }
    printf("[server] read %s\n", response);
    return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    int pid = getpid();
    char pipe[64];
    sprintf(pipe, "%d", pid);

    //Step 1.2
    mkfifo(pipe, 0644);
    printf("[client] made pipe %s\n", pipe);

    //Step 2.1
    *to_server = open("wkp", O_WRONLY, 0644);
    printf("[client] opened wkp\n");
    printf("[client] writing %s\n", pipe);
    write(*to_server, &pipe, sizeof(pipe));
    //Step 2.2

    //Step 2.4
    int private = open(pipe, O_RDONLY, 0644);
    printf("[client] opened pipe %s\n", pipe);

    int res = -1;
    read(private, &res, HANDSHAKE_BUFFER_SIZE);
    if (res == -1) {
        printf("[client] Error: didn't receive server message\n");
        return 1;

    }
    //Step 2.5?
    printf("[client] read %d\n", res);
    remove(pipe);
    printf("[client] removed %s\n", pipe);

    int msg = 0;
    printf("[client] writing %d\n", msg);
    write(*to_server, &msg, sizeof(int));
    return private;
}
