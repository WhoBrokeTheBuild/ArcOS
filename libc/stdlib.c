#include <stdlib.h>
#include <string.h>

char * itoa(int value, char * str, int base) {
    int i, j, sign;
    char c;

    if ((sign = value) < 0) value = -value;
    
    i = 0;
    do {
        j = value % base;
        str[i++] = j + (j >= 10 ? '7' : '0');
    } while((value /= base) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    for (i = 0, j = strlen(str) -1; i < j; ++i, --j) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    return str;
}