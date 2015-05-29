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
		struct ustring * us1;
		struct ustring * us2;
		struct ustring * us3;
		struct ustring * us4[BUF_SIZE];

		fprintf_s(out, "=====Line %d=====\n", i);
		fprintf_s(out, "%x %x %x\n%d\n%s\n", buf[0], buf[1], buf[2], strlen(buf), buf, BUF_SIZE);
		fprint_uchar_dex(out, buf, BUF_SIZE);
		fprint_uchar_len(out, buf, BUF_SIZE);
		fprintf_s(out, "\n");

		init_ustring(&us1, index, buf, BUF_SIZE);
		init_ustring(&us2, fenwick, buf, BUF_SIZE);
		init_ustring(&us3, index, "", BUF_SIZE);
		clone_ustring(us1, us3);
		init_ustring(&us4[0], index, "", BUF_SIZE);
		init_ustring(&us4[1], index, "Chinese", BUF_SIZE);
		init_ustring(&us4[2], index, "ooops", BUF_SIZE);
		init_ustring(&us4[3], index, "Yeeeeeah", BUF_SIZE);

		fprintf_s(out, "-----us1-----\n");
		fprint_ustring(out, us1);
		fprint_index(out, us1);
		fprintf_s(out, "\n-------------\n");

		fprintf_s(out, "-----us2-----\n");
		fprint_ustring(out, us2);
		fprint_index(out, us2);
		fprintf_s(out, "\n-------------\n");

		fprintf_s(out, "-----us3-----\n");
		fprint_ustring(out, us3);
		fprint_index(out, us3);
		fprintf_s(out, "\n-------------\n");

		fprintf_s(out, "%s\n", !compare_ustring(us1, us3) ? "Yes" : "No");

		slice_ustring(us1, us4[0], 0, 5);
		fprintf_s(out, "-----us4-----\n");
		fprint_ustring(out, us4[0]);
		fprint_index(out, us4[0]);
		fprintf_s(out, "\n-------------\n");

		fprintf_s(out, "%s\n", find_ustring(us1, us4[1]) ? "True" : "False");

		cat_ustring(us1, us4[2]);
		cat_partial_ustring(us1, us4[3], 1, 3);

		clear_ustring(&us1);
		clear_ustring(&us2);
		clear_ustring(&us3);
		for (int j = 0; j < 4; ++j) {
			clear_ustring(&us4[j]);
		};
		fprintf_s(out, "\n================\n");
		++i;
	}

	free(buf);
	fclose(in);
	fclose(out);
	return 0;
}