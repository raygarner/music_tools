#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

const int MAJOR_SCALE[DEGREES] = { TONE, TONE, SEMITONE, TONE, TONE, TONE, 
                                   SEMITONE };
const char INTERVAL[2] = { 'h', 'w' };
const char *MODES[DEGREES] = { "Ionian", "Dorian", "Phrygian", "Lydian", 
                               "Mixolydian", "Aeolian", "Locrian" };
const char *NOTES[TONES] = { "Cn", "C+", "Dn", "E-", "En", "Fn", "F+", "Gn", "A-",
                             "An", "B-", "Bn" };

int
read_accidental(char a)
{
	switch (a) {
	case '+':
		return SHARP;
	case '-':
		return FLAT;
	case 'n': /* fallthrough */
	case 'x':
		return 0;
	}
	printf("Invalid input %c for accidental\n", a);
	exit(ERROR);
}

int
read_note(char n)
{
	switch (tolower(n)) {
	case 'x':
		return X;
	case 'c':
		return C;
	case 'd':
		return D;
	case 'e':
		return E;
	case 'f':
		return F;
	case 'g':
		return G;
	case 'a':
		return A;
	case 'b':
		return B;
	}
	printf("Invalid input format for note: %c\n", n);
	exit(ERROR);
}

int
read_tone(char n, char a)
{
	return clock_mod((read_note(n) + read_accidental(a)), TONES);
}

int
read_mode(const char *input)
{
	int m;
	char copy[2];

	copy[0] = toupper(input[0]);
	copy[1] = tolower(input[1]);
	for (m = 0; m < DEGREES; m++)
		if (strncmp(copy, MODES[m], 2) == 0)
			return m;
	return ERROR;
}

void
init_key_field(int key_field[TONES][DEGREES], int def)
{
	int n, m;
	
	for (n = 0; n < TONES; n++)
		for (m = 0; m < DEGREES; m++)
			key_field[n][m] = def;
}

int
clock_mod(int x, int mod)
{
	if (x == X)
		return x;
	return x < 0 ? mod - (abs(x) % mod) : x % mod;
}

int
step(int degree, int note, int mode)
{
	return (note + MAJOR_SCALE[(degree + mode) % DEGREES]) % TONES;
}

void
read_key_list(int key_field[TONES][DEGREES], int val)
{
	char c, buf[BUFLEN];
	int note, mode, i;

	while ((c = getchar()) != EOF) {
		if (isspace(c))
			continue;
		note = read_tone(c, getchar());
		scanf("%16s", buf);
		mode = read_mode(buf);
		if (note == X) {
			for (i = 0; i < TONES; i++)
				key_field[i][mode]= val;
		} else {
			key_field[note][mode] = val;
		}
	}
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

void
print_mode(int m)
{
	/*
	int d, i = MAJOR_SCALE[m];
	char c;

	for (d = 0; d < DEGREES; d++) {
		c = i == TONE ? 'w' : 'h';
		putchar(c);
		m = (m + 1) % DEGREES;
		i = MAJOR_SCALE[m];
	}
	*/
	printf("%s", MODES[m]);
}

int
is_diatonic(int note, int root, int mode)
{
	int d, cn = root;

	for (d = 0; d < DEGREES; d++) {
		if (cn == note)
			return TRUE;
		cn = step(d, cn, mode);
	}
	return FALSE;
}

int
is_accidental(int note)
{
	return !is_diatonic(note, C, IONIAN);
}

int
is_correct_accidental(int root, int mode, int accidental)
{
	int d, cn = root;

	if (clock_mod(root+mode,TONES) == C+1 || 
	clock_mod(root+mode,TONES) == F+1 || clock_mod(root+mode,TONES) == B)
		return TRUE;
	for (d = 0; d < DEGREES; d++) {
		if (is_accidental(cn) && \
		is_diatonic(clock_mod(cn+accidental*-1, TONES), root, mode) && \
		is_accidental(clock_mod(cn+accidental*-2, TONES))) {
			return FALSE;
		}
		cn = step(d, cn, mode);
	}
	return TRUE;
}
