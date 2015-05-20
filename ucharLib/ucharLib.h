#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define UTF8_0 0x80
#define UTF8_1 0x0
#define UTF8_2 0xC0
#define UTF8_3 0xE0
#define UTF8_4 0xF0

// typedef
typedef unsigned char uchar;

// struct
struct ustring;

// enum
enum ustring_type {index, fenwick};

// static function
static size_t lowbit(size_t x);

// function for uchar
int  get_uchar_len(uchar uc);

// function for ustring
size_t init_ustring_index(struct ustring * us);
size_t update_ustring_index(struct ustring * us);
size_t update_ustring_len(struct ustring *us, size_t l);

// function for debug
size_t fprint_uchar_dex(FILE * out, uchar *s, size_t l);
size_t fprint_uchar_len(FILE * out, uchar *s, size_t l);