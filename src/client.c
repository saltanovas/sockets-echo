#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "helpers.h"
#include <sys/ioctl.h>

int init_client(unsigned short port_number, char *ip_address)
{
    int client_socket = 0;
    struct sockaddr_in server_address;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
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

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        fprintf(stderr, RED "Connection refused.\n\n" RESET);
        return -1;
    }
    else
    {
        printf(GREEN "Successfully connected to %s:%d\n\n" RESET, inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
        return client_socket;
    }
}

void client_conversation(int client_socket)
{
    int read_size = 0;
    char answer;
    char *number = (char *)calloc(CLIENT_MESSAGE_SIZE, sizeof(char));
    char *server_response = (char *)calloc(SERVER_MESSAGE_SIZE, sizeof(char));

    if (number == NULL || server_response == NULL)
    {
        fprintf(stderr, RED "Failed to allocate memory!\n");
        close(client_socket);
        printf("Disconnected.\n\n");
        exit(1);
    }

    printf("Try to guess the number between %d and %d: ", MIN_NUMBER, MAX_NUMBER);
    do
    {
        get_client_input(number, CLIENT_MESSAGE_SIZE);
        if(send(client_socket, number, strlen(number), 0) <= 0)
        {
            fprintf(stderr, "Failed to send the message.\n" RESET);
            break;
        }
        read_size = recv(client_socket, server_response, SERVER_MESSAGE_SIZE, 0);

        if (strcmp(server_response, messages[EQUAL]) == 0)
        {
            printf("\nCongratulations, the number was guessed right!\n");
            printf("Do you wish to restart the game? y/n: ");

            for (scanf("%c", &answer); answer != 'y' && answer != 'n'; scanf("%c", &answer))
            {
                clean_stdin();
                printf("Please enter y for yes and n for no: ");
            }

            if (answer == 'y')
            {
                printf(CLEAR_CONSOLE);
                clean_stdin();
                printf("Restarting the game.\n");
                printf("Try to guess the number between %d and %d: ", MIN_NUMBER, MAX_NUMBER);
            }
            else
            {
                printf("The game is over.\n" GREEN);
                break;
            }
        }
        else if (strcmp(server_response, messages[BAD_SYNTAX]) == 0)
        {
            printf("The server was not able to translate the message, please try again: ");
        }
        else if (read_size > 0)
        {
            printf("You have to guess a %s number: ", server_response);
        }

        memset(server_response, 0, read_size);

    } while (read_size > 0);

    if (read_size == 0)
    {
        fprintf(stderr, RED "\nThe connection with the server is lost.\n");
    }
    else if (read_size == -1)
    {
        fprintf(stderr, RED "\nFailed to receive the message.\n");
    }

    free(number);
    free(server_response);
    close(client_socket);
    printf("Disconnected.\n\n");
}

void get_client_input(char *number, int size)
{
    char *pos;

    for (fgets(number, size, stdin);
         number[strlen(number) - 1] != '\n' || !contains_numbers_only(number);
         fgets(number, size, stdin))
    {
        if ((pos = strchr(number, '\n')) != NULL)
            *pos = '\0';
        else
            clean_stdin();

        printf("Please guess the number between %d and %d: ", MIN_NUMBER, MAX_NUMBER);
    }
}
