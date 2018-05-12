#include <kernel/cpu/ports.h>

uint8_t read_port_byte(uint16_t port) {
  uint8_t result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

void write_port_byte(uint16_t port, uint8_t data) {
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t read_port_word(uint16_t port) {
  uint16_t result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

void write_port_word(uint16_t port, uint16_t data) {
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint8_t read_byte(uint16_t seg, uint16_t offset) {
    return *(uint8_t *)(seg * 16 + offset);
}

void write_byte(uint16_t seg, uint16_t offset, uint8_t data) {
    *(uint8_t *)(seg * 16 + offset) = data;
}

uint16_t read_word(uint16_t seg, uint16_t offset) {
    return *(uint16_t *)(seg * 16 + offset);
}

void write_word(uint16_t seg, uint16_t offset, uint16_t data) {
    *(uint16_t *)(seg * 16 + offset) = data;
}
