#ifndef __SCREEN_H
#define __SCREEN_H

#include "types.h"

class Screen
{
  private:
    static int8_t y; // Line position.
    static int8_t x; // Character position at the line.

  public:
    Screen();
    ~Screen();

    void printf(char*);
    void clearScreen();
};

#endif