/* $ echo "an aeolian" | crd 1 9
 * An Cn En Gn */
/* crd <degree of triad> <extension notes> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

int
main(int argc, char *argv[])
{
	int mode, i = DEGREES-1, n, key_root, chord_degree, chord_root, alter;
	char c, buf[BUFLEN], ext;

	if (argc < 2) {
		printf("Please pass chord degree and (optionally) extensions\n");
		printf("eg: crd 1 7\n");
		return 1;
	}
	chord_degree = atoi(argv[1]) - 1;
	if ((argc > 2 && argv[2][0] == ALL_KEYS) || (argc > 3 && argv[3][0] == ALL_KEYS)) {
		for (key_root = 0; key_root < TONES; key_root++) {
			for (mode = 0; mode < DEGREES; mode++) {
				alter = get_correct_accidental(key_root, mode);
				chord_root = apply_steps(I, mode, key_root, chord_degree);
				n = chord_root;
				print_note(alter, n);
				putchar(' ');
				n = apply_steps(chord_degree, mode, chord_root, III);
				print_note(alter, n);
				putchar(' ');
				n = apply_steps(chord_degree, mode, chord_root, V);
				print_note(alter, n);
				putchar(' ');
				if (argc > 2 && argv[2][0] != ALL_KEYS) {
					for (i = 0; i < strlen(argv[2]); i++) {
						ext = argv[2][i];
						n = apply_steps(chord_degree, mode, chord_root, atoi(&ext)-1);
						print_note(alter, n);
						putchar(' ');
					}
				}
				printf("- ");
				print_note(alter, key_root);
				putchar(' ');
				printf("%s\n", MODES[mode]);
			}
		}
	} else {
		while ((c = getchar()) != EOF) {
			if (isspace(c))
				continue;
			key_root = read_tone(c, getchar());
			scanf("%16s", buf);
			mode = read_mode(buf);
			if (key_root == X) {
				for (key_root = 0; key_root < TONES; key_root++) {
					alter = get_correct_accidental(key_root, mode);
					chord_root = apply_steps(I, mode, key_root, chord_degree);
					n = chord_root;
					print_note(alter, n);
					putchar(' ');
					n = apply_steps(chord_degree, mode, chord_root, III);
					print_note(alter, n);
					putchar(' ');
					n = apply_steps(chord_degree, mode, chord_root, V);
					print_note(alter, n);
					putchar(' ');
					if (argc > 2) {
						for (i = 0; i < strlen(argv[2]); i++) {
							ext = argv[2][i];
							n = apply_steps(chord_degree, mode, chord_root, atoi(&ext)-1);
							print_note(alter, n);
							putchar(' ');
						}
					}
					printf("- ");
					print_note(alter, key_root);
					putchar(' ');
					printf("%s\n", MODES[mode]);
				}
			} else {
				alter = get_correct_accidental(key_root, mode);
				chord_root = apply_steps(I, mode, key_root, chord_degree);
				n = chord_root;
				print_note(alter, n);
				putchar(' ');
				n = apply_steps(chord_degree, mode, chord_root, III);
				print_note(alter, n);
				putchar(' ');
				n = apply_steps(chord_degree, mode, chord_root, V);
				print_note(alter, n);
				putchar(' ');
				if (argc > 2) {
					for (i = 0; i < strlen(argv[2]); i++) {
						ext = argv[2][i];
						n = apply_steps(chord_degree, mode, chord_root, atoi(&ext)-1);
						print_note(alter, n);
						putchar(' ');
					}
				}
				printf("- ");
				print_note(alter, key_root);
				putchar(' ');
				printf("%s\n", MODES[mode]);
			}
		}
	}
	return 0;
}
