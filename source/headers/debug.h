#ifndef DEBUG_H
#define DEBUG_H

void debug_init(const char *pc_ip, int port);
void debug_send(const char *format, ...);
void debug_close();

#endif