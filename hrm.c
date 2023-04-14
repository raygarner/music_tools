/* hrm: harmonise
 * takes a list of notes (eg output from crd) and adds an accompany part
 * beneath it (bass voice)
 * additionally takes a key sig to use to generate the bass voice
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"

enum { THIRD = 2, FOURTH = 3, FIFTH = 4, SIXTH = 5 };
enum { FOURTH_TONES = 5, TRITONE = 6, FIFTH_TONES = 7 };

typedef struct Path Path;
struct Path {
	int faults;
	Node *head;
};

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

int
add_middle_note(int bass_note, int mld_note, int root, int mode)
{
	/* calculate chord */
	/* branch for adding 1st, 3rd or 5th */
	/* evaluate based on smoothness */
	int chord_root, chord_degree, inverted;

	chord_degree = calc_degree(bass_note, root, mode);
	if (is_primary_degree(bass_note, root, mode)) {
		chord_root = bass_note;
		inverted = FALSE;
	} else {
		chord_root = apply_steps(chord_degree, mode, bass_note, -THIRD);
		inverted = TRUE;
	}
	/* if root and fifth then mid must be 3rd */
	if ((inverted == FALSE) && (mld_note == apply_steps(chord_degree, mode, chord_root, FIFTH))) {
		return apply_steps(chord_degree, mode, chord_root, THIRD);
	}
	/* if root and third then mid must be 5th or 1st*/
	if ((inverted == FALSE) && (mld_note == apply_steps(chord_degree, mode, chord_root, THIRD))) {
		/* TODO: branch here (fifth or first) */
		return apply_steps(chord_degree, mode, chord_root, FIFTH);
	}
	/* if both first then mid must be 3rd */
	if ((inverted == FALSE) && mld_note == bass_note) {
		return apply_steps(chord_degree, mode, chord_root, THIRD);
	}
	/* if third and fifth then mid must be first */
	if ((inverted == TRUE) && (mld_note == apply_steps(chord_degree, mode, chord_root, FIFTH))) {
		return chord_root;
	}
	/* if third and third then mid must be first */
	if ((inverted == TRUE) && (mld_note == bass_note)) {
		return chord_root;
	}
	/* if third and first then mid must be fifth or first */
	if ((inverted == TRUE) && mld_note == chord_root) {
		/* TODO: branch here */
		return apply_steps(chord_degree, mode, chord_root, FIFTH);
	}
	return ERROR;
}

int
min(int a, int b)
{
	return a < b ? a : b;
}

/* starts at final chord adding most complete note */
/* works back adding nearest chord tone */
Node *
generate_middle_line(Node *bass_tail, Node *mld_tail, int root, int mode)
{
	/* check if bass note is primary degree */
	/* if it is then add third if melody is octave or fifth */
	/* if melody is third then add fifth or octave (whichever is nearer to prev) */
	Node *mid_head = NULL, *mid_tail = NULL;
	int mid_note, first, third, fifth, adj_note, dfirst, dthird, dfifth,
	    chord_degree;

	while (bass_tail && mld_tail) {
		if (mid_tail == NULL) /* add most complete end chord */
			mid_note = add_middle_note(bass_tail->data, mld_tail->data, root, mode);
		else { /* use nearest chord tone */
			chord_degree = calc_degree(bass_tail->data, root, mode);
			/* if root in bass */
			if (is_primary_degree(bass_tail->data, root, mode)) {
				first = bass_tail->data;
				third = apply_steps(chord_degree, mode, first, THIRD);
				fifth = apply_steps(chord_degree, mode, first, FIFTH);
			} else { /* if third in bass */
				first = apply_steps(chord_degree, mode, bass_tail->data, -THIRD);
				third = bass_tail->data;
				fifth = apply_steps(chord_degree-THIRD, mode, first, FIFTH);
			}
			adj_note = mid_tail->data;
			dfirst = abs(min_tone_diff(adj_note, first));
			dthird = abs(min_tone_diff(adj_note, third));
			dfifth = abs(min_tone_diff(adj_note, fifth));
			if (dthird <= dfirst && dthird <= dfifth)
				mid_note = third;
			else if (dfifth <= dfirst && dfifth <= dthird)
				mid_note = fifth;
			else
				mid_note = first;
		}
		mid_head = prepend_node(mid_head, mid_note);
		if (mid_tail == NULL)
			mid_tail = mid_head;
		bass_tail = bass_tail->prev;
		mld_tail = mld_tail->prev;
	}
	return mid_head;
}

