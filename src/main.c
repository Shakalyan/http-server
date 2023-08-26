#include <stdio.h>
#include "httpserver/server.h"

int main() {
    
    Server *server = create_server(3333, 16, 100);
    start_server(server);

    stop_server(server);
    
    return 0;
}