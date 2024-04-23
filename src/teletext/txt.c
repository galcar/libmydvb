#include <stdio.h>

#include "txtfont.h"

int main (int argc, char *argv) {
	unsigned char b;
	int char_num;
	int i, k;
	
	printf ("unsigned char character_set[256][9*8]=[\n");
	for (k=0; k < 256; k++) {
		printf ("/*%d 0x%.2x*/\n", k, k);
		char_num = k * TXT_CHAR_HEIGHT;
		printf ("\t[");
		for (i=0; i < TXT_CHAR_HEIGHT; i++) {
			b = txtfont[char_num + i];
			printf ("%d,", (b&0x01)==0?0:1);
			printf ("%d,", (b&0x02)==0?0:1);
			printf ("%d,", (b&0x04)==0?0:1);
			printf ("%d,", (b&0x08)==0?0:1);
			printf ("%d,", (b&0x10)==0?0:1);
			printf ("%d,", (b&0x20)==0?0:1);
			printf ("%d,", (b&0x40)==0?0:1);
			printf ("%d,", (b&0x80)==0?0:1);
			printf ("\n\t ");
		}
		printf ("\t]");
		if (k < 256 - 1) {
			printf (",");
		}
		printf ("\n");
	}
	printf ("];\n");
	
	return 0;
}
