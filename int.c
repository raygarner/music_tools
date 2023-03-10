#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

int degree_val(int, int);
int is_degree_flat(int, int);

int
degree_val(int d, int m)
{
	int n, v = 0;
	
	for (n = 0; n < d-1; n++)
		v += MAJOR_SCALE[(n+m)%DEGREES];
	return v;
}

int
is_degree_flat(int d, int m)
{
	return degree_val(d, m) < degree_val(d, IONIAN) ? TRUE : FALSE;
}

int
main(int argc, char *argv[])
{
	int m, n, i, len,
	    key_field[TONES][DEGREES];
	char tmp;
	
	if (argc > 2 && argv[2][0] == ALL_KEYS) {
		init_key_field(key_field, 0);
	} else {
		init_key_field(key_field, -999);
		read_key_list(key_field, 0);
	}
	len = strlen(argv[1]);
	for (i = 0; i < len; i++) { /* for each flattened degree */
		for (m = 0; m < DEGREES; m++) { /* for each mode */
			tmp = argv[1][i];
			if (is_degree_flat(atoi(&tmp), m)) {
				/* increment all keys of this mode */
				for (n = 0; n < TONES; n++)
					key_field[n][m]++;
			}
		}
	}
	print_matching_keys(key_field, len);
	return 0;
}
