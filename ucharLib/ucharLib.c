#include "ucharLib.h"

struct ustring {
	enum ustring_type type;
	size_t index_len;
	__int8 * index;
	size_t string_len;
	uchar * string;
};

int get_uchar_len(uchar uc) {
	if ((uc ^ UTF8_0) >> 6 == 0) {
		return 0;
	}
	else if ((uc ^ UTF8_1) >> 7 == 0) {
		return 1;
	}
	else if ((uc ^ UTF8_2) >> 5 == 0) {
		return 2;
	}
	else if ((uc ^ UTF8_3) >> 4 == 0) {
		return 3;
	}
	else if ((uc ^ UTF8_4) >> 3 == 0) {
		return 4;
	}
	else {
		return 0;
	}
}

size_t init_ustring_index(struct ustring * us, size_t l) {
	if (us == NULL) {
		return -1;
	}
	else {
		if (us->index != NULL) {
			free(us->index);
			us->index_len = 0;
		}
		return update_ustring_index(us, l);
	}
}

size_t update_ustring_index(struct ustring * us, size_t l) {
	if (us == NULL) {
		return -1;
	}
	else {
		size_t i = 0;
		size_t j = 0;
		size_t sizeof_index;
		if (us->index == NULL) {
			sizeof_index = 1;
			us->index = calloc(sizeof_index, sizeof(__int8));
		}
		else {
			sizeof_index = us->index_len;
		}

		while (us->string[i] != '\0' && i < l) {
			int uclen = get_uchar_len(us->string[i]);
			if (uclen == 0) {
				return -1;
			}
			i += uclen;

			if (us->type == index) {
				if (j > sizeof_index - 1) {	//dynamic expand
					sizeof_index *= 2;
					__int8 * p = realloc(us->index, sizeof_index * sizeof(__int8));
					if (p == NULL) {
						return -1;
					}
					else
					{
						us->index = p;
					}
				}
				us->index[j] = uclen;
				++j;
			}
			else if (us->type == fenwick) {
				// fenwick tre
			}
		}
		return j;
	}
}

size_t update_ustring_len(struct ustring *us, size_t l) {
	if (us == NULL) {
		return -1;
	}
	else {
		size_t i = 0;
		while (us->string[i] != '\0' && i < l) {
			++i;
		}
		us->string_len = i;
		return i;
	}
}

size_t fprint_uchar_dex(FILE * out, uchar *s, size_t l) {
	size_t i = 0;
	while (s[i] != '\0' && i < l) {
		fprintf_s(out, "%x ", s[i]);
		++i;
	}
	fprintf_s(out, "\n");
	return i;
}

size_t fprint_uchar_len(FILE * out, uchar *s, size_t l) {
	size_t i = 0;
	size_t count = 0;
	while (s[i] != '\0' && i < l) {
		int t = get_uchar_len(s[i]);
		fprintf_s(out, "%d ", t);
		i += t;
		++count;
	}
	fprintf_s(out, "\n");
	return count;
}
