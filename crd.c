#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define TERMINAL '.'

enum { NOT_PRESENT, PRESENT };

int
main(int argc, char *argv[])
{
	int note, mode, presence[DEGREES], d, current_note, chord, i = DEGREES-1;
	char c;

	//chord = atoi(argv[1]); /* 84 tonic triad */
	/*
	while (i >= 0) {
		presence[i--] = chord % 2;
		chord /= 2;
	}
	*/
	for (i = 0; i < 7; i++)
		presence[i] = argv[1][i] == '1' ? PRESENT : NOT_PRESENT;
	while ((c = getchar()) != TERMINAL) {
		note = read_note(c) + read_accidental(getchar()) % TONES;
		getchar();
		mode = getchar() - 48;
		current_note = note;
		for (d = 0; d < DEGREES; d++) {
			if (presence[d])
				printf("%s ", NOTES[current_note]);
			current_note = (current_note + MAJOR_SCALE[(mode+d) % DEGREES]) % TONES;
		}
		putchar('\n');
		getchar();
	}
	return 0;
}
