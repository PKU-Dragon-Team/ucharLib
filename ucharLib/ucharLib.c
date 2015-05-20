#include "ucharLib.h"

struct ustring {
	enum ustring_type type;
	size_t index_len;
	size_t * index;
	size_t string_len;
	uchar * string;
};

static size_t lowbit(size_t x) {
	return x & (~x + 1);
}

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

int init_ustring(struct ustring * us, enum ustring_type type, uchar *s, size_t l) {
	if (us == NULL) {
		return -1;
	}
	if (s == NULL) {
		us->type = type;
		us->index_len = 0;
		us->index = NULL;
		us->string_len = 0;
		us->string = NULL;
	}
	else {
		us->type = type;
		us->string = s;
		us->string_len = update_ustring_len(us, l);
		size_t check = init_ustring_index(us);
		if (check < 0) {
			return -1;
		}
	}
	return 0;
}

int clear_ustring(struct ustring * us) {
	if (us == NULL) {
		return -1;
	}
	us->string_len = 0;
	if (us->string != NULL) {
		free(us->string);
	}
	us->index_len = 0;
	if (us->index != NULL) {
		free(us->index);
	}
	return 0;
}

size_t init_ustring_index(struct ustring * us) {
	if (us == NULL) {
		return -1;
	}
	else {
		if (us->index != NULL) {
			free(us->index);
			us->index_len = 0;
		}
		return update_ustring_index(us);
	}
}

size_t update_ustring_index(struct ustring * us) {
	if (us == NULL) {
		return -1;
	}
	else {
		// loop init
		size_t i = 0;
		size_t i_index = 0;
		size_t index_size;
		size_t l = us->string_len;

		if (us->index == NULL) {
			index_size = 1;
			us->index = calloc(index_size, sizeof(size_t));
		}
		else {
			index_size = us->index_len;
		}

		// loop body
		while (us->string[i] != '\0' && i < l) {
			int uclen = get_uchar_len(us->string[i]);
			if (uclen == 0) {
				return -1;
			}
			if (i_index > index_size - 1) {	// dynamic expand
				index_size *= 2;
				size_t * p = realloc(us->index, index_size * sizeof(size_t));
				if (p == NULL) {
					return -1;
				}
				else
				{
					us->index = p;
				}
			}

			if (us->type == index) {
				// index
				us->index[i_index] = i;
			}
			else if (us->type == fenwick) {
				// fenwick tree
				size_t sum = 0;
				size_t kt = i_index;
				size_t k = lowbit(i_index);
				while (kt > k) {
					kt -= k;
					sum += us->index[kt];
					k = lowbit(kt);
				}
				us->index[i_index] = i - sum;
			}
			else {
				// default
				return -1;
			}

			// loop update
			i += uclen;
			++i_index;
		}
		us->index_len = index_size;
		return i_index;
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
		if (t == 0) {
			return -1;
		}
		fprintf_s(out, "%d ", t);
		i += t;
		++count;
	}
	fprintf_s(out, "\n");
	return count;
}

void fprint_ustring(FILE * out, struct ustring us) {
	fprintf_s(out, "%d\n%d\n", us.type, us.index_len);

	// print us.index
	size_t max = us.index_len;
	size_t i = 0;
	for (i = 0; i < max; ++i) {
		fprintf_s(out, "%d\t", us.index[i]);
	}

	// print us.string_len
	fprintf_s(out, "\n%d\n", us.string_len);

	// print us.string
	fprintf_s(out, "%s\n", us.string, us.string_len);
}