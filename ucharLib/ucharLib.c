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
		us->index = NULL;
		us->index_len = 0;
		update_ustring_index(us);
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

int clone_ustring(const struct ustring *us1, struct ustring * us2) {
	if (us1 == NULL || us2 == NULL) {
		return -1;
	}

	if (us2->string == NULL) {
		us2->string = calloc(us1->string_len + 1, sizeof(uchar));
		if (us2->string == NULL) {
			return -1;
		}
	}
	else if (us2->string_len < us1->string_len) {
		uchar * temp = realloc(us2->string, (us1->string_len + 1) * sizeof(uchar));
		if (temp == NULL) {
			return -1;
		}
		us2->string = temp;
	}
	us2->string_len = us1->string_len;
	strcpy_s(us2->string, (us2->string_len + 1) * sizeof(uchar), us1->string);

	if (us2->index == NULL) {
		us2->index = calloc(us1->index_len, sizeof(size_t));
		if (us2->index == NULL) {
			return -1;
		}
	}
	else if (us2->index_len < us1->index_len) {
		size_t * temp = realloc(us2->index, us1->index_len * sizeof(size_t));
		if (temp == NULL) {
			return -1;
		}
		us2->index = temp;
	}
	us2->index_len = us1->index_len;
	memcpy(us2->index, us1->index, us1->index_len * sizeof(size_t));

	return 0;
}

int slice_ustring(const struct ustring * us1, struct ustring * us2, size_t start, size_t end) {
	if (us1 == NULL || us2 == NULL) {
		return -1;
	}
	return 0;
}

size_t update_ustring_index(struct ustring * us) {
	return update_nth_ustring_index(us, 0);
}

size_t update_nth_ustring_index(struct ustring * us, size_t n){
	if (us == NULL || n > us->index_len) {
		return 0;
	}
	// loop init
	if (us->index_len == 0) {
		us->index = calloc(1, sizeof(size_t));
		if (us->index == NULL) {
			return 0;
		}
		us->index_len = 1;
	}
	size_t i = us->index[n];
	size_t i_index = n;
	size_t index_size = us->index_len;
	size_t l = us->string_len;

	// loop body
	while (us->string[i] != '\0' && i < l) {
		int uclen = get_uchar_len(us->string[i]);
		if (uclen == 0) {
			break;
		}
		if (i_index + 1 > index_size) {	// dynamic expand
			index_size *= 2;
			size_t * p = realloc(us->index, index_size * sizeof(size_t));
			if (p == NULL) {
				break;
			}
			us->index = p;
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
			return 0;
		}

		// loop update
		i += uclen;
		++i_index;
	}
	us->index_len = index_size;
	return i_index - n + 1;
}

size_t update_ustring_len(struct ustring *us, size_t l) {
	if (us == NULL) {
		return 0;
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

size_t fprint_uchar_dex(FILE * out, const uchar *s, size_t l) {
	size_t i = 0;
	while (s[i] != '\0' && i < l) {
		fprintf_s(out, "%x ", s[i]);
		++i;
	}
	fprintf_s(out, "\n");
	return i;
}

size_t fprint_uchar_len(FILE * out, const uchar *s, size_t l) {
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

void fprint_ustring(FILE * out, const struct ustring us) {
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