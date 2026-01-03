#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>
#include <time.h>

#define BROADCAST_PORT 9000
#define BROADCAST_IP "255.255.255.255"
#define MAX_PEERS 50
#define USERNAME_LEN 32
#define CHUNK_SIZE 4096

typedef enum {
    MSG_TEXT,
    MSG_FILE_METADATA,
    MSG_FILE_CHUNK
} MessageType;

typedef struct {
    char username[USERNAME_LEN];
    int tcp_port;
} BeaconPacket;

typedef struct {
    char username[USERNAME_LEN];
    struct in_addr ip_addr;
    int tcp_port;
    time_t last_seen;
} Peer;

typedef struct {
    int type;
    char sender_name[USERNAME_LEN];
    size_t payload_len;
} MessagePacket;

typedef struct {
    char filename[256];
    size_t file_size;
} FileMetadata;

void init_network_threads();
void send_text_message(int peer_index, const char *msg);
void send_file(int peer_index, const char *filepath);

#endif
