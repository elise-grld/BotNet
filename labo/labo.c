#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include "../network.h"
#include "../utils.h"

int main(int argc, char *argv[]) 
{
    char* port = DEFAULT_PORT;
    if (argc == 2) {
        port = argv[1];

    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid_serveur = sfork();
    if (pid_serveur == 0) {
        sexecl("./zombie/zombie", "zombie", port, NULL);
        perror("[Labo] Erreur lancement zombie");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    pid_t pid_controller = sfork();
    if (pid_controller == 0) {
        sexecl("./controller/controller", "controller", port, NULL);
        perror("[Labo] Erreur lancement controller");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    swait(NULL);
    skill(pid_serveur, SIGTERM);
    swait(NULL);
    return 0;
}