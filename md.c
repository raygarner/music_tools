#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"

void check_relative_modes(int, int, int[TONES][DEGREES]);
void init_key_freq(int[TONES][DEGREES], int);
void print_matching_keys(const int[TONES][DEGREES], int);
int process_notes(const char[], int, int[TONES][DEGREES]);

void
check_relative_modes(int tonic, int mode, int key_freq[TONES][DEGREES])
{
	int current_note = tonic, m;
	
	for (m = 0; m < DEGREES; m++) {
		key_freq[current_note][(m+mode)%DEGREES]++;
		current_note = \
		    (current_note + MAJOR_SCALE[(m+mode)%DEGREES]) % TONES;
	}
}

void
init_key_freq(int key_freq[TONES][DEGREES], int def)
{
	int n, m;
	
	for (n = 0; n < TONES; n++)
		for (m = 0; m < DEGREES; m++)
			key_freq[n][m] = def;
}

void
print_matching_keys(const int key_freq[TONES][DEGREES], int len)
{
	int n, m;

	for (n = 0; n < TONES; n++) {
		for (m = 0; m < DEGREES; m++) {
			if (key_freq[n][m] == len)
				printf("%2s %s\n", NOTES[n], MODES[m]);
		}
	}
}

int
process_notes(const char notes[], int len, int key_freq[TONES][DEGREES])
{
	int note_count = 0, note, m, i = 0;

	while (i < len) {
		note = (read_note(notes[i]) + read_accidental(notes[i+1])) % TONES;
		note_count++;
		for (m = 0; m < DEGREES; m++)
			check_relative_modes(note, m, key_freq);
		i += 2;
	}
	return note_count;
}

int
main(int argc, char *argv[])
{
	int key_freq[TONES][DEGREES], note_count, note, mode;
	char c, buf[BUFLEN];

	if (argc < 2) {
		printf("Please pass notes as args\n");
		printf("eg: md cnf+\n");
		return -1;
	}
	if (argc > 2 && argv[2][0] == '-') {
		init_key_freq(key_freq, 0); /* all keys are allowed */
	} else {
		init_key_freq(key_freq, -999);
		while ((c = getchar()) != EOF) {
			if (isspace(c))
				continue;
			printf("c = %c\n", c);
			note = read_note(c);
			scanf("%s", buf);
			printf("buf = %s\n", buf);
			mode = read_mode(buf);
			key_freq[note][mode] = 0; /* mark this mode as allowed */
		}
	}
	note_count = process_notes(argv[1], strlen(argv[1]), key_freq);
	print_matching_keys(key_freq, note_count);
	return 0;
}
