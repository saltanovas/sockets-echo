#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

int init_server(unsigned short port_number, char *ip_address);
void accept_connections(int server_socket);
void *server_conversation(void *sfd);

#endif
