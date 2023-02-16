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
	int note, mode, presence[DEGREES], d, current_note, chord, 
	    i = DEGREES-1, allowed_keys[TONES][DEGREES], n, m;
	char c, buf[BUFLEN];

	for (i = 0; i < 7; i++)
		presence[i] = argv[1][i] == '1' ? PRESENT : NOT_PRESENT;
	if (argc > 2 && argv[2][0] == '-') {
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
			scanf("%s", buf);
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
