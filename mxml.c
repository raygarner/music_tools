/* take output from hrm and convert it to musicxml */
/* print to stdout, let user redirect into file */
/* note always use sharps to represent chromatics */
#include <stdio.h>

#include "common.h"

#define PARTS 3

enum { BASS, MIDDLE, MELODY };

const char *ID[PARTS] = { "bass", "middle", "melody" };
const char *NAME[PARTS] = { "Bass", "Middle", "Melody" };
const int OCTAVE[PARTS] = { 4, 5, 6 };

void
write_headers()
{
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
	printf("<!DOCTYPE score-partwise PUBLIC\n");
	printf("\t\"-//Recordare//DTD MusicXML 4.0 Partwise//EN\"\n");
	printf("\t\"http://www.musicxml.org/dtds/partwise.dtd\">\n");
}

void
write_part_def(const char *id, const char *name, const char *indent)
{
	printf("%s<score-part id=\"%s\">\n", indent, id);
	printf("%s\t<part-name>%s</part-name>\n", indent, name);
	printf("%s</score-part>\n", indent);
}

void
write_part_line(const char *id, Node *head_note, int octave, const char *indent)
{
	char note;
	int alter;

	printf("%s<part id=\"%s\">\n", indent, id);
	printf("%s\t<measure number=\"1\">\n", indent);
	while (head_note) {
		printf("%s\t\t<note>\n", indent);
		printf("%s\t\t\t<pitch>\n", indent);
		if (is_accidental(head_note->data)) {
			note = NOTES[head_note->data-1][0];
			alter = SHARP;
		} else {
			note = NOTES[head_note->data][0];
			alter = NATURAL;
		}
		printf("%s\t\t\t\t<step>%c</step>\n", indent, note);;
		printf("%s\t\t\t\t<alter>%d</alter>\n", indent, alter);
		printf("%s\t\t\t\t<octave>%d</octave>\n", indent, octave);
		printf("%s\t\t\t</pitch>\n", indent);
		printf("%s\t\t\t<duration>1</duration>\n", indent);
		printf("%s\t\t\t<type>quarter</type>\n", indent);
		printf("%s\t\t</note>\n", indent);
		head_note = head_note->next;
	}
	printf("%s\t</measure>\n", indent);
	printf("%s</part>\n", indent);
}

int
main(int argc, char *argv[])
{
	Node *mldy_head = NULL;
	int i;

	mldy_head = prepend_node(mldy_head, 0);
	mldy_head = prepend_node(mldy_head, 0);
	mldy_head = prepend_node(mldy_head, 0);
	mldy_head = prepend_node(mldy_head, 0);
	write_headers();
	printf("<score-partwise version=\"4.0\">\n");
	printf("\t<part-list>\n");
	for (i = PARTS-1; i >= 0; i--) {
		write_part_def(ID[i], NAME[i], "\t\t");
	}
	printf("\t</part-list>\n");
	for (i = PARTS-1; i >= 0; i--) {
		write_part_line(ID[i], mldy_head, OCTAVE[i], "\t");
	}
	printf("</score-partwise>\n");
	putchar('\n');
	return 0;
}
