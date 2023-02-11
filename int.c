#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

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
	int m, target = 3, n, v = 0, modes[DEGREES], i, len, d;
	char tmp;
	
	len = strlen(argv[1]);
	for (m = 0; m < DEGREES; m++)
		modes[m] = 0;
	for (i = 0; i < len; i++) {
		for (m = 0; m < DEGREES; m++) {
			tmp = argv[1][i];
			if (is_degree_flat(atoi(&tmp), m))
				modes[m]++;
		}
	}
	for (m = 0; m < DEGREES; m++) {
		if (modes[m] == len)
			printf("x %s\n", MODES[m]);
	}
	return 0;
}