int
pick_primary_chord(int mld_degree)
{
	switch (mld_degree) {
	case I: /* I or IV */
		return I;
	case II:
		return V;
	case III:
		return I;
	case IV:
		return IV;
	case V: /* I or V */
		return I;
	case VI:
		return IV;
	case VII:
		return V;
	}
	return -1;
}

int
negative(int x)
{
	return x < 0;
}

int
faulty_note(Node *bass_note, Node *mld_note, int root, int mode)
{
	int fault = 0, bass_degree, prev_bass_degree, ia, ib;

	if (!(bass_note->prev && mld_note->prev))
		return 0;
	/* if consecutive bass notes */
	if (bass_note->prev->data == bass_note->data)
		fault += 1;
	/* parallel octaves */
	if (bass_note->data == mld_note->data && bass_note->prev->data == mld_note->prev->data)
		fault += 1;
	/* tritone leap */
	if (abs(min_tone_diff(bass_note->data, bass_note->prev->data)) == TRITONE)
		fault += 1;
	/* paralell fifths */
	bass_degree = calc_degree(bass_note->data, root, mode);
	prev_bass_degree = calc_degree(bass_note->prev->data, root, mode);
	if ((mld_note->data == apply_steps(bass_degree, mode, bass_note->data, FIFTH) && \
	mld_note->prev->data == apply_steps(prev_bass_degree, mode, bass_note->prev->data, FIFTH)))
		fault += 1;
	/* double leaps in same direction */
	if (bass_note->prev->prev) {
		ia = min_tone_diff(bass_note->data, bass_note->prev->data);
		ib = min_tone_diff(bass_note->prev->data, bass_note->prev->prev->data);
		if (abs(ia) > TONE && abs(ib) > TONE && negative(ia) && negative(ib)) {
			fault += 1;
		}
	}
	/* sirening */
	if (bass_note->prev->prev && bass_note->prev->prev->prev && \
	bass_note->data == bass_note->prev->prev->data &&
	bass_note->prev->data == bass_note->prev->prev->prev->data)
		fault += 1;
	return fault;
}

