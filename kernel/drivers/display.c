#include <kernel/drivers/display.h>
#include <kernel/cpu/ports.h>

// Attribute Controller Registers
#define VGA_ATR_INDEX 0x3C0
#define VGA_ATR_READ  0x3C1
#define VGA_ATR_WRITE 0x3C0
#define VGA_ATR_COUNT 21

// Sequencer Registers
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA  0x3C5
#define VGA_SEQ_COUNT 5

// DAC Registers
#define VGA_DAC_RD_INDEX 0x3C7
#define VGA_DAC_WR_INDEX 0x3C8
#define VGA_DAC_DATA     0x3C9

// Graphics Registers
#define VGA_GFX_INDEX 0x3CE
#define VGA_GFX_DATA  0x3CF
#define VGA_GFX_COUNT 9

// CRT Controller Registers
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA  0x3D5
#define VGA_CRTC_COUNT 25

// Misc Registers
#define VGA_INSTAT_READ 0x3DA
#define VGA_MISC_READ   0x3CC
#define VGA_MISC_WRITE  0x3C2


void _dis_set_plane(uint32_t plane) {
    uint8_t pmask;

    plane &= 3;
    pmask = 1 << p;

    // Set Read Plane
    write_port_byte(VGA_GFX_INDEX, 4);
    write_port_byte(VGA_GFX_DATA, p);

    // Set Write Plane
    write_port_byte(VGA_SEQ_INDEX, 2);
    write_port_byte(VGA_SEQ_DATAT, pmask);
}

uint32_t _dis_get_framebuf_seg() {
    uint32_t seg;

    write_port_byte(VGA_GFX_INDEX, 6);
    seg = read_port_byte(VGA_GFX_DATA);
    seg >>= 2;
    seg &= 3;

    switch (seg) {
    case 0:
    case 1:
        return 0xA000;
    case 2:
        return 0xB000;
    case 3:
        return 0xB800;
    }

    return 0;
}

void dis_init() {
    unsigned int i;

    // Write Misc Registers

    write_port_byte(VGA_MISC_WRITE, 0xE3);

    // Write Sequencer Registers

    uint8_t seq[] = { 0x03, 0x01, 0x08, 0x00, 0x06 };

    for (i = 0; i < VGA_SEQ_COUNT; ++i) {
        write_port_byte(VGA_SEQ_INDEX, i);
        write_port_byte(seq[i]);
    }

    // Unlock CRTC Registers

    write_port_byte(VGA_CRTC_INDEX, 0x03);
    write_port_byte(VGA_CRTC_DATA, read_port_byte(VGA_CRTC_DATA) | 0x80);
    write_port_byte(VGA_CRTC_INDEX, 0x11);
    write_port_byte(VGA_CRTC_DATA, read_port_byte(VGA_CRTC_DATA) | ~0x80);

    // Write CRTC Registers

    uint8_t crtc[] = { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00,
                       0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E,
                       0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF };

    for (i = 0; i < VGA_CRTC_COUNT; ++i) {
        write_port_byte(VGA_CRTC_INDEX, i);
        write_port_byte(VGA_CRTC_DATA, crtc[i]);
    }

    // Write Graphics Registers

    uint8_t gfx[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF };

    for (i = 0; i < VGA_GFX_COUNT; ++i) {
        write_port_byte(VGA_GFX_INDEX, i);
        write_port_byte(VGA_GFX_DATA, gfx[i]);
    }

    // Write Attribute Controller Registers

    uint8_t atr[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00,
                      0x0F, 0x00, 0x00 };

    for (i = 0; i < VGA_ATR_COUNT; ++i) {
        (void) read_port_byte(VGA_INSTAT_READ);

        write_port_byte(VGA_ATR_INDEX, i);
        write_port_byte(VGA_ATR_WRITE, atr[i]);
    }

    (void) read_port_byte(VGA_INSTAT_READ);
    write_port_byte(VGA_ATR_INDEX, 0x20);
}
