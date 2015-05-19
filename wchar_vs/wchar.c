#include <stdio.h>
//#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define BUF_SIZE 1000
#define UTF8_0 0x80
#define UTF8_1 0x0
#define UTF8_2 0xC0
#define UTF8_3 0xE0
#define UTF8_4 0xF0

typedef unsigned char uchar;

int getuclen(uchar wc) {
	if ((wc ^ UTF8_0) >> 6 == 0) {
		return 0;
	}
	else if ((wc ^ UTF8_1) >> 7 == 0) {
		return 1;
	}
	else if ((wc ^ UTF8_2) >> 5 == 0) {
		return 2;
	}
	else if ((wc ^ UTF8_3) >> 4 == 0) {
		return 3;
	}
	else if ((wc ^ UTF8_4) >> 3 == 0) {
		return 4;
	}
	else {
		return 0;
	}
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "en_US.UTF-8");

	FILE * in;
	FILE * out;
	fopen_s(&in, "in.txt", "r");
	fopen_s(&out, "out.txt", "w");

	uchar * buf = calloc(BUF_SIZE, sizeof(uchar));

	int i = 0;
	for (i = 0; i < 3; ++i) {
		fscanf_s(in, "%s", buf, BUF_SIZE);
		fprintf_s(out, "%x %x %x\n%d\n%s\n", buf[0], buf[1], buf[2], strlen(buf), buf, BUF_SIZE);
		fprintucdex(out, buf, BUF_SIZE);
		fprintuclen(out, buf, BUF_SIZE);
		fprintf_s(out, "\n");
	}


	free(buf);
	fclose(in);
	fclose(out);
	return 0;
}

int fprintucdex(FILE * out, uchar *s, int l) {
	int i = 0;
	while (s[i] != '\0' && i < BUF_SIZE) {
		fprintf_s(out, "%x ", s[i]);
		++i;
	}
	fprintf_s(out, "\n");
	return i;
}

int fprintuclen(FILE * out, uchar *s, int l) {
	int i = 0;
	int count = 0;
	while (s[i] != '\0' && i < l) {
		int t = getuclen(s[i]);
		fprintf_s(out, "%d ", t);
		i += t;
		++count;
	}
	fprintf_s(out, "\n");
	return count;
}