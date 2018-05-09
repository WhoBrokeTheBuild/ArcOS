#ifndef CPU_IDT_H
#define CPU_IDT_H

#include <stdint.h>

void idt_set_gate(int n, uint32_t handler);
void idt_apply();

#endif // CPU_IDT_H