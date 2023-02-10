#include <stdio.h>
#include <stdlib.h>

#define DEGREES 7
#define TONES   12
#define TERMINAL '.'

enum { NOT_PRESENT, PRESENT };

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
	printf("Invalid input format for note: %c\n", n);
	exit(-1);
}

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
