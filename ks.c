/* ks cn ionian + */

#include <stdio.h>

#include "common.h"

int
main(int argc, char *argv[])
{
	int n, m, a;

	n = clock_mod(read_note(argv[1][0]) + read_accidental(argv[1][1]), TONES);
	m = read_mode(argv[2]);
	a = read_accidental(argv[3][0]);
	printf("n = %d\tm = %d\ta = %d\n", n, m, a);
	return 0;
}
