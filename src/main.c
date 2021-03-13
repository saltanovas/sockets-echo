#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
    unsigned short port = 0;
    int sfd = 0;

    if (argc == 1)
    {
        printf("\nUse -c prefix to initialize a client.\n");
        printf("Use -s prefix to initialize a server.\n");
        printf("A port number after the prefix is optional.\n");
        printf("Exiting...\n\n");
    }
    else if (argc == 2 || argc == 3)
    {
        if (argc == 2)
        {
            port = PORT;
        }
        else if (!is_port_valid(argv[2]))
        {
            printf(RED "\nThe port %s number is invalid. Exiting...\n\n" RESET, argv[2]);
            exit(1);
        }
        else
        {
            port = strtoul(argv[2], NULL, 10);
        }

        if (strncmp(argv[1], "-s", strlen(argv[1])) == 0)
        {
            printf(GREEN "\nIntializing server on %s:%hu\n" RESET, IP_ADDRESS, port);

            if ((sfd = init_server(port, IP_ADDRESS)) == -1)
                exit(1);

            accept_connections(sfd);
        }
        else if (strncmp(argv[1], "-c", strlen(argv[1])) == 0)
        {
            printf(GREEN "\nInitializing client on %s:%hu\n" RESET, IP_ADDRESS, port);

            if ((sfd = init_client(port, IP_ADDRESS)) == -1)
                exit(1);

            client_conversation(sfd);
        }
        else
        {
            fprintf(stderr, RED "\n%s argument is unknown. Exiting...\n\n" RESET, argv[1]);
        }
    }
    else
    {
        fprintf(stderr, RED "\nToo many arguments. Exiting...\n\n" RESET);
    }
}
