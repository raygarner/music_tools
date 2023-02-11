#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

int
main(int argc, char *argv[])
{
	int note, mode, presence[DEGREES], d, current_note, chord, 
	    i = DEGREES-1, allowed_keys[TONES][DEGREES], n, m;
	char c, buf[BUFLEN];

	for (i = 0; i < 7; i++)
		presence[i] = argv[1][i] == '1' ? PRESENT : NOT_PRESENT;
	/*
	if (argc > 2 && argv[2][0] == '-') {
		init_key_field(allowed_keys, PRESENT);
	} else { 
		init_key_field(allowed_keys, NOT_PRESENT);
		read_key_list(allowed_keys, PRESENT);
	}
	for (n = 0; n < TONES; n++) {
		for (m = 0; m < DEGREES; m++) {
			if (allowed_keys[n][m]) {
				current_note = n;
				for (d = 0; d < DEGREES; d++) {
					if (presence[d])
						printf("%s ", NOTES[current_note]);
					current_note = (current_note + MAJOR_SCALE[(m+d) % DEGREES]) % TONES;
				}
				putchar('\n');
			}
		}
	}
	*/
	if (argc > 2 && argv[2][0] == '-') {
		for (n = 0; n < TONES; n++) {
			for (m = 0; m < DEGREES; m++) {
				current_note = n;
				for (d = 0; d < DEGREES; d++) {
					if (presence[d])
						printf("%s ", NOTES[current_note]);
					current_note = (current_note + MAJOR_SCALE[(m+d) % DEGREES]) % TONES;
				}
				putchar('\n');
			}
		}
	} else {
		while ((c = getchar()) != EOF) {
			if (isspace(c))
				continue;
			note = (read_note(c) + read_accidental(getchar())) % TONES;
			scanf("%s", buf);
			mode = read_mode(buf);
			current_note = note;
			for (d = 0; d < DEGREES; d++) {
				if (presence[d])
					printf("%s ", NOTES[current_note]);
				current_note = (current_note + MAJOR_SCALE[(mode+d) % DEGREES]) % TONES;
			}
			putchar('\n');
		}
	}
	return 0;
}
