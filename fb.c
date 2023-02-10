/*
Copyright 2023 Ray Garner

This program is free software: you can redistribute it and/or modify it under 
the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR 
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#define DEGREES 7
#define STRINGS 6
#define OCTAVE 12
#define FRETS OCTAVE
#define EMPTY -1

enum { SEMITONE = 1, TONE };

enum { SIXTH, FIFTH, FOURTH, THIRD, SECOND, FIRST };

const int IONIAN[DEGREES] = {TONE, TONE, SEMITONE, TONE, TONE, TONE, SEMITONE};

void init_string(int[FRETS]);
void print_string(int[FRETS]);
void print_fret_nums(int);
void write_string(int[FRETS], int, int);

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
		f = (f + IONIAN[(d + m) % DEGREES]) % FRETS;
		d = (d + 1) % DEGREES;
	}
}

int
main(int argc, char *argv[])
{
	int fretboard[STRINGS][FRETS], s, f, m;

	if (argc < 3) {
		printf("Please give start fret and mode as args\n");
		printf("eg: fb 5 6\n");
		return 1;
	} else {
		m = atoi(argv[2]) - 1;
		f = atoi(argv[1]);
	}
	for (s = 0; s < STRINGS; s++)
		init_string(fretboard[s]);
	write_string(fretboard[SIXTH], f+7*0, m);
	write_string(fretboard[FIFTH], f+7*1, m);
	write_string(fretboard[FOURTH], f+7*2, m);
	write_string(fretboard[THIRD], f+7*3, m);
	write_string(fretboard[SECOND], f+7*4+1, m);
	write_string(fretboard[FIRST], f+7*5+1, m);
	for (s = FIRST; s >= SIXTH; s--)
		print_string(fretboard[s]);
	putchar('\n');
	print_fret_nums(FRETS * 2);
	return 0;
}

