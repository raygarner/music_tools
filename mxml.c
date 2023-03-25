/* take output from hrm and convert it to musicxml */
/* print to stdout, let user redirect into file */
/* note always use sharps to represent chromatics */
/* can read up to 3 part music */
#include <stdio.h>
#include <ctype.h>

#include "common.h"

#define PARTS 3

enum { BASS_CLEF, TREBLE_CLEF };
enum { BASS, MIDDLE, MELODY };

const char *ID[PARTS] = { "bass", "middle", "melody" };
const char *NAME[PARTS] = { "low", "mid", "high" };
const int OCTAVE[PARTS] = { 3, 5, 6 };

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
write_part_line(const char *id, Node *head_note, int octave, int clef, 
                const char *indent)
{
	char note;
	int alter, interval;

	printf("%s<part id=\"%s\">\n", indent, id);
	printf("%s\t<measure number=\"1\">\n", indent);
	if (clef == BASS_CLEF) {
		printf("%s\t\t<attributes>\n", indent);
		printf("%s\t\t\t<clef>\n", indent);
		printf("%s\t\t\t\t<sign>F</sign>\n", indent);
		printf("%s\t\t\t\t<line>4</line>\n", indent);
		printf("%s\t\t\t</clef>\n", indent);
		printf("%s\t\t</attributes>\n", indent);
	}
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
		/* calc octave */
		if (head_note->prev) {
			interval = min_tone_diff(head_note->prev->data, head_note->data);
			/* if we have wrapped wround (passed into a different octave */
			/* if the interval downwards and the new note value is above the previous one then we have stepped down an octave*/
			if (interval < 0 && head_note->prev->data < head_note->data) {
				octave--;
			} else if (interval > 0 && head_note->prev->data > head_note->data) {
				octave++;
			}
		}
		printf("%s\t\t\t\t<octave>%d</octave>\n", indent, octave); /* TODO: make octave be the one nearest to the previous note */
		printf("%s\t\t\t</pitch>\n", indent);
		printf("%s\t\t\t<duration>1</duration>\n", indent);
		if (head_note->next)
			printf("%s\t\t\t<type>quarter</type>\n", indent);
		else
			printf("%s\t\t\t<type>whole</type>\n", indent);
		printf("%s\t\t</note>\n", indent);
		head_note = head_note->next;
	}
	printf("%s\t</measure>\n", indent);
	printf("%s</part>\n", indent);
}

int
main(int argc, char *argv[])
{
	Node *melody_head = NULL, *melody_tail = NULL, *middle_head = NULL,
	     *middle_tail = NULL, *bass_head = NULL, *bass_tail = NULL;
	int i, root, mode;
	char c, buf[BUFLEN];

	write_headers();
	printf("<score-partwise version=\"4.0\">\n");
	printf("\t<part-list>\n");
	for (i = PARTS-1; i >= 0; i--)
		write_part_def(ID[i], NAME[i], "\t\t");
	printf("\t</part-list>\n");
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
		c = getchar();
		do {
			if (isspace(c))
				continue;
			middle_tail = append_node(middle_tail, read_tone(c, getchar()));
			if (middle_head == NULL)
				middle_head = middle_tail;
		} while ((c = getchar()) != '-');
		while (isspace(c = getchar())) {}
		root = read_tone(c, getchar());
		scanf("%16s", buf);
		mode = read_mode(buf);
		c = getchar();
		do {
			if (isspace(c))
				continue;
			bass_tail = append_node(bass_tail, read_tone(c, getchar()));
			if (bass_head == NULL)
				bass_head = bass_tail;
		} while ((c = getchar()) != '-');
		while (isspace(c = getchar())) {}
		root = read_tone(c, getchar());
		scanf("%16s", buf);
		mode = read_mode(buf);
		write_part_line(ID[MELODY], melody_head, OCTAVE[MELODY], TREBLE_CLEF, "\t");
		write_part_line(ID[MIDDLE], middle_head, OCTAVE[MIDDLE], TREBLE_CLEF, "\t");
		write_part_line(ID[BASS], bass_head, OCTAVE[BASS], BASS_CLEF, "\t");
		delete_list(melody_head);
		delete_list(middle_head);
		delete_list(bass_head);
		c = getchar();
		while (isspace(c) && c != EOF) {
			c = getchar();
		}
	}
	printf("</score-partwise>\n");
	return 0;
}
