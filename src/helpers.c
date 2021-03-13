#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include "helpers.h"

const char *messages[] =
{
    [EQUAL] = "equal",
    [SMALLER] = "smaller",
    [BIGGER] = "bigger",
    [BAD_SYNTAX] = "Message contains bad syntax."
};

void clean_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        continue;
}

int contains_numbers_only(char *s)
{
    while (*s != '\0' && *s != '\n' && *s != '\r')
        if (!isdigit(*s++))
            return 0;
    return 1;
}

int create_random_number(int lower, int upper)
{
    srand(time(0));
    return (rand() % (upper - lower + 1)) + lower;
}

int compare(int server_number, int client_number)
{
    if (client_number < server_number)
        return BIGGER;
    else if (client_number > server_number)
        return SMALLER;
    else
        return EQUAL;
}

int client_msg_size()
{
    return floor(log10(abs(MAX_NUMBER))) + 3;
}

int is_port_valid(char *port)
{
    return contains_numbers_only(port) && atoi(port) > 0 && atoi(port) <= USHRT_MAX;
}
