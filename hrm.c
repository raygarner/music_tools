/* hrm: harmonise
 * takes a list of notes (eg output from crd) and adds an accompany part
 * beneath it (bass voice)
 * additionally takes a key sig to use to generate the bass voice
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"

enum { THIRD = 2, FIFTH = 4, SIXTH = 5 };

int
pick_primary_chord(int mld_degree)
{
	switch (mld_degree) {
	case I: /* I or IV */
		return rand() % 2 == 0 ? I : IV;
	case II:
		return V;
	case II:
		return I;
	case IV:
		return IV;
	case V: /* I or V */
		return rand() % 2 == 0 ? I : V;
	case VI:
		return IV;
	case VII:
		return V;
	}
	return -1;
}

/* TODO: idea: binary branch everytime you encounter a I or V in the melody because
 * the bass note could be 1 of two possibilities in either case here */
Node *
generate_bass_line(Node *melody_tail, int root, int mode)
{
	Node *bass_head = NULL, *melody_note = melody_tail;
	int bass_note, x, y, dx, dy, mld_degree, i, chord, freq[DEGREES];

	for (i = 0; i < DEGREES; i++)
		freq[i] = 0;
	while (melody_note) {
		mld_degree = calc_degree(melody_note->data, root, mode);
		if (melody_note->next == NULL) {
			bass_note = root;
		} else {
			bass_note = melody_note->data;
		}
		bass_head = prepend_node(bass_head, bass_note);
		melody_note = melody_note->prev;
	}
	return bass_head;
}

int
main(int argc, char *argv[])
{
	/* while not EOF keep reading notes */
	/* linked list of notes */
	/* start with brute force algo for finding best result */

	/* read into linked list */
	/* start at end of melody */
	/* add bassline to make each beat a chord I, IV or V in either root */
	/* or first inversion */
	char c, buf[BUFLEN];
	int root = -1, mode = -1;
	Node *melody_tail = NULL, *melody_head = NULL, *bass_head = NULL;

	if (argc < 2) {
		printf("Please pass seed as arg\n");
		printf("eg: hrm 5\n");
		return 1;
	}
	srand(atoi(argv[1]));
	c = getchar();
	while (c != EOF) {
		do {
			if (isspace(c))
				continue;
			melody_tail = append_node(melody_tail, read_tone(c, getchar()));
			if (melody_head == NULL)
				melody_head = melody_tail;
		} while ((c = getchar()) != '-');
		while (isspace(c = getchar())) {}
		root = read_tone(c, getchar());
		scanf("%16s", buf);
		mode = read_mode(buf);
		bass_head = generate_bass_line(melody_tail, root, mode);
		printf("mldy: ");
		print_list(melody_head);
		printf("bass: ");
		print_list(bass_head);
		printf("%s %s\n", NOTES[root], MODES[mode]);
		getchar();
		c = getchar();
		delete_list(melody_head);
		delete_list(bass_head);
		melody_head = NULL;
		melody_tail = NULL;
		bass_head = NULL;
	}
	return 0;
}
