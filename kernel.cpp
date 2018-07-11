#include "types.h"
#include "globalDescriptorTable.h"
#include "std.h"
#include "interrupts.h"

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    interrupts.Activate();
    std::printf("Hello\n");
    while(1);
}