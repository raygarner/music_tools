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

#define KEYS  12
#define TONES 7
#define WIDTH 16

enum { FALSE, TRUE };

enum { FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH };

enum { C_NATURAL, G_NATURAL, D_NATURAL, A_NATURAL, E_NATURAL, B_NATURAL,
       F_SHARP, C_SHARP, A_FLAT, E_FLAT, B_FLAT, F_NATURAL };

enum { A, B, C, D, E, F, G };

const char tone_icons[TONES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
const char *key_icons[KEYS] = {"C", "G", "D", "A", "E", "B", "F#", "C#", "Ab",
                               "Eb", "Bb", "F"};

int spacing(int, int);
void print_empty_line();
void print_key_sig(int, int[TONES]);
int *note_status(int, int);
int is_flat_key(int);
int calc_accidentals(int, int);

int
spacing(int flat_key, int tone)
{
	int search_tone = F, spaces = 0, interval;

	search_tone = flat_key ? B : F;
	interval = flat_key ? FOURTH : FIFTH;
	while (1) {
		if (search_tone == tone)
			return spaces;
		search_tone = (search_tone + interval) % TONES;
		spaces++;
	}
}

void
print_empty_line()
{
	int i;

	for (i = 0; i <= WIDTH+1; i++)
		putchar('=');
	putchar('\n');
}

void
print_key_sig(int flat_key, int altered[TONES])
{
	int line, spaces, s;
	char icon, space;
	
	icon = flat_key ? 'b' : '#';
	for (line = G; line >= A; line--) {
		spaces = spacing(flat_key, line);
		space = line % 2 == 1 ? '=' : ' ';
		putchar(space);
		for (s = 0; s < spaces; s++)
			putchar(space);
		if (altered[line])
			putchar(icon);
		else
			putchar(space);
		for (s = 0; s < (WIDTH - spaces); s++)
			putchar(space);
		putchar('\n');
	}
	print_empty_line();
	putchar('\n');
	print_empty_line();
}

int*
note_status(int flat_key, int accidentals)
{
	int *altered = malloc(sizeof(int) * TONES);
	int a, current_note, interval;

	for (a = 0; a < TONES; a++)
		altered[a] = FALSE;
	current_note = flat_key ? B : F;
	interval = flat_key ? FOURTH : FIFTH;
	for (a = 0; a < accidentals; a++) {
		altered[current_note] = TRUE;
		current_note = (current_note + interval) % TONES;
	}
	return altered;
}

int
is_flat_key(int key)
{
	return (key <= C_SHARP && key >= C_NATURAL) ? FALSE : TRUE;
}

int
calc_accidentals(int flat_key, int key)
{
	return flat_key ? KEYS - key : key;
}

int
main(int argc, char *argv[])
{
	int k, accidentals, flat_key, *altered;

	if (argc < 2) {
		printf("Enter a number for the circle: cf <n>\n");
		printf("eg: cf 5\n");
		return -1;
	}
	k = atoi(argv[1]);
	if (k < 0 || k > 11) {
		printf("Invalid key number. Choose between 0 and 11\n");
		return -1;
	}
	flat_key = is_flat_key(k);
	accidentals = calc_accidentals(flat_key, k);
	altered = note_status(flat_key, accidentals);
	printf("Key: %s major (%d%s)\n", key_icons[k], accidentals,
	                                    flat_key ? "b" : "#");
	printf("------------------\n\n");
	print_key_sig(flat_key, altered);
	free(altered);
	return 0;
}
