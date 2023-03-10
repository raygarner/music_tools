#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

void
apply_bitfield(const int presence[DEGREES], int m, int n)
{
	int d;

	for (d = 0; d < DEGREES; d++) {
		if (presence[d])
			printf("%s ", NOTES[n]);
		n = step(d, n, m);
	}
}

int
main(int argc, char *argv[])
{
	int note, mode, presence[DEGREES], i = DEGREES-1, n, m;
	char c, buf[BUFLEN];

	if (argc < 2) {
		printf("Please pass note bitfied as arg\n");
		printf("eg: crd 1010100 (for tonic triad)\n");
		return 1;
	}
	for (i = 0; i < 7; i++)
		presence[i] = argv[1][i] == '1' ? PRESENT : NOT_PRESENT;
	if (argc > 2 && argv[2][0] == ALL_KEYS) {
		for (n = 0; n < TONES; n++) {
			for (m = 0; m < DEGREES; m++) {
				apply_bitfield(presence, m, n);
				putchar('\n');
			}
		}
	} else {
		while ((c = getchar()) != EOF) {
			if (isspace(c))
				continue;
			note = read_tone(c, getchar());
			scanf("%16s", buf);
			mode = read_mode(buf);
			if (note == X)
				for (note = 0; note < TONES; note++) {
					apply_bitfield(presence, mode, note);
					putchar('\n');
				}
			else
				apply_bitfield(presence, mode, note);
			putchar('\n');
		}
	}
	return 0;
}
