#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define UTF8_0 0x80
#define UTF8_1 0x0
#define UTF8_2 0xC0
#define UTF8_3 0xE0
#define UTF8_4 0xF0

typedef unsigned char uchar;

struct ustring;

enum ustring_type {index, fenwick};

int  get_uchar_len(uchar uc);

size_t init_ustring_index(struct ustring * us, size_t l);
size_t update_ustring_index(struct ustring * us, size_t l);
size_t update_ustring_len(struct ustring *us, size_t l);

size_t fprint_uchar_dex(FILE * out, uchar *s, size_t l);
size_t fprint_uchar_len(FILE * out, uchar *s, size_t l);