#define DEGREES 7
#define TONES   12
#define BUFLEN  16

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

extern const int MAJOR_SCALE[DEGREES];
extern const char INTERVAL[2];
extern const char *MODES[DEGREES];
extern const char *NOTES[TONES];

int read_accidental(char);
int read_note(char);
int read_mode(const char *);


