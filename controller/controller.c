#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../network.h"
#include "../utils.h"

int sockfd;

void child_handler() {
    char buffer[BUFFER_SIZE];
    ssize_t nbChar;

    while ((nbChar = sread(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[nbChar] = '\0';
      printf("%s", buffer);
    }

    printf("Connection interrupted by Ctrl-D.\n");
    sclose(sockfd);
}

int initSocketClient(char * serverIP, int serverPort)
{
  int sockClientfd = ssocket();
  sconnect(serverIP, serverPort, sockClientfd);
  
  return sockClientfd;
}

int main(int argc, char **argv)
{
  printf("BotNet System activated.\n");

  int port = SERVER_PORT;
  if (argc == 2) {
    port = atoi(argv[1]);
  }

  sockfd = initSocketClient(SERVER_IP, port);
  if (sockfd == -1) {
    printf("Socket error\n");
  }

  int childId = fork_and_run0(child_handler);

  printf("Execute commands in the shell terminal:\n");
  char bash_cmd[BUFFER_SIZE];
  ssize_t nbChar;

  while ((nbChar = sread(STDIN_FILENO, bash_cmd, sizeof(bash_cmd) - 1)) > 0)
  {
    bash_cmd[nbChar] = '\0';
    swrite(sockfd, bash_cmd, nbChar);
  }

  shutdown(sockfd, SHUT_WR);
  sclose(sockfd);

  int wstatus;
  swait(&wstatus);

  printf("You are now disconnected.\n");
  
}
