#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CLEAR_CONSOLE "\e[1;1H\e[2J"

#define MIN_NUMBER 1
#define MAX_NUMBER 9999

#define PORT 4125
#define IP_ADDRESS "127.0.0.1"
#define QUEUE_LENGTH 10

#define CLIENT_MESSAGE_SIZE client_msg_size()
#define SERVER_MESSAGE_SIZE 30

enum values
{
    EQUAL = 0,
    SMALLER = 1,
    BIGGER = 2,
    BAD_SYNTAX = 3
};

const char *messages[4];

void clean_stdin(void);
int contains_numbers_only(char *s);
int client_msg_size();
int create_random_number(int lower, int upper);
int compare(int server_number, int client_number);
int is_port_valid(char *port);

#endif
