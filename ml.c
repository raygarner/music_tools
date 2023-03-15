/* ml: melodically arrange note input (eg output from crd) 
 * maximises symetry and minimises intervals between notes
 * ref: https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.433.2985&rep=rep1&type=pdf
 * chord based melody (chapter 2.5.2)
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"

int
min_tone_diff(int note_a, int note_b)
{
	int down_steps = clock_mod(note_a - note_b, TONES);
	int up_steps = clock_mod(note_b - note_a, TONES);

	if (down_steps < up_steps)
		return 0-down_steps;
	else
		return up_steps;
}

int
count_scale_steps(int root, int mode, int start_note, int end_note)
{
	int i, d, cn = start_note;

	d = calc_degree(start_note, root, mode);
	for (i = 0; i < DEGREES; i++) {
		if (cn == end_note)
			return i;
		cn = step(d, cn, mode);
		d = (d+1) % DEGREES;
	}
	return ERROR;
}

int
apply_steps(int degree, int mode, int note, int steps)
{
	int dir, s, i;

	dir = steps < 0 ? DOWN: UP;
	degree = steps < 0 ? clock_mod(degree - 1, DEGREES) : degree;
	for (s = 0; s != steps; s += dir) {
		i = MAJOR_SCALE[clock_mod(degree + mode, DEGREES)];
		note = clock_mod(note + dir * i, TONES);
		degree = clock_mod(degree + dir, DEGREES);
	}
	return note;
}

int
main(int argc, char *argv[])
{
	char c, buf[BUFLEN];
	int notes[DEGREES], notes_len, melody_len, i, *melody = NULL, 
	    tone_diff, step_diff, mode, root, d, passing_step;

	if (argc < 2) {
		printf("Please pass melody length and seed as args\n");
		printf("eg: ml 5 98\n");
		return 1;
	}
	melody_len = atoi(argv[1]);
	if (melody_len == 0) {
		printf("Invalid melody length\n");
		return 1;
	}
	melody_len = melody_len % 2 == 0 ? melody_len + 1 : melody_len;
	melody = calloc(melody_len, sizeof(int));
	srand(atoi(argv[2]));
	i = 0;
	while ((c = getchar()) != '-') {
		if (isspace(c))
			continue;
		notes[i] = read_tone(c, getchar());
		if (i == DEGREES-1)
			break;
		i++;
	}
	notes_len = i;
	while (isspace(c = getchar())) {}
	root = read_tone(c, getchar());
	scanf("%16s", buf);
	mode = read_mode(buf);
	for (i = 0; i < melody_len; i+=2)
		melody[i] = notes[rand() % notes_len];
	for (i = 1; i < melody_len; i+=2) {
		tone_diff = min_tone_diff(melody[i-1], melody[i+1]);
		step_diff = count_scale_steps(root, mode, melody[i-1], 
		                              melody[i+1]);
		step_diff = tone_diff < 0 ? DEGREES-step_diff : step_diff;
		if (step_diff != 0)
			passing_step = rand() % step_diff;
		else
			passing_step = rand() % 2 == 0 ? -1 : 1;
		passing_step = tone_diff < 0 ? 0 - passing_step : passing_step;
		d = calc_degree(melody[i-1], root, mode);
		melody[i] = apply_steps(d, mode, melody[i-1], passing_step);
	}
	for (i = 0; i < melody_len; i++)
		printf("%s ", NOTES[melody[i]]);
	putchar('\n');
	free(melody);
	return 0;
}
