#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/network.h"
#include "../include/ui.h"

/*
 * Load configuration from lume.conf
 * Expected format:
 *   username=user
 *   port=8080
 *
 * Returns 1 on success (only if BOTH username and port are present),
 * 0 on failure (including when only one of them is configured).
 */
int load_config_file(char *username, int *port) {
    FILE *file = fopen("lume.conf", "r");
    if (!file) {
        return 0; // config file not found
    }

    char line[256];
    int found_username = 0;
    int found_port = 0;

    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);

        /* If the line does not end with a newline and we're not at EOF,
         * it was longer than the buffer; discard the rest of this physical line
         * so that the remainder is not treated as a separate config line.
         */
        if (len > 0 && line[len - 1] != '\n' && !feof(file)) {
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) {
                /* discard */
            }
            /* Skip parsing this overlong line */
            continue;
        }

        // Remove trailing newline, if present
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        if (strncmp(line, "username=", 9) == 0) {
            strncpy(username, line + 9, USERNAME_LEN);
            username[USERNAME_LEN - 1] = '\0';
            found_username = 1;
        } else if (strncmp(line, "port=", 5) == 0) {
            *port = atoi(line + 5);
            found_port = 1;
        }
    }

    fclose(file);
    return found_username && found_port;
}

// cppcheck-suppress constParameter
int main(int argc, char *argv[]) {
    char config_username[USERNAME_LEN];
    int config_port = 0;

    if (argc == 3) {
        // 1️⃣ Use command-line arguments
        strncpy(app_state.local_username, argv[1], USERNAME_LEN);
        app_state.local_username[USERNAME_LEN - 1] = '\0';
        app_state.local_tcp_port = atoi(argv[2]);

    } else if (load_config_file(config_username, &config_port)) {
        // 2️⃣ Fallback to config file
        strncpy(app_state.local_username, config_username, USERNAME_LEN);
        app_state.local_username[USERNAME_LEN - 1] = '\0';
        app_state.local_tcp_port = config_port;

    } else {
        // 3️⃣ Neither args nor config available
        fprintf(stderr, "Usage: %s <username> <tcp_port>\n", "lume");
        return 1;
    }

    init_ui();
    init_network_threads();

    log_message("Welcome to Lume, %s!", app_state.local_username);
    log_message("Listening on port %d...", app_state.local_tcp_port);

    handle_input();

    cleanup_ui();
    return 0;
}
