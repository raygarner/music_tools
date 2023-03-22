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
enum { FOURTH_TONES = 5, FIFTH_TONES = 7 };

void improve_bass_line(Node *, Node *, int, int);

/* is degree I, IV or V */
int
is_primary_degree(int note, int root, int mode)
{
	switch (calc_degree(note, root, mode)) {
	case I: /* fallthrough */
	case IV: /* fallthrough */
	case V: /* fallthrough */
		return TRUE;
	}
	return FALSE;
}

/* TODO: do the same searh used on bass improvement */
int
add_middle_note(Node *bass_note, Node *mld_note, int root, int mode)
{
	/* calculate chord */
	/* branch for adding 1st, 3rd or 5th */
	/* evaluate based on smoothness */
	int chord_root, chord_degree, inverted;

	chord_degree = calc_degree(bass_note->data, root, mode);
	if (is_primary_degree(bass_note->data, root, mode)) {
		chord_root = bass_note->data;
		inverted = FALSE;
	} else {
		chord_root = apply_steps(chord_degree, mode, bass_note->data, -THIRD);
		inverted = TRUE;
	}
	/* if root and fifth then mid must be 3rd */
	if ((inverted == FALSE) && (mld_note->data == (bass_note->data + FIFTH_TONES) % TONES)) {
		return apply_steps(chord_degree, mode, chord_root, THIRD);
	}
	/* if root and third then mid must be 5th or 1st*/
	if ((inverted == FALSE) && (mld_note->data == apply_steps(chord_degree, mode, chord_root, THIRD))) {
		/* TODO: branch here (fifth or first) */
		return (bass_note->data + FIFTH_TONES) % TONES;
	}
	/* if both first then mid must be 3rd */
	if ((inverted == FALSE) && mld_note->data == bass_note->data) {
		return apply_steps(chord_degree, mode, chord_root, THIRD);
	}
	/* if third and fifth then mid must be first */
	if ((inverted == TRUE) && (mld_note->data == (chord_root + FIFTH_TONES) % TONES)) {
		return chord_root;
	}
	/* if third and third then mid must be first */
	if ((inverted == TRUE) && (mld_note->data == bass_note->data)) {
		return chord_root;
	}
	/* if third and first then mid must be fifth or first */
	if ((inverted == TRUE) && mld_note->data == chord_root) {
		/* TODO: branch here */
		return (bass_note->data + FIFTH_TONES) % TONES;
	}
	return ERROR;
}

Node *
generate_middle_line(Node *bass_head, Node *mld_head, int root, int mode)
{
	/* check if bass note is primary degree */
	/* if it is then add third if melody is octave or fifth */
	/* if melody is third then add fifth or octave (whichever is nearer to prev) */
	Node *mid_head = NULL, *mid_tail = NULL;
	int mid_note;

	while (bass_head && mld_head) {
		mid_note = add_middle_note(bass_head, mld_head, root, mode);
		mid_tail = append_node(mid_tail, mid_note);
		if (mid_head == NULL)
			mid_head = mid_tail;
		bass_head = bass_head->next;
		mld_head = mld_head->next;
	}
	return mid_head;
}

int
pick_primary_chord(int mld_degree, const int freq[DEGREES])
{
	switch (mld_degree) {
	case I: /* I or IV */
		if (freq[I] < freq[IV])
			return I;
		else if (freq[IV] < freq[I])
			return IV;
		else
			return rand() % 2 ? I : IV;
	case II:
		return V;
	case III:
		return I;
	case IV:
		return IV;
	case V: /* I or V */
		if (freq[I] < freq[V])
			return I;
		else if (freq[V] < freq[I])
			return V;
		else
			return rand() % 2 ? I : V;
	case VI:
		return IV;
	case VII:
		return V;
	}
	return -1;
}

/* repeated notes */
/* parallel octaves or 5ths */
/* double leap */
/* smaller number is better */
int
faulty_note(Node *bass_note, Node *mld_note)
{
	int fault = 0;

	if (!(bass_note->prev && mld_note->prev))
		return 0;
	/* if consecutive bass notes */
	if (bass_note->prev->data == bass_note->data)
		fault += 1;
	/* parallel octaves */
	if (bass_note->data == mld_note->data && bass_note->prev->data == mld_note->prev->data)
		fault += 1;
	/* paralell fifths */
	if ((mld_note->data == (bass_note->data + FIFTH_TONES) % TONES) && \
	(mld_note->prev->data == (bass_note->prev->data + FIFTH_TONES) % TONES))
		fault += 1;
	/* leaps of a fourth or greater */
	if (abs(min_tone_diff(bass_note->data, bass_note->prev->data)) >= FOURTH_TONES)
		fault += 1;
	/* double leaps */
	if (bass_note->prev->prev && \
	abs(min_tone_diff(bass_note->data, bass_note->prev->data)) > TONE && \
	abs(min_tone_diff(bass_note->prev->data, bass_note->prev->prev->data)) > TONE)
		fault += 1;
	return fault;
}

int
count_faults(Node *bass_note, Node *mld_note)
{
	int c = 0;

	while (bass_note && mld_note) {
		c += faulty_note(bass_note, mld_note);
		bass_note = bass_note->prev;
		mld_note = mld_note->prev;
	}
	return c;
}

