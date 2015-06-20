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
typedef unsigned long long llu;
typedef long long lld;

// ----- enum -----
enum ustring_type {
	index, fenwick
};

// ----- struct -----
struct ustring {
	enum ustring_type type;
	llu index_len;
	llu * index;
	llu string_len;
	uchar * string;
};

// ----- static function -----
static llu lowbit(llu x);
static llu fenwick_sum(const llu index[], llu i_index);
static void free_if_not_null(void * p);

// ----- function for uchar -----
int  get_uchar_len(uchar uc);

// ----- function for ustring -----
/* only init_ustring accepts (and only accepts) raw ustring (newly malloc),
 * others assume the ustring have been initiated.
 * If type of return value is int, 0 means success.
 * If it is llu, it means how many actions have done successfully.
 */

// Init `*us` with `s` with max length `l` and index type `type`
int init_ustring(struct ustring ** us, enum ustring_type type, const uchar s[], llu l);

// Set us's content with s at max length l;
int set_ustring(struct ustring * us, const uchar s[], llu l);

// Free the memory of *us
int clear_ustring(struct ustring ** us);

// Get the index of n-th character in us.
llu get_ustring_index(const struct ustring * us, llu n);

// Compare the two ustring, like strcmp. If either us1 or us2 is NULL, it will compare the address.
int compare_ustring(const struct ustring * us1, const struct ustring * us2);

// Find us2 in us1, true if found, false otherwise.
bool find_ustring(const struct ustring * us1, const struct ustring *us2);

// Copy us1 to us2. Automatically calloc or realloc us2 if necessary.
int clone_ustring(const struct ustring * us1, struct ustring * us2);

// Copy us1 from start to end-1 and override us2. Automatically calloc or realloc us2 if necessary.
int slice_ustring(const struct ustring * us1, struct ustring * us2, llu start, llu end);

// Concatenate us1 to the end of us2. Automatically calloc or realloc if necessary.
int cat_ustring(const struct ustring * us1, struct ustring * us2);

// Concatenate us1 at (start, end-1) to the end of us2. Automatically expand us2.
int cat_partial_ustring(const struct ustring * us1, struct ustring * us2, llu start, llu end);

/* Hash us and mod the hashcode with n
 * Use the algorithm from http://stackoverflow.com/a/107657/4900529
 */
llu hash_ustring(const struct ustring * us, llu seed, llu n);

// Update the index of us, returns how many index units were made.
llu refresh_ustring_index(struct ustring * us);

/* Update the index after n-th index unit of us, assumes the index of 0...n are correct,
   returns how many index units were made. */
llu update_ustring_index(struct ustring * us, llu n);

// Update the string_len of us, returns the length.
llu update_ustring_len(struct ustring *us, llu l);

// Resize us->string to length n
int resize_ustring(uchar ** s, llu n);

// Resize us->index to length n
int resize_ustring_index(llu ** index, llu n);

// ----- function for debug -----
llu fprint_uchar_dex(FILE * out, const uchar s[], llu l);
llu fprint_uchar_len(FILE * out, const uchar s[], llu l);
void fprint_ustring(FILE * out, const struct ustring * us);
void fprint_index(FILE * out, const struct ustring * us);

#endif