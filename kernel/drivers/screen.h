
#define SCR_WIDTH  80
#define SCR_HEIGHT 25

extern void scr_clear();

extern void scr_putc(char c);
extern void scr_putc_at(char c, int x, int y);

extern void scr_print(char * s);
extern void scr_print_at(char * s, int x, int y);

extern void scr_set_cursor(int x, int y);
extern void scr_shift_cursor();
