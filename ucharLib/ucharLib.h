#ifndef UCHARLIB
#define UCHARLIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// ----- UTF8 Masks UTF8���� -----
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
   �������ֲ�ͬ����������ö�� */
enum ustring_type {
    index, fenwick
};

// ----- struct -----

/* The struct represents the string of uchar
   ��װUTF8�ַ����Ľṹ�� */
struct ustring {
    enum ustring_type type;
    llu index_len;
    llu * index;
    llu string_len;
    uchar * string;
};

// ----- function for uchar -----

/* Get the proper length of the UTF8 character that starts with uc
   ������ uc ��ͷ��UTF8�ַ��ĳ���*/
int  get_uchar_len(uchar uc);

// ----- function for ustring -----
/* only init_ustring accepts (and only accepts) raw pointer to ustring,
   others assume the pointer to ustring have been initiated. */

/* Initiate *us with s with max length l and index_type type, return 0 if success.
   �ó������Ϊ l ���ַ��� s �� �������� type ����ʼ��ָ�� *us���ɹ�����0 */
int init_ustring(struct ustring ** us, enum ustring_type type, const uchar s[], llu l);

/* Set the content of us with s at max length l, return 0 if success.
   �� us ���������óɳ������Ϊ l ���ַ��� s���ɹ�����0 */
int set_ustring(struct ustring * us, const uchar s[], llu l);

/* Free the memory of *us, return 0 if success.
   �ͷ�ָ�� *us ���ڴ棬�ɹ�����0 */
int clear_ustring(struct ustring ** us);

/* Get the index of n-th character in us.
   ���� us �е� n ���ַ�������*/
llu get_ustring_index(const struct ustring * us, llu n);

/* Compare the two ustring, like strcmp. If either us1 or us2 is NULL, it will compare the address.
   ������ ustring ���бȽϣ���Ϊ���� strcmp (string.h)���������һ��Ϊ NULL�����Ƚ����ַ�� */
int compare_ustring(const struct ustring * us1, const struct ustring * us2);

/* Find us_sub in us_base, true if found, false otherwise.
   ���ַ��� us_base �в����ַ��� us_sub �Ƿ���� */
bool find_ustring(const struct ustring * us_base, const struct ustring * us_sub);

/* Copy us_base to us_target. Automatically calloc or realloc us1 if necessary, return 0 if success.
   ���ַ��� us_base ���ǵ� �ַ��� us_target ��ȥ�����Զ������ڴ棩���ɹ�����0 */
int clone_ustring(struct ustring * us_target, const struct ustring * us_base);

/* Copy us_base from start to end-1 and override us_target. Automatically calloc or realloc us1 if necessary, return 0 if success.
   ���ַ��� us_base ������λ�� start �� end-1 ���ǵ��ַ��� us_target ��ȥ�����Զ������ڴ棩���ɹ�����0 */
int slice_ustring(struct ustring * us_target, const struct ustring * us_base, llu start, llu end);

/* Concatenate us_base to the end of us_target. Automatically calloc or realloc if necessary, return 0 if success.
   ���ַ��� us_base ���ӵ��ַ��� us_target ��β�������Զ������ڴ棩���ɹ�����0 */
int cat_ustring(struct ustring * us_target, const struct ustring * us_base);

/* Concatenate us_base at (start, end-1) to the end of us_target. Automatically expand us2, return 0 if success.
   ���ַ��� us_base ������λ�� start �� end-1 ���ӵ��ַ��� us_target ��β�������Զ������ڴ棩���ɹ�����0 */
int cat_partial_ustring(struct ustring * us_target, const struct ustring * us_base, llu start, llu end);

/* Hash us and mod the hash_code with n
   Use the algorithm from http://stackoverflow.com/a/107657/4900529
   ����ַ��� us �Ĺ�ϣֵ������ n ȡģ */
llu hash_ustring(const struct ustring * us, llu seed, llu n);

/* Update the index of us, returns how many index units were updated.
   �����ַ��� us �������������ж��ٸ�������Ԫ�����£����Զ������ڴ棩 */
llu refresh_ustring_index(struct ustring * us);

/* Update the index after n-th index unit of us, assumes the index of 0...n are correct,
   returns how many index units were updated.
   �ӵ� n ��������Ԫ��ʼ�����ַ��� us ������������������ 0~n ����ȷ�ģ���
   �����ж��ٸ�������Ԫ�����£����Զ������ڴ棩 */
llu update_ustring_index(struct ustring * us, llu n);

/* Update the string_len of us, returns the length.
   �����ַ��� us �ĳ��� string_len�����ظó��� */
llu update_ustring_len(struct ustring *us, llu l);

/* Resize us->string to length n, return 0 if success
   ���ַ��� us �� string �ĳ��ȸ�Ϊ n ���ɹ�����0 */
int resize_ustring(uchar ** s, llu n);

/* Resize us->index to length n, return 0 if success
   ���ַ��� us �� index �ĳ��ȸ�Ϊ n ���ɹ�����0 */
int resize_ustring_index(llu ** index, llu n);

// ----- function for debug -----
llu fprint_uchar_dex(FILE * out, const uchar s[], llu l);
llu fprint_uchar_len(FILE * out, const uchar s[], llu l);
void fprint_ustring(FILE * out, const struct ustring * us);
void fprint_index(FILE * out, const struct ustring * us);

#endif