int
alt_chord_choice(int chord, int mld_degree)
{
	switch (mld_degree) {
	case I:
		return chord == I ? IV : I;
	case V:
		return chord == I ? V : I;
	}
	return chord;
}

/* 3 choices: */
/* a) use different chord */
/* b) invert chord */
/* c) use different chord and invert */
/* TODO: valgrind this */
int
improve_bass_note(Node *bass_note, Node *mld_note, int root, int mode)
{
	int tmp_note, mld_degree, old_bass_degree, new_bass_degree;
	Node *bass_alt_head_a = NULL;
	Node *bass_alt_head_b = NULL;
	Node *bass_alt_head_c = NULL;
	int a_faults, b_faults, c_faults, base_faults;
	int ret = 0;

	if (bass_note->next == NULL || mld_note->next == NULL)
		return count_faults(bass_note, mld_note);
	tmp_note = bass_note->data;
	mld_degree = calc_degree(mld_note->data, root, mode);
	old_bass_degree = calc_degree(bass_note->data, root, mode);
	bass_alt_head_a = copy_list(bass_note);
	bass_alt_head_b = copy_list(bass_note);
	bass_alt_head_c = copy_list(bass_note);
	bass_alt_head_a->prev = bass_note->prev;
	bass_alt_head_b->prev = bass_note->prev;
	bass_alt_head_c->prev = bass_note->prev;

	/* try different chord */
	new_bass_degree = alt_chord_choice(old_bass_degree, mld_degree);
	bass_alt_head_a->data = apply_steps(0, mode, root, new_bass_degree);
	a_faults = improve_bass_note(bass_alt_head_a->next, mld_note->next, root, mode);

	/* try inverting different chord */
	bass_alt_head_b->data = apply_steps(new_bass_degree, mode, bass_alt_head_a->data, THIRD);
	b_faults = improve_bass_note(bass_alt_head_b->next, mld_note->next, root, mode);

	/* try inverting original chord */
	bass_alt_head_c->data = apply_steps(old_bass_degree, mode, tmp_note, THIRD);
	c_faults = improve_bass_note(bass_alt_head_c->next, mld_note->next, root, mode);

	/* base faults */
	base_faults = improve_bass_note(bass_note->next, mld_note->next, root, mode);

	if (base_faults <= a_faults && base_faults <= b_faults && base_faults <= c_faults) {
		bass_note->data = tmp_note;
		ret = base_faults;
	} else if (a_faults <= base_faults && a_faults <= b_faults && a_faults <= c_faults) {
		bass_note->data = bass_alt_head_a->data;
		ret = a_faults;
	} else if (b_faults <= base_faults && b_faults <= a_faults && b_faults <= c_faults) {
		bass_note->data = bass_alt_head_b->data;
		ret = b_faults;
	//} else if (c_faults <= base_faults && c_faults <= a_faults && c_faults <= b_faults) {
	} else {
		bass_note->data = bass_alt_head_c->data;
		ret = c_faults;
	}
	delete_list(bass_alt_head_a);
	delete_list(bass_alt_head_b);
	delete_list(bass_alt_head_c);
	return ret;
}

void
improve_bass_line(Node *bass_head, Node *mld_head, int root, int mode)
{
	while (bass_head && mld_head) {
		improve_bass_note(bass_head, mld_head, root, mode);
		bass_head = bass_head->next;
		mld_head = mld_head->next;
	}
}

Node *
generate_bass_line(Node *melody_tail, Node *bass_tail, int root, int mode)
{
	Node *bass_head = NULL, *melody_note = melody_tail;
	int bass_note, mld_degree, i, chord, freq[DEGREES];

	for (i = 0; i < DEGREES; i++)
		freq[i] = 0;
	while (melody_note) {
		mld_degree = calc_degree(melody_note->data, root, mode);
		if (melody_note->next == NULL) {
			bass_note = root;
		} else {
			chord = pick_primary_chord(mld_degree, freq);
			bass_note = apply_steps(0, mode, root, chord);
			freq[chord]++;
		}
		bass_head = prepend_node(bass_head, bass_note);
		if (bass_tail == NULL)
			bass_tail = bass_head;
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
	Node *melody_tail = NULL, *melody_head = NULL, *bass_head = NULL,
	     *bass_tail = NULL, *mid_head = NULL;

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
		bass_head = generate_bass_line(melody_tail, bass_tail, root, mode);
		//improve_bass_line(bass_head, melody_head, root, mode);
		improve_bass_note(bass_head, melody_head, root, mode);
		mid_head = generate_middle_line(bass_head, melody_head, root, mode);
		printf("mldy: ");
		print_list(melody_head);
		//printf("midl: ");
		//print_list(mid_head);
		printf("bass: ");
		print_list(bass_head);
		printf("%s %s\n", NOTES[root], MODES[mode]);
		getchar();
		c = getchar();
		delete_list(melody_head);
		delete_list(bass_head);
		delete_list(mid_head);
		melody_head = NULL;
		melody_tail = NULL;
		bass_head = NULL;
		bass_tail = NULL;
	}
	return 0;
}
