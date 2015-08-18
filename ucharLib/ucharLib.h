#ifndef UCHARLIB
#define UCHARLIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// ----- UTF8 Masks UTF8掩码 -----
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

/* The enum using for distinguish different types of index
   用于区分不同类型索引的枚举 */
enum ustring_type {
    index, fenwick
};

// ----- struct -----

/* The struct represents the string of uchar
   封装UTF8字符串的结构体 */
struct ustring {
    enum ustring_type type;
    llu index_len;
    llu * index;
    llu string_len;
    uchar * string;
};

// ----- function for uchar -----

/* Get the proper length of the UTF8 character that starts with uc
   返回以 uc 开头的UTF8字符的长度*/
int  get_uchar_len(uchar uc);

// ----- function for ustring -----
/* only init_ustring accepts (and only accepts) raw pointer to ustring,
   others assume the pointer to ustring have been initiated. */

/* Initiate *us with s with max length l and index_type type, return 0 if success.
   用长度最大为 l 的字符串 s 和 索引类型 type 来初始化指针 *us，成功返回0 */
int init_ustring(struct ustring ** us, enum ustring_type type, const uchar s[], llu l);

/* Set the content of us with s at max length l, return 0 if success.
   将 us 的内容设置成长度最大为 l 的字符串 s，成功返回0 */
int set_ustring(struct ustring * us, const uchar s[], llu l);

/* Free the memory of *us, return 0 if success.
   释放指针 *us 的内存，成功返回0 */
int clear_ustring(struct ustring ** us);

/* Get the index of n-th character in us.
   返回 us 中第 n 个字符的索引*/
llu get_ustring_index(const struct ustring * us, llu n);

/* Compare the two ustring, like strcmp. If either us1 or us2 is NULL, it will compare the address.
   将两个 ustring 进行比较，行为类似 strcmp (string.h)，如果任意一个为 NULL，则会比较其地址。 */
int compare_ustring(const struct ustring * us1, const struct ustring * us2);

/* Find us_sub in us_base, true if found, false otherwise.
   在字符串 us_base 中查找字符串 us_sub 是否存在 */
bool find_ustring(const struct ustring * us_base, const struct ustring * us_sub);

/* Copy us_base to us_target. Automatically calloc or realloc us1 if necessary, return 0 if success.
   将字符串 us_base 覆盖到 字符串 us_target 中去（会自动分配内存），成功返回0 */
int clone_ustring(struct ustring * us_target, const struct ustring * us_base);

/* Copy us_base from start to end-1 and override us_target. Automatically calloc or realloc us1 if necessary, return 0 if success.
   将字符串 us_base 从索引位置 start 到 end-1 覆盖到字符串 us_target 中去（会自动分配内存），成功返回0 */
int slice_ustring(struct ustring * us_target, const struct ustring * us_base, llu start, llu end);

/* Concatenate us_base to the end of us_target. Automatically calloc or realloc if necessary, return 0 if success.
   将字符串 us_base 连接到字符串 us_target 的尾部（会自动分配内存），成功返回0 */
int cat_ustring(struct ustring * us_target, const struct ustring * us_base);

/* Concatenate us_base at (start, end-1) to the end of us_target. Automatically expand us2, return 0 if success.
   将字符串 us_base 从索引位置 start 到 end-1 连接到字符串 us_target 的尾部（会自动分配内存），成功返回0 */
int cat_partial_ustring(struct ustring * us_target, const struct ustring * us_base, llu start, llu end);

/* Hash us and mod the hash_code with n
   Use the algorithm from http://stackoverflow.com/a/107657/4900529
   获得字符串 us 的哈希值，并对 n 取模 */
llu hash_ustring(const struct ustring * us, llu seed, llu n);

/* Update the index of us, returns how many index units were updated.
   更新字符串 us 的索引，返回有多少个索引单元被更新（会自动分配内存） */
llu refresh_ustring_index(struct ustring * us);

/* Update the index after n-th index unit of us, assumes the index of 0...n are correct,
   returns how many index units were updated.
   从第 n 个索引单元开始更新字符串 us 的索引（假设索引的 0~n 是正确的），
   返回有多少个索引单元被更新（会自动分配内存） */
llu update_ustring_index(struct ustring * us, llu n);

/* Update the string_len of us, returns the length.
   更新字符串 us 的长度 string_len，返回该长度 */
llu update_ustring_len(struct ustring *us, llu l);

/* Resize us->string to length n, return 0 if success
   将字符串 us 的 string 的长度改为 n ，成功返回0 */
int resize_ustring(uchar ** s, llu n);

/* Resize us->index to length n, return 0 if success
   将字符串 us 的 index 的长度改为 n ，成功返回0 */
int resize_ustring_index(llu ** index, llu n);

// ----- function for debug -----
llu fprint_uchar_dex(FILE * out, const uchar s[], llu l);
llu fprint_uchar_len(FILE * out, const uchar s[], llu l);
void fprint_ustring(FILE * out, const struct ustring * us);
void fprint_index(FILE * out, const struct ustring * us);

#endif