#include <kernel/cpu/idt.h>

#define LOW_16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define KERNEL_CODE_SEG 0x08

#ifdef __linux__
#   define PACKED __attribute__((packed))
#else
#   define PACKED
#endif 

#define IDT_ENTRIES 256

typedef struct {
    uint16_t low_offset;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t high_offset;
} PACKED idt_gate_t;

idt_gate_t _idt[IDT_ENTRIES];

typedef struct {
    uint16_t limit;
    uint32_t base;
} PACKED idt_register_t;

idt_register_t _idt_reg;

void idt_set_gate(int index, uint32_t handler) {
    _idt[index].low_offset = LOW_16(handler);
    _idt[index].sel = KERNEL_CODE_SEG;
    _idt[index].always0 = 0;
    _idt[index].flags = 0x8E; // ???
    _idt[index].high_offset = HIGH_16(handler);
}

void idt_apply() {
    _idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    _idt_reg.base = (uint32_t)&_idt;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&_idt_reg));
}