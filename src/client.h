#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

int init_client(unsigned short port_number, char *ip_address);
void client_conversation(int client_socket);
void get_client_input(char *number, int size);

#endif
