#ifndef DEBUG_H
#define DEBUG_H

#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <gccore.h>

#include "globals.h"

void debug_init(const char *pc_ip, int port);
void debug_send(const char *format, ...);
void debug_close();

#endif