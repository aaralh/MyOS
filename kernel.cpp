#include "types.h"
#include "globalDescriptorTable.h"
#include "screen.h"

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    Screen screen;
    screen.printf("H\ne\nl\nl\no\n \nw\no\nr\nl\nd\n!\n");
    screen.printf("\nH\ne\nl\nl\no\n!\n");
    screen.printf("H\ne\nl\nl\no w\norld!");

    screen.clearScreen();

    screen.printf("Hello World!");

    GlobalDescriptorTable gdt;


    while(1);
}