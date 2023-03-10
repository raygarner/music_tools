#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"

#define STRINGS 6
#define OCTAVE 12
#define FRETS OCTAVE
#define EMPTY -1

enum { SIXTH, FIFTH, FOURTH, THIRD, SECOND, FIRST };

void init_string(int[FRETS]);
void print_string(int[FRETS]);
void print_fret_nums(int);
void write_string(int[FRETS], int, int);
int note_to_fret(int);

void
init_string(int string[FRETS])
{
	int f;

	for (f = 0; f < FRETS; f++)
		string[f] = -1;
}

void
print_string(int string[FRETS])
{
	int f, i;

	putchar('|');
	for (i = 0; i < 2; i++ ) {
		for (f = 0; f < FRETS; f++) {
			if (string[f] != EMPTY)
				printf(" %d|", string[f]+1);
			else
				printf("  |");
		}
	}
	putchar('\n');
}

void
print_fret_nums(int max)
{
	int f;
	
	for (f = 1; f <= max; f++)
		printf("%3d", f);
	putchar('\n');
}

void
write_string(int string[FRETS], int start, int m)
{
	int f = (start - 1) % FRETS, d = 0;

	while (string[f] == EMPTY) {
		string[f] = d;
		f = step(d, f, m);
		d = (d + 1) % DEGREES;
	}
}

int
note_to_fret(int note)
{
	/* E (4) = 0 */
	note -= 4;
	if (note < 0)
		return FRETS + note;
	else
		return note;
}

void
display_mode(int f, int m)
{
	int fretboard[STRINGS][FRETS], s;

	for (s = 0; s < STRINGS; s++)
		init_string(fretboard[s]);
	write_string(fretboard[SIXTH],  f+7*0,   m);
	write_string(fretboard[FIFTH],  f+7*1,   m);
	write_string(fretboard[FOURTH], f+7*2,   m);
	write_string(fretboard[THIRD],  f+7*3,   m);
	write_string(fretboard[SECOND], f+7*4+1, m);
	write_string(fretboard[FIRST],  f+7*5+1, m);
	for (s = FIRST; s >= SIXTH; s--)
		print_string(fretboard[s]);
	putchar('\n');
	print_fret_nums(FRETS * 2);
}

int
main(int argc, char *argv[])
{
	int fretboard[STRINGS][FRETS], s, f, m, key_field[TONES][DEGREES], n;
	char c, buf[BUFLEN];

	while ((c = getchar()) != EOF) {
		if (isspace(c))
			continue;
		n = read_tone(c, getchar());
		scanf("%10s", buf);
		m = read_mode(buf);
		display_mode(note_to_fret(n), m);
		printf("\n\n");
	}
	return 0;
}

