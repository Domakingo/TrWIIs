#ifndef DEBUG_H
#define DEBUG_H

#include <gccore.h>
#include <stdarg.h> // Necessario per funzioni var-args

extern void debug_init(const char *pc_ip, int port);
extern void debug_send(const char *format, ...); // Usa var-args
extern void debug_close();

#endif
