#define DEGREES 7
#define TONES   12
#define BUFLEN  16
#define ERROR   -1
#define ALL_KEYS '-'

enum { NOT_PRESENT, PRESENT };

enum { IONIAN, DORIAN, PHYRIGIAN, LYDIAN, MIXOLYDIAN, AEOLIAN, LOCRIAN };

enum {
	SEMITONE = 1,
	TONE = 2
};

enum { FALSE, TRUE };

enum {
	X = -999, /* wildcard */
	C = 0,
	D = 2,
	E = 4,
	F = 5,
	G = 7,
	A = 9,
	B = 11
};

enum { DOWN = -1, UP = 1 };

enum { FLAT = -1, SHARP = 1};

typedef struct Node Node;
struct Node {
	int data;
	Node *next;
};

extern const int MAJOR_SCALE[DEGREES];
extern const char *MODES[DEGREES];
extern const char *NOTES[TONES];

int read_accidental(char);
int read_note(char);
int read_tone(char, char);
int read_mode(const char *);
int clock_mod(int, int);
int step(int, int, int);
void init_key_field(int[TONES][DEGREES], int);
void read_key_list(int[TONES][DEGREES], int);
void print_matching_keys(const int[TONES][DEGREES], int);
void print_mode(int);
int is_diatonic(int, int, int);
int is_accidental(int);
int is_correct_accidental(int, int, int);
Node *prepend_node(Node *head);
Node *pop_head(Node *head);
