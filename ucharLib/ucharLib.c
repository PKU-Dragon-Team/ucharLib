#include "ucharLib.h"

static size_t lowbit(size_t x) {
	return x & (~x + 1);
}

size_t fenwick_sum(const size_t index[], size_t i_index) {
	size_t sum = 0;
	size_t kt = i_index;
	size_t k = lowbit(i_index);
	while (kt > k) {
		kt -= k;
		sum += index[kt];
		k = lowbit(kt);
	}
	return sum;
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

int init_ustring(struct ustring ** us, enum ustring_type type, const uchar s[], size_t l) {
	if (us == NULL) {
		return -1;
	}
	*us = malloc(sizeof(struct ustring));
	if (*us == NULL) {
		return -1;
	}
	if (s == NULL) {
		(*us)->type = type;
		(*us)->index_len = 0;
		(*us)->index = NULL;
		(*us)->string_len = 0;
		(*us)->string = NULL;
	}
	else {
		uchar * ts = calloc(strnlen(s, l) + 1, sizeof(uchar));
		if (ts == NULL) {
			return -1;
		}
		memcpy(ts, s, strnlen(s, l) * sizeof(uchar));
		(*us)->type = type;
		(*us)->string = ts;
		(*us)->string_len = update_ustring_len(*us, l);
		(*us)->index = NULL;
		(*us)->index_len = 0;
		refresh_ustring_index(*us);
	}
	return 0;
}

int set_ustring(struct ustring * us, const uchar s[], size_t l) {
	if (us == NULL) {
		return -1;
	}
	if (us->string == NULL) {
		us->string = calloc(l + 1, sizeof(uchar));
		if (us->string == NULL) {
			return -1;
		}
	}
	else if (us->string_len < l) {
		if (resize_ustring(&us->string, l + 1) != 0) {
			return -1;
		}
	}
	us->string_len = l;
	memcpy(us->string, s, (us->string_len + 1) * sizeof(uchar));
	refresh_ustring_index(us);

	return 0;
}

int clear_ustring(struct ustring ** us) {
	if (us == NULL) {
		return -1;
	}
	if (*us != NULL) {
		if ((*us)->string != NULL) {
			free((*us)->string);
		}
		if ((*us)->index != NULL) {
			free((*us)->index);
		}
		free(*us);
		*us = NULL;
	}
	return 0;
}

size_t get_ustring_index(const struct ustring * us, size_t n) {
	if (us == NULL || n + 1 > us->index_len) {
		return 0;
	}
	if (us->type == index) {
		return us->index[n];
	}
	else if (us->type == fenwick) {
		return fenwick_sum(us->index, n) + us->index[n];
	}
	else {
		return 0;
	}
}

int compare_ustring(const struct ustring * us1, const struct ustring * us2) {
	if (us1 == NULL || us2 == NULL) {
		if (us1 > us2) {
			return 1;
		}
		else if (us1 < us2) {
			return -1;
		}
		else {
			return 0;
		}
	}
	if (us1->string == NULL || us2->string == NULL) {
		if (us1->string > us2->string) {
			return 1;
		}
		else if (us1->string < us2->string) {
			return -1;
		}
		else {
			return 0;
		}
	}
	return strcmp(us1->string, us2->string);
}

bool find_ustring(const struct ustring * us1, const struct ustring *us2) {
	if (us1 == NULL || us2 == NULL || us1->string == NULL || us2->string == NULL) {
		return false;
	}
	return strstr(us1->string, us2->string) != NULL;
}

int clone_ustring(const struct ustring *us1, struct ustring * us2) {
	if (us1 == NULL || us2 == NULL || us1->string == NULL) {
		return -1;
	}

	if (us2->string == NULL) {
		us2->string = calloc(us1->string_len + 1, sizeof(uchar));
		if (us2->string == NULL) {
			return -1;
		}
	}
	else if (us2->string_len < us1->string_len) {
		if (resize_ustring(&us2->string, us1->string_len + 1) != 0) {
			return -1;
		}
	}
	us2->string_len = us1->string_len;
	memcpy(us2->string, us1->string, (us2->string_len + 1) * sizeof(uchar));

	if (us2->index == NULL) {
		us2->index = calloc(us1->index_len, sizeof(size_t));
		if (us2->index == NULL) {
			return -1;
		}
	}
	else if (us2->index_len < us1->index_len) {
		if (resize_ustring_index(&us2->index, us1->index_len) != 0) {
			return -1;
		}
	}
	us2->index_len = us1->index_len;
	memcpy(us2->index, us1->index, us1->index_len * sizeof(size_t));

	return 0;
}

int slice_ustring(const struct ustring * us1, struct ustring * us2, size_t start, size_t end) {
	if (us1 == NULL || us2 == NULL) {
		return -1;
	}
	size_t s;
	size_t e;
	if (us2->type == index) {
		s = us1->index[start];
		e = ((end > us1->index_len) ? us1->index[us1->index_len] : us1->index[end]) - 1;
	}
	else if (us2->type == fenwick) {
		s = fenwick_sum(us1->index, start) + us1->index[start];
		e = ((end > us1->index_len) ? fenwick_sum(us1->index, us1->index_len) + us1->index[us1->index_len] : fenwick_sum(us1->index, end) + us1->index[end]) - 1;
	}
	else {
		return -1;
	}

	if (e - s + 2 > us2->string_len) {
		if (resize_ustring(&us2->string, (e - s + 2)) != 0) {
			return -1;
		}
	}

	us2->string_len = e - s + 1;
	memcpy(us2->string, us1->string + s, (e - s + 1) * sizeof(uchar));
	us2->string[us2->string_len] = '\0';
	refresh_ustring_index(us2);
	return 0;
}

int cat_ustring(const struct ustring * us1, struct ustring * us2) {
	if (us1 == NULL) {
		return -1;
	}
	return cat_partial_ustring(us1, us2, 0, us1->index_len);
}

int cat_partial_ustring(const struct ustring * us1, struct ustring * us2, size_t start, size_t end) {
	if (us1 == NULL || us2 == NULL) {
		return -1;
	}
	size_t s;
	size_t e;
	if (us2->type == index) {
		s = us1->index[start];
		e = ((end > us1->index_len) ? us1->index[us1->index_len] : us1->index[end]) - 1;
	}
	else if (us2->type == fenwick) {
		s = fenwick_sum(us1->index, start) + us1->index[start];
		e = ((end > us1->index_len) ? fenwick_sum(us1->index, us1->index_len) + us1->index[us1->index_len] : fenwick_sum(us1->index, end) + us1->index[end]) - 1;
	}
	else {
		return -1;
	}

	if (resize_ustring(&us2->string, us2->string_len + e - s + 2) != 0) {
		return -1;
	}

	memcpy(us2->string + us2->string_len, us1->string + s, (e - s + 1) * sizeof(uchar));
	us2->string_len = us2->string_len + e - s + 1;
	us2->string[us2->string_len] = '\0';
	refresh_ustring_index(us2);
	return 0;
}

// Here use the algorithm from http://stackoverflow.com/a/107657/4900529
size_t hash_ustring(const struct ustring * us, size_t seed, size_t n) {
	size_t hash = seed;
	uchar * s = us->string;
	if (s != NULL) {
		while (*s) {
			hash = hash * 101 + *s++;
		}
	}
	return hash % n;
}

size_t refresh_ustring_index(struct ustring * us) {
	return update_ustring_index(us, 0);
}

size_t update_ustring_index(struct ustring * us, size_t n) {
	if (us == NULL || n > us->index_len) {
		return 0;
	}
	// loop init
	if (us->index_len == 0) {
		us->index = calloc(1, sizeof(size_t));
		if (us->index == NULL) {
			return 0;
		}
		us->index_len = 0;
	}
	size_t index_size = us->index_len;
	size_t l = us->string_len;
	size_t i;
	size_t i_index;
	if (n == 0) {
		i = 0;
		i_index = 0;
	}
	else {
		i = us->index[n - 1];
		i_index = n - 1;
	}

	// loop body
	while (us->string[i] != '\0' && i < l) {
		int uclen = get_uchar_len(us->string[i]);
		if (uclen == 0) {
			break;
		}
		if (i_index + 1 > index_size) {	// dynamic expand
			index_size = index_size * 2 + 1;
			if (resize_ustring_index(&us->index, (index_size + 1)) != 0) {
				break;
			}
		}

		if (us->type == index) {
			// index
			us->index[i_index] = i;
		}
		else if (us->type == fenwick) {
			// fenwick tree
			size_t sum = fenwick_sum(us->index, i_index);
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
	if (us->type == index) {
		// index
		us->index[i_index] = i;
	}
	else if (us->type == fenwick) {
		// fenwick tree
		size_t sum = fenwick_sum(us->index, i_index);
		us->index[i_index] = i - sum;
	}
	else {
		// default
		return 0;
	}
	us->index_len = i_index;
	return i_index - n;
}

size_t update_ustring_len(struct ustring *us, size_t l) {
	if (us == NULL) {
		return 0;
	}
	size_t i = 0;
	while (us->string[i] != '\0' && i < l) {
		++i;
	}
	us->string_len = i;
	return i;
}

int resize_ustring(uchar ** s, size_t n) {
	uchar * p = realloc(*s, n * sizeof(uchar));
	if (p == NULL) {
		return -1;
	}
	*s = p;
	return 0;
}

int resize_ustring_index(size_t ** index, size_t n) {
	size_t * p = realloc(*index, n * sizeof(size_t));
	if (p == NULL) {
		return -1;
	}
	*index = p;
	return 0;
}

size_t fprint_uchar_dex(FILE * out, const uchar s[], size_t l) {
	size_t i = 0;
	while (s[i] != '\0' && i < l) {
		fprintf_s(out, "%x ", s[i]);
		++i;
	}
	fprintf_s(out, "\n");
	return i;
}

size_t fprint_uchar_len(FILE * out, const uchar s[], size_t l) {
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

void fprint_ustring(FILE * out, const struct ustring * us) {
	fprintf_s(out, "%d\n%llu\n", us->type, us->index_len);

	// print us->index
	size_t max = us->index_len;
	for (size_t i = 0; i < max; ++i) {
		fprintf_s(out, "%llu\t", us->index[i]);
	}

	// print us->string_len
	fprintf_s(out, "\n%llu\n", us->string_len);

	// print us->string
	fprintf_s(out, "%s\n", us->string);
}

void fprint_index(FILE * out, const struct ustring * us) {
	size_t n = us->index_len;

	for (size_t i = 0; i < n; ++i) {
		fprintf_s(out, "%llu ", get_ustring_index(us, i));
	}
	fprintf_s(out, "\n");
}