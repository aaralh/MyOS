#include "screen.h"

int8_t Screen::y; // Line position.
int8_t Screen::x; // Character position at the line.
uint16_t* VideoMemory = (uint16_t*)0xb8000; // Location of text mode


Screen::Screen() {
    Screen::y = 0;
    Screen::x = 0;
}

Screen::~Screen(){
}

void Screen::printf(char* str){

    for(int i = 0; str[i] != '\0'; ++i) {

        switch(str[i]) {
            case '\n':
                x = 0;
                y++;
                break;
            
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        // Check if row is crowing over screen.
        if (x >= 80) {
            y++;
            x=0;

        }
        // If line amount exceeds height of the screen.
        if (y >= 25) {
            // Loop all lines through and move them up by one ie first line will be removed.
            for(int8_t line = 1; line < 25; line++) {
                for(int8_t character = 0; character < 80; character++) {
                    VideoMemory[80*(line-1)+character] = VideoMemory[80*(line)+character];
                }
            }
            y = y - 1;
        }
    }
}

void Screen::clearScreen(){

    for(int8_t line = 1; line < 26; line++) {
        for(int8_t character = 0; character < 80; character++) {
            VideoMemory[80*line+character] = (VideoMemory[80*line+character] & 0xFF00) | ' ';
        }
    }

    x = 0;
    y = 0;
}