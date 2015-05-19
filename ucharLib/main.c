#include <stdio.h>
#include <locale.h>
#include "ucharLib.h"

#define BUF_SIZE 1000

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
		fprint_uchar_dex(out, buf, BUF_SIZE);
		fprint_uchar_len(out, buf, BUF_SIZE);
		fprintf_s(out, "\n");
	}

	free(buf);
	fclose(in);
	fclose(out);
	return 0;
}