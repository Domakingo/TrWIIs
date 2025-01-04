#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "headers/debug.h"
#include "headers/globals.h"

static int debug_socket = -1;
static struct sockaddr_in debug_addr;

void debug_init(const char *pc_ip, int port) {
    // Initialize the network
    if (net_init() < 0) {
        return;
    }

    debug_socket = net_socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (debug_socket < 0) {
        return;
    }

    // Configure the destination address
    memset(&debug_addr, 0, sizeof(debug_addr));
    debug_addr.sin_family = AF_INET;
    debug_addr.sin_port = htons(port);
    debug_addr.sin_addr.s_addr = inet_addr(pc_ip);

    // Connect to the server
    if (net_connect(debug_socket, (struct sockaddr *)&debug_addr, sizeof(debug_addr)) < 0) {
        net_close(debug_socket);
        debug_socket = -1;
        return;
    }
}

void debug_send(const char *format, ...) {
    if (debug_socket < 0) {
        return;
    }

    char buffer[256]; // Buffer for the formatted message
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args); // Format the message
    va_end(args);

    int msg_len = strlen(buffer);
    net_send(debug_socket, buffer, msg_len, 0);
}

void debug_close() {
    if (debug_socket >= 0) {
        net_close(debug_socket);
        debug_socket = -1;
    }
    net_deinit();
}