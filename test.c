#include <stdio.h>

#include "common.h"

int
main(int argc, char *argv[])
{
	int n, m, a;
	char mode[BUFLEN];
	//print_mode(IONIAN);
	n = read_note(getchar());
	a = read_accidental(getchar());
	n = clock_mod(n+a, TONES);
	scanf("%s", mode);
	m = read_mode(mode);
	printf("accidental = %d\n", a);
	printf("n = %d\tm = %d\n", n, m);
	//printf("%d\n", is_diatonic(11, 6, IONIAN));
	printf("%d\n", is_correct_accidental(n, m, SHARP));
	return 0;
}