int
count_faults(Node *bass_note, Node *mld_note, int root, int mode)
{
	int c = 0, range = 0, leaps = 0, steps = 0, interval;

	while (bass_note && mld_note) {
		if (bass_note->prev) {
			interval = min_tone_diff(bass_note->data, bass_note->prev->data);
			range += interval;
			if (abs(interval) > TONE) {
				leaps++;
			} else {
				steps++;
			}
		}
		c += faulty_note(bass_note, mld_note, root, mode);
		bass_note = bass_note->prev;
		mld_note = mld_note->prev;
	}
	/*
	if (abs(range) > TONES)
		c += abs(range) - TONES;
	if (leaps > steps)
		c += leaps-steps;
	*/
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
Path
improve_bass_note(Node *bass_note, Node *mld_note, int root, int mode)
{
	int tmp_note, mld_degree, old_bass_degree, new_bass_degree, 
	    a_faults = 9999, b_faults = 9999, c_faults = 9999, 
	    base_faults = 9999, ret = 0, inverted_note;
	Node *bass_alt_head_a = NULL, *bass_alt_head_b = NULL,
	     *bass_alt_head_c = NULL, *def_head = NULL, *def_line = NULL,
	     *a_line = NULL, *b_line = NULL, *c_line = NULL;
	Path best_path, a_path, b_path, c_path, def_path;

	best_path.head = NULL;
	a_path.head = NULL;
	b_path.head = NULL;
	c_path.head = NULL;
	def_path.head = NULL;

	best_path.faults = 9999;
	a_path.faults = 9999;
	b_path.faults = 9999;
	c_path.faults = 9999;
	def_path.faults = 9999;

	if (bass_note->next == NULL || mld_note->next == NULL) {
		best_path.head = copy_list_from_tail(bass_note);
		best_path.faults = count_faults(bass_note, mld_note, root, mode);
		return best_path;
	}
	tmp_note = bass_note->data;
	mld_degree = calc_degree(mld_note->data, root, mode);
	old_bass_degree = calc_degree(bass_note->data, root, mode);

	/* try different chord */
	if (mld_degree == I || mld_degree == V) {
		bass_alt_head_a = copy_list(bass_note);
		bass_alt_head_a->prev = bass_note->prev;

		new_bass_degree = alt_chord_choice(old_bass_degree, mld_degree);
		bass_alt_head_a->data = apply_steps(I, mode, root, new_bass_degree);
		a_path = improve_bass_note(bass_alt_head_a->next, mld_note->next, root, mode);

		/* try inverting different chord */
		inverted_note = apply_steps(new_bass_degree, mode, bass_alt_head_a->data, THIRD);
		if (mld_note->data == bass_alt_head_a->data) { /* if both first then try inverting bass */
			bass_alt_head_b = copy_list(bass_note);
			bass_alt_head_b->prev = bass_note->prev;
			bass_alt_head_b->data = apply_steps(new_bass_degree, mode, bass_alt_head_a->data, THIRD);
			b_path = improve_bass_note(bass_alt_head_b->next, mld_note->next, root, mode);
		}
	}

	/* try inverting original chord */
	inverted_note = apply_steps(old_bass_degree, mode, tmp_note, THIRD);
	if (mld_note->data == bass_note->data) {
		bass_alt_head_c = copy_list(bass_note);
		bass_alt_head_c->prev = bass_note->prev;
		bass_alt_head_c->data = inverted_note;
		c_path = improve_bass_note(bass_alt_head_c->next, mld_note->next, root, mode);
	}

	/* base faults */
	def_head = copy_list(bass_note);
	def_head->prev = bass_note->prev;
	def_path = improve_bass_note(def_head->next, mld_note->next, root, mode);

	if (def_path.faults <= a_path.faults && def_path.faults <= b_path.faults && def_path.faults <= c_path.faults) {
		best_path = def_path;
		delete_list(a_path.head);
		delete_list(b_path.head);
		delete_list(c_path.head);
	} else if (a_path.faults <= def_path.faults && a_path.faults <= b_path.faults && a_path.faults <= c_path.faults) {
		best_path = a_path;
		delete_list(def_path.head);
		delete_list(b_path.head);
		delete_list(c_path.head);
	} else if (b_path.faults <= def_path.faults && b_path.faults <= a_path.faults && b_path.faults <= c_path.faults) {
		best_path = b_path;
		delete_list(def_path.head);
		delete_list(a_path.head);
		delete_list(c_path.head);
	} else {
		best_path = c_path;
		delete_list(def_path.head);
		delete_list(a_path.head);
		delete_list(b_path.head);
	}
	delete_list(def_head);
	delete_list(bass_alt_head_a);
	delete_list(bass_alt_head_b);
	delete_list(bass_alt_head_c);
	return best_path;
}

Node *
generate_bass_line(Node *melody_tail, Node **bass_tail, int root, int mode)
{
	Node *bass_head = NULL, *melody_note = melody_tail;
	int bass_note, mld_degree, chord;

	while (melody_note) {
		mld_degree = calc_degree(melody_note->data, root, mode);
		chord = pick_primary_chord(mld_degree);
		bass_note = apply_steps(0, mode, root, chord);
		bass_head = prepend_node(bass_head, bass_note);
		if (*bass_tail == NULL)
			*bass_tail = bass_head;
		melody_note = melody_note->prev;
	}
	return bass_head;
}

int
main(int argc, char *argv[])
{
	char c, buf[BUFLEN];
	int root = -1, mode = -1, alter;
	Node *melody_tail = NULL, *melody_head = NULL, *bass_head = NULL,
	     *bass_tail = NULL, *mid_head = NULL, *new_bass_head = NULL,
	     *new_bass_tail = NULL;
	Path improved_bass;

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
		bass_head = generate_bass_line(melody_tail, &bass_tail, root, mode);
		improved_bass = improve_bass_note(bass_head, melody_head, root, mode);
		new_bass_head = improved_bass.head;
		delete_list(bass_head);
		new_bass_tail = new_bass_head;
		while (new_bass_tail->next)
			new_bass_tail = new_bass_tail->next;
		mid_head = generate_middle_line(new_bass_tail, melody_tail, root, mode);
		alter = get_correct_accidental(root, mode);
		print_list(melody_head, TRUE, root, mode);
		printf("- ");
		print_note(alter, root);
		putchar(' ');
		printf("%s\n", MODES[mode]);
		print_list(mid_head, TRUE, root, mode);
		printf("- ");
		print_note(alter, root);
		putchar(' ');
		printf("%s\n", MODES[mode]);
		print_list(new_bass_head, TRUE, root, mode);
		printf("- ");
		print_note(alter, root);
		putchar(' ');
		printf("%s\n", MODES[mode]);
		putchar('\n');
		getchar();
		c = getchar();
		delete_list(new_bass_head);
		delete_list(melody_head);
		delete_list(mid_head);
		melody_head = NULL;
		melody_tail = NULL;
		bass_head = NULL;
		bass_tail = NULL;
		new_bass_head = NULL;
		new_bass_tail = NULL;
	}
	return 0;
}
