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
const char *NOTES[TONES] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab",
                             "A", "Bb", "B" };

int
read_accidental(char a)
{
	switch (a) {
	case '+':
		return 1;
	case '-':
		return -1;
	case 'n':
		return 0;
	}
	printf("Invalid input format for accidental\n");
	exit(-1);
}

int
read_note(char n)
{
	switch (tolower(n)) {
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
	exit(-1);
}

int
read_mode(const char *input)
{
	int m;

	for (m = 0; m < DEGREES; m++) {
		if (strcmp(input, MODES[m]) == 0)
			return m;
	}
}
