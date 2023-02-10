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
#include <string.h>
#include <stdlib.h>

#define DEGREES  7
#define TONES    12

enum { IONIAN, DORIAN, PHYRIGIAN, LYDIAN, MIXOLYDIAN, AEOLIAN, LOCRIAN };

enum {
	SEMITONE = 1,
	TONE = 2
};

enum {
	C = 0,
	D = 2,
	E = 4,
	F = 5,
	G = 7,
	A = 9,
	B = 11
};

const int MAJOR_SCALE[DEGREES] = { TONE, TONE, SEMITONE, TONE, TONE, TONE, 
                                   SEMITONE };
const char INTERVAL[2] = { 'h', 'w' };
const char *MODES[DEGREES] = { "Ionian", "Dorian", "Phrygian", "Lydian", 
                               "Mixolydian", "Aeolian", "Locrian" };
const char *NOTES[TONES] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab",
                             "A", "Bb", "B" };

int read_accidental(char);
int read_note(char);
void check_relative_modes(int, int, int[TONES][DEGREES]);
void init_key_freq(int[TONES][DEGREES]);
void print_matching_keys(const int[TONES][DEGREES], int);
int process_notes(const char[], int, int[TONES][DEGREES]);

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
	printf("Invalid input format\n");
	exit(-1);
}

int
read_note(char n)
{
	switch (n) {
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
	printf("Invalid input format\n");
	exit(-1);
}

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
init_key_freq(int key_freq[TONES][DEGREES])
{
	int n, m;
	
	for (n = 0; n < TONES; n++)
		for (m = 0; m < DEGREES; m++)
			key_freq[n][m] = 0;
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
	int key_freq[TONES][DEGREES], note_count;

	if (argc < 2) {
		printf("Please pass notes as args\n");
		printf("eg: md cnf+\n");
		return -1;
	}
	init_key_freq(key_freq);
	note_count = process_notes(argv[1], strlen(argv[1]), key_freq);
	print_matching_keys(key_freq, note_count);
	return 0;
}
