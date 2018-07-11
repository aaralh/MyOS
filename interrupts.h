
#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

    #include "globalDescriptorTable.h"
    #include "types.h"
    #include "port.h"

    class InterruptManager {
        protected:

            struct GateDescriptor {
                uint16_t handlerAddressLowBits;
                uint16_t gdt_codeSegmentSelector;
                uint8_t reserved;
                uint8_t access;
                uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer {
                uint16_t size;
                uint32_t base;
            } __attribute__((packed));

            uint16_t hardwareInterruptOffset;
       
            static void SetInterruptDescriptorTableEntry(uint8_t interrupt,
                uint16_t codeSegmentSelectorOffset, void (*handler)(),
                uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);


            static void IgnoreInterruptRequest();

            static void HandleInterruptRequest0x00(); // Interrupt for timer.
            static void HandleInterruptRequest0x01(); // Interrupt for keyboard.


            static void HandleException0x00();
            static void HandleException0x01();


            static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);

            Port8BitSlow masterCommandPort;
            Port8BitSlow masterDataPort;
            Port8BitSlow slaveCommandPort;
            Port8BitSlow slaveDataPort;

        public:
            InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
            ~InterruptManager();
            uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
    };
#endif