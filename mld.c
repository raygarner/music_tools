/* build melody from chord input (including key mode)
 * put chord notes on strong beats (even beats)
 * fill gaps with most stepwise notes */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"

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

void
generate_line(int melody_len, const int notes[DEGREES], int notes_len, 
              int root, int mode)
{
	int *melody = NULL, i, tone_diff, step_diff, passing_step, d;

	melody = calloc(melody_len, sizeof(int));
	for (i = 0; i < melody_len; i+=2)
		melody[i] = notes[rand() % notes_len];
	for (i = 1; i < melody_len; i+=2) {
		tone_diff = min_tone_diff(melody[i-1], melody[i+1]);
		step_diff = count_scale_steps(root, mode, melody[i-1],
		                              melody[i+1]);
		step_diff = tone_diff < 0 ? DEGREES - step_diff : step_diff;
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
	printf("- %s %s\n", NOTES[root], MODES[mode]);
	free(melody);
}

int
main(int argc, char *argv[])
{
	char c, buf[BUFLEN];
	int notes[DEGREES], notes_len, melody_len, i, mode, 
	    root;

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
	srand(atoi(argv[2]));
	c = getchar();
	while (c != EOF) {
		i = 0;
		do {
			if (isspace(c))
				continue;
			notes[i] = read_tone(c, getchar());
			if (i == DEGREES-1)
				break;
			i++;
		} while ((c = getchar()) != '-');
		notes_len = i;
		while (isspace(c = getchar())) {}
		root = read_tone(c, getchar());
		scanf("%16s", buf);
		mode = read_mode(buf);
		generate_line(melody_len, notes, notes_len, root, mode);
		getchar();
		c = getchar();
	}
	return 0;
}
