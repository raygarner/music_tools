#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

//#define KEYS  12
//#define TONES 7
#define WIDTH 16

enum { FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH };

enum { C_NATURAL, G_NATURAL, D_NATURAL, A_NATURAL, E_NATURAL, B_NATURAL,
       F_SHARP, C_SHARP, A_FLAT, E_FLAT, B_FLAT, F_NATURAL };

enum { B_LINE = 1, F_LINE = 5 };

const char *key_icons[TONES] = {"C", "G", "D", "A", "E", "B", "F#", "C#", "Ab",
                               "Eb", "Bb", "F"};
                      /* {1, 7, 2, 9, 4, 11, 6, 1, 8, 3, 10, 5} */
/*
const int cf[DEGREES] = {C_NATURAL, C_SHARP, D_NATURAL, E_FLAT, E_NATURAL,
                       F_NATURAL, F_SHARP, G_NATURAL, A_FLAT, A_NATURAL,
                       B_FLAT, B_NATURAL};
*/

int spacing(int, int);
void print_empty_line();
void print_key_sig(int, int[DEGREES]);
int *note_status(int, int);
int is_flat_key(int);
int calc_accidentals(int, int);

int
spacing(int flat_key, int degree)
{
	int search_tone, spaces = 0, interval;

	search_tone = flat_key ? B_LINE : F_LINE;
	interval = flat_key ? FOURTH : FIFTH;
	while (1) {
		if (search_tone == degree)
			return spaces;
		search_tone = (search_tone + interval) % DEGREES;
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
print_key_sig(int flat_key, int altered[DEGREES])
{
	int line, spaces, s;
	char icon, space;
	
	icon = flat_key ? 'b' : '#';
	for (line = DEGREES-1; line >= 0; line--) {
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
	int *altered = malloc(sizeof(int) * DEGREES);
	int a, current_note, interval;

	for (a = 0; a < DEGREES; a++)
		altered[a] = FALSE;
	current_note = flat_key ? B_LINE : F_LINE;
	interval = flat_key ? FOURTH : FIFTH;
	for (a = 0; a < accidentals; a++) {
		altered[current_note] = TRUE;
		current_note = (current_note + interval) % DEGREES;
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
	return flat_key ? TONES - key : key;
}

int
relative_ionian(int n, int m)
{
	int i;

	for (i = m; i > 0; i--) {
		n -= MAJOR_SCALE[m];
		m--;
		if (m == -1)
			m = 6;
	}
	return n < 0 ? TONES + n : n ;
}

int
note_to_cf(int n)
{
	return n % 2 == 0 ? n : (n + 6) % TONES;
}

int
main(int argc, char *argv[])
{
	int k, accidentals, flat_key, *altered, n, m, i;
	char c, buf[BUFLEN];

	if (argc < 2) {
		/*
		printf("Enter a number for the circle: cf <n>\n");
		printf("eg: cf 5\n");
		return -1;
		*/
		/* read modes from stdin here */
		while ((c = getchar()) != EOF) {
			if (isspace(c))
				continue;
			n = (read_note(c) + read_accidental(getchar())) % 12;
			printf("n = %d\n", n);
			scanf("%s", buf);
			m = read_mode(buf);
			printf("m = %d\n", m);
			printf("relative ionian %d\n", relative_ionian(n,m));
			k = note_to_cf(relative_ionian(n, m));
			printf("k = %d\n", k);
			flat_key = is_flat_key(k);
			printf("flat key = %d\n", flat_key);
			accidentals = calc_accidentals(flat_key, k);
			printf("accidentals = %d\n", accidentals);
			altered = note_status(flat_key, accidentals);
			for (i = 0; i < DEGREES; i++)
				printf("%d ", altered[i]);
			putchar('\n');
			printf("printing key signature\n");
			print_key_sig(flat_key, altered);
			free(altered);
			//display_mode(note_to_fret(n), m);
			printf("\n\n");
		}
		printf("F = %d\n", F);
		return 0;
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
