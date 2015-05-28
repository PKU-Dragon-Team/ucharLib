#include <stdio.h>
#include <locale.h>
#include "ucharLib.h"

#define BUF_SIZE BUFSIZ

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "en_US.UTF-8");

	FILE * in;
	FILE * out;
	fopen_s(&in, "in.txt", "r");
	fopen_s(&out, "out.txt", "w");

	uchar * buf = calloc(BUF_SIZE, sizeof(uchar));

	int i = 0;
	while (fscanf_s(in, "%s", buf, BUF_SIZE) != EOF) {
		fprintf_s(out, "=====Line %d=====\n", i);
		fprintf_s(out, "%x %x %x\n%d\n%s\n", buf[0], buf[1], buf[2], strlen(buf), buf, BUF_SIZE);
		fprint_uchar_dex(out, buf, BUF_SIZE);
		fprint_uchar_len(out, buf, BUF_SIZE);
		fprintf_s(out, "\n\n");
		struct ustring * us1 = malloc(sizeof(struct ustring));
		init_ustring(us1, index, buf, BUF_SIZE);
		struct ustring * us2 = malloc(sizeof(struct ustring));
		init_ustring(us2, fenwick, buf, BUF_SIZE);
		us2->type = fenwick;
		refresh_ustring_index(us2);
		fprint_ustring(out, us1);
		fprint_ustring(out, us2);
		fprint_index(out, us1);
		fprint_index(out, us2);

		clear_ustring(us1);
		free(us1);
		clear_ustring(us2);
		free(us2);
		fprintf_s(out, "=================\n");
		++i;
	}

	free(buf);
	fclose(in);
	fclose(out);
	return 0;
}