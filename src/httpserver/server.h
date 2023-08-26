#ifndef SERVER_GUARD
#define SERVER_GUARD

#include "session.h"

typedef struct Server {
    int server_fd;
    int port;
    int max_clients_number;
    Session *sessions;
    int active_sessions;
} Server;

Server *create_server(int port, int max_connection_requests, int max_clients_number);

void stop_server(Server *server);

void start_server(Server *server);

#endif