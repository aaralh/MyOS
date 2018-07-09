#ifndef __SCREEN_H
#define __SCREEN_H

#include "types.h"

static int8_t y; // Line position.
static int8_t x; // Character position at the line.

void printf(char*);
void clearScreen();


#endif