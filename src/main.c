#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/network.h"
#include "../include/ui.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <username> <tcp_port>\n", "lume");
        return 1;
    }

    strncpy(app_state.local_username, argv[1], USERNAME_LEN);
    app_state.local_tcp_port = atoi(argv[2]);

    init_ui();
    init_network_threads();
    
    log_message("Welcome to Lume, %s!", app_state.local_username);
    log_message("Listening on port %d...", app_state.local_tcp_port);
    
    handle_input();
    
    cleanup_ui();
    return 0;
}
