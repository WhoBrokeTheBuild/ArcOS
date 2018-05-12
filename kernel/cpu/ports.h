#ifndef CPU_PORTS_H
#define CPU_PORTS_H

#include <stdint.h>

uint8_t read_port_byte(uint16_t port);
void write_port_byte(uint16_t port, uint8_t data);

uint16_t read_port_word(uint16_t port);
void write_port_word(uint16_t port, uint16_t data);

uint8_t read_byte(uint16_t seg, uint16_t offset);
void write_byte(uint16_t seg, uint16_t offset, uint8_t data);

uint16_t read_word(uint16_t seg, uint16_t offset);
void write_word(uint16_t seg, uint16_t offset, uint16_t data);

#endif // CPU_PORTS_H
