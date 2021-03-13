#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "server.h"
#include "helpers.h"

int init_server(unsigned short port_number, char *ip_address)
{
    int server_socket = 0;
    struct sockaddr_in server_address;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        fprintf(stderr, RED "Could not create a socket.\n\n" RESET);
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    if (inet_aton(ip_address, &server_address.sin_addr) <= 0)
    {
        fprintf(stderr, RED "Failed to convert an ip address.\n\n" RESET);
        return -1;
    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        fprintf(stderr, RED "Could not bind the address.\n\n" RESET);
        return -1;
    }

    if (listen(server_socket, 1) == -1)
    {
        fprintf(stderr, RED "Failed to start listen for connections on a socket.\n\n" RESET);
        return -1;
    }

    return server_socket;
}

void accept_connections(int server_socket)
{
    pthread_t tid;  

    printf(GREEN "The server is ready and waiting for connections.\n\n" RESET);
    while (1)
    {
        int *client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_socket, NULL, NULL)) != 1)
        {
            if (pthread_create(&tid, NULL, server_conversation, client_socket) == 0)
            {
                printf(GREEN "Client %d connected!\n" RESET, *client_socket);
            }
            else
            {
                fprintf(stderr, RED "Failed to connect a client.\n" RESET);
                close(*client_socket);
            }
        }
        else
        {
            fprintf(stderr, RED "Failed to accept a connection.\n" RESET);
        }
    }

    close(server_socket);
}

void *server_conversation(void *sfd)
{
    char* server_message = (char *)calloc(SERVER_MESSAGE_SIZE, sizeof(char));
    char* client_message = (char *)calloc(CLIENT_MESSAGE_SIZE, sizeof(char));

    if (server_message == NULL || client_message == NULL)
    {
        fprintf(stderr, RED "Failed to allocate memory!\n" RESET);
        close(*((int *)sfd));
        pthread_kill(pthread_self(), 0);
    }

    int random_number = create_random_number(MIN_NUMBER, MAX_NUMBER);
    int read_size = 0;

    while ((read_size = recv(*((int *)sfd), client_message, sizeof(client_message), 0)) > 0)
    {
        if (contains_numbers_only(client_message))
        {
            strcpy(server_message, messages[compare(random_number, atoi(client_message))]);
            if (random_number == atoi(client_message))
            {
                random_number = create_random_number(MIN_NUMBER, MAX_NUMBER);
            }
        }
        else
        {
            strcpy(server_message, messages[BAD_SYNTAX]);
        }

        if (send(*((int *)sfd), server_message, strlen(server_message), 0) == -1)
        {
            fprintf(stderr, RED "Failed to send the message\n\n" RESET);
        }

        memset(server_message, 0, strlen(server_message));
        memset(client_message, 0, read_size);
    }

    if (read_size == 0)
    {
        fprintf(stderr, RED "Client %d disconnected.\n" RESET, *((int *)sfd));
    }
    else if (read_size == -1)
    {
        fprintf(stderr, RED "Failed to receive the message.\n" RESET);
    }

    free(server_message);
    free(client_message);
    close(*((int *)sfd));
    pthread_kill(pthread_self(), 0);
}
