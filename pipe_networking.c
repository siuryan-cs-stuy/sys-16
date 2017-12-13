#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("wkp", 0644);
  printf("[server] created wkp\n");
  int wkp = open("wkp", O_RDONLY);
  printf("[server] opened wkp\n");
  char pipe_name[64];
  read(wkp, &pipe_name, sizeof(pipe_name));
  printf("[server] pipe_name: %s\n", pipe_name);
  remove("WKP");
  printf("[server] WKP removed\n");

  *to_client = open(pipe_name, O_WRONLY, 0644);
  printf("[server] writing %s\n", ACK);
  write(*to_client, ACK, sizeof(int));

  int res = -1;
  read(wkp, &res, sizeof(int));
  to_client = &wkp;
  if (res == -1) {
    printf("Error: didn't receive client message\n");
    return 1;
  }
  printf("[server] read %d\n", res);
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
  char pipe_name[64];
  sprintf(pipe_name, "pipe_%d", pid);
  mkfifo(pipe_name, 0644);
  printf("[client] made pipe %s\n", pipe_name);

  *to_server = open("wkp", O_WRONLY, 0644);
  printf("[client] opened wkp\n");
  printf("[client] writing %s\n", pipe_name);
  write(*to_server, &pipe_name, sizeof(pipe_name));

  int priv = open(pipe_name, O_RDONLY, 0644);
  printf("[client] opened pipe %s\n", pipe_name);

  int res = -1;
  read(priv, &res, HANDSHAKE_BUFFER_SIZE);
  if (res == -1) {
    printf("Error: didn't receive server message\n");
    return 1;

  }
  printf("[client] read %d\n", res);
  remove(pipe_name);
  printf("[client] removed %s\n", pipe_name);

  int msg = 0;
  printf("[client] writing %d\n", msg);
  write(*to_server, &msg, sizeof(int));
  return priv;
}
