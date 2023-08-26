#include "server.h"
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

Server *create_server(int port, int max_connection_requests, int max_clients_number) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("create_server [socket]");
        return NULL;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    socklen_t addrlen = sizeof(server_addr);
    
    int reuse_addr = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    
    if (bind(server_fd, (struct sockaddr*)&server_addr, addrlen) == -1) {
        perror("create_server [bind]");
        return NULL;
    }

    if (listen(server_fd, max_connection_requests)) {
        perror("create_server [listen]");
        return NULL;
    }

    Server *server = malloc(sizeof(Server));
    server->port = port;
    server->server_fd = server_fd;
    server->max_clients_number = max_clients_number;
    server->sessions = malloc(sizeof(Session) * max_clients_number);
    server->active_sessions = 0;

    return server;
}

static int prepare_fds(Server *server, fd_set *read_fds) {
    FD_ZERO(read_fds);
    FD_SET(server->server_fd, read_fds);
    int max_fd = server->server_fd;
    for (int i = 0; i < server->active_sessions; ++i) {
        FD_SET(server->sessions[i].socket_fd, read_fds);
        if (max_fd < server->sessions[i].socket_fd)
            max_fd = server->sessions[i].socket_fd;
    }
    return max_fd;
}

static void create_session(Server *server) {
    struct sockaddr *client_addr = NULL;
    socklen_t *client_len = NULL;
    int client_fd = accept(server->server_fd, client_addr, client_len);
    if (client_fd == -1) {
        perror("accept");
    }
    server->sessions[server->active_sessions].socket_fd = client_fd;
    ++server->active_sessions;
    printf("New session\n");
}

static void destroy_session(Server *server, int session_index) {
    --server->active_sessions;
    server->sessions[session_index] = server->sessions[server->active_sessions];
}

static void handle_request(char *buffer, unsigned long size) {
    printf("%s\n", buffer);
}

static void read_from_socket(Server *server, int session_index) {
    char read_buffer[1024];
    printf("Read...\n");
    int read_count = read(server->sessions[session_index].socket_fd, read_buffer, 1024);
    if (read_count) {
        handle_request(read_buffer, 1024);
    } else {
        destroy_session(server, session_index);
    }
}

void start_server(Server *server) {
    fd_set read_fds;
    while (1) {
        int max_fd = prepare_fds(server, &read_fds);
        printf("Await...\n");
        int count = select(max_fd+1, &read_fds, NULL, NULL, NULL);

        printf("Ready to read: %d\n", count);
        if (FD_ISSET(server->server_fd, &read_fds)) {
            create_session(server);
            --count;
        }

        for (int i = 0; i < server->active_sessions; ++i) {
            if (!count) break;
            if (FD_ISSET(server->sessions[i].socket_fd, &read_fds)) {
                read_from_socket(server, i);
                --count;
            }
        }
    }
}

void stop_server(Server *server) {
    close(server->server_fd);
    free(server->sessions);
    free(server);
}