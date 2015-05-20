#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define UTF8_0 0x80		// 1000 0000
#define UTF8_1 0x0		// 0000 0000
#define UTF8_2 0xC0		// 1100 0000
#define UTF8_3 0xE0		// 1110 0000
#define UTF8_4 0xF0		// 1111 0000

// typedef
typedef unsigned char uchar;

// enum
enum ustring_type { index, fenwick };

// struct
struct ustring;

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
size_t fprint_ustring(FILE * out, struct ustring us);