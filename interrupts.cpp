
#include "interrupts.h"
#include "std.h"

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];


void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interrupt,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType) {

    interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
}


InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable)
    : masterCommandPort(0x20),
      masterDataPort(0x21),
      slaveCommandPort(0xA0),
      slaveDataPort(0xA1) {
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint8_t i = 255; i > 0; --i)
    {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    masterCommandPort.Write(0x11);
    slaveCommandPort.Write(0x11);

    // remap
    masterDataPort.Write(hardwareInterruptOffset);
    slaveDataPort.Write(hardwareInterruptOffset+8);

    masterDataPort.Write(0x04);
    slaveDataPort.Write(0x02);

    masterDataPort.Write(0x01);
    slaveDataPort.Write(0x01);

    masterDataPort.Write(0x00);
    slaveDataPort.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(GateDescriptor) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

InterruptManager::~InterruptManager() {
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset() {
    return hardwareInterruptOffset;
}

void InterruptManager::Activate() {
    asm("sti");
}

void InterruptManager::Deactivate() {

}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt, uint32_t esp) {
    std::printf("INTERRUPT");

    return esp;
}
