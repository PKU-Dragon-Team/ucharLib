#ifndef UCHARLIB
#define UCHARLIB

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

// ----- typedef -----
typedef unsigned char uchar;

// ----- enum -----
enum ustring_type { index, fenwick };

// ----- struct -----
struct ustring {
	enum ustring_type type;
	size_t index_len;
	size_t * index;
	size_t string_len;
	uchar * string;
};

// ----- static function -----
static size_t lowbit(size_t x);
static size_t fenwick_sum(const size_t index[], size_t i_index);

// ----- function for uchar -----
int  get_uchar_len(uchar uc);

// ----- function for ustring -----
/* only init_ustring accepts (and only accepts) raw ustring (newly malloc),
   others assume the ustring have been initiated.
   * If type of return value is int, 0 means success.
   If it is size_t, it means how many actions have done successfully.
   */

// Init `*us` with `s` with max length `l` and index type `type`
int init_ustring(struct ustring ** us, enum ustring_type type, const uchar s[], size_t l);

// Free the memory of *us
int clear_ustring(struct ustring ** us);

// Get the index of n-th character in us.
size_t get_ustring_index(const struct ustring * us, size_t n);

// Compare the two ustring, like strcmp. If either us1 or us2 is NULL, it will compare the address.
int compare_ustring(const struct ustring * us1, const struct ustring * us2);

// Find us2 in us1, true if found, false otherwise.
bool find_ustring(const struct ustring * us1, const struct ustring *us2);

// Copy us1 to us2. Automatically calloc or realloc us2 if necessary.
int clone_ustring(const struct ustring * us1, struct ustring * us2);

// Copy us1 from start to end-1 and override us2. Automatically calloc or realloc us2 if necessary.
int slice_ustring(const struct ustring * us1, struct ustring * us2, size_t start, size_t end);

// Concatenate us1 to the end of us2. Automatically calloc or realloc if necessary.
int cat_ustring(const struct ustring * us1, struct ustring * us2);

// Concatenate us1 at (start, end-1) to the end of us2. Automatically expand us2.
int cat_partial_ustring(const struct ustring * us1, struct ustring * us2, size_t start, size_t end);

// Hash a ustring and mod the hash with n
size_t hash_ustring(const struct ustring * us, size_t seed, size_t n);

// Update the index of us, returns how many index units were made.
size_t refresh_ustring_index(struct ustring * us);

/* Update the index after n-th index unit of us, assumes the index of 0...n are correct,
   returns how many index units were made. */
size_t update_ustring_index(struct ustring * us, size_t n);

// Update the string_len of us, returns the length.
size_t update_ustring_len(struct ustring *us, size_t l);

// Resize us->string to length n
int resize_ustring(uchar ** s, size_t n);

// Resize us->index to length n
int resize_ustring_index(size_t ** index, size_t n);

// ----- function for debug -----
size_t fprint_uchar_dex(FILE * out, const uchar s[], size_t l);
size_t fprint_uchar_len(FILE * out, const uchar s[], size_t l);
void fprint_ustring(FILE * out, const struct ustring * us);
void fprint_index(FILE * out, const struct ustring * us);

#endif