#include <string.h>
#include <stdint.h>

size_t strlen(const char * str) {
    size_t i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

int strcmp(const char * str1, const char * str2) {
  int i;
  for (i = 0; str1[i] == str2[i]; i++) {
    if (str1[i] == '\0') return 0;
  }
  return str1[i] - str2[i];
}

void * memcpy(void * dest, void * src, size_t len) {
    while (len--) *(uint8_t *)dest = *(uint8_t *)src;
    return dest;
}
