/* character font for Panteltje (c) krs teletext program */

/* there are differences with ASCII */
/* graphics are in 128 up (bit 8 set) */


#ifndef _TXTFONT_H
#define _TXTFONT_H

#define TXT_CHAR_WIDTH	8
#define TXT_CHAR_HEIGHT	9

static unsigned char txtfont[256 * TXT_CHAR_HEIGHT]=
{
/* 0 */
0x59,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 0 ^@ */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 1 ^A */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 2 ^B */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 3 ^C */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 4 ^D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 5 ^E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 6 ^F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 7 ^G */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 8 ^H */ 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 9 ^I */
/* 10 */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 10 ^J */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 11 ^K */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 12 ^L */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 13 ^M */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 14 ^N */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 15 ^O */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 16 ^P */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 17 ^Q */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 18 ^R */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 19 ^S */
/* 20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 20 ^T */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 21 ^U */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 22 ^V */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 23 ^V */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 24 ^W */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 25 ^X */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 26 ^Y */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 27 ^[ ESCAPE */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 28 ^\ FILE SEPARATOR */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 29 ^] GROUP SEPARATOR */
/* 30 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 30 ^^ RECORD SEPARATOR */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 31 ^_ UNIT SEPARATOT */
/* start of characters */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 32 space */
0x40,0x40,0x40,0x40,0x00,0x00,0x40,0x00,0x00,/* 33 ! */
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,/* 34 ~ */
0x28,0x28,0x7c,0x28,0x7c,0x28,0x28,0x00,0x00,/* 35 # */
0x10,0x3c,0x50,0x38,0x14,0x78,0x10,0x00,0x00,/* 36 $ */
0x60,0x64,0x08,0x10,0x20,0x4c,0x0c,0x00,0x00,/* 37 % */
0x30,0x48,0x50,0x20,0x54,0x48,0x34,0x00,0x00,/* 38 & */
0x10,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,/* 39 ' */
/* 40 */
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/* 40 ( */
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00,0x00,/* 41 ) */
0x00,0x10,0x54,0x38,0x54,0x10,0x00,0x00,0x00,/* 42 * */
0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00,0x00,/* 43 + */
0x00,0x00,0x00,0x00,0x60,0x20,0x40,0x00,0x00,/* 44 , */
0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,/* 45 - */
0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,/* 46 . */
0x00,0x04,0x08,0x10,0x20,0x40,0x00,0x00,0x00,/* 47 / */
0x38,0x44,0x4c,0x54,0x64,0x44,0x38,0x00,0x00,/* 48 0 */
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,/* 49 1 */
/* 50 */
0x38,0x44,0x04,0x18,0x20,0x40,0x7c,0x00,0x00,/* 50 2 */
0x38,0x44,0x04,0x18,0x04,0x44,0x38,0x00,0x00,/* 51 3 */
0x18,0x28,0x48,0x48,0x7c,0x08,0x08,0x00,0x00,/* 52 4 */
0x7c,0x40,0x40,0x78,0x04,0x04,0x78,0x00,0x00,/* 53 5 */
0x38,0x40,0x40,0x78,0x44,0x44,0x38,0x00,0x00,/* 54 6 */
0x7c,0x44,0x08,0x10,0x10,0x10,0x10,0x00,0x00,/* 55 7 */
0x38,0x44,0x44,0x38,0x44,0x44,0x38,0x00,0x00,/* 56 8 */
0x38,0x44,0x44,0x3c,0x04,0x04,0x38,0x00,0x00,/* 57 9 */
0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00,0x00,/* 58 : */
0x00,0x60,0x60,0x00,0x60,0x20,0x40,0x00,0x00,/* 59 ; */
/* 60 */
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00,0x00,/* 60 < */
0x00,0x00,0x7c,0x00,0x7c,0x00,0x00,0x00,0x00,/* 61 = */
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,0x00,/* 62 > */
0x38,0x44,0x04,0x08,0x10,0x00,0x10,0x00,0x00,/* 63 ? */
0x38,0x44,0x04,0x34,0x54,0x54,0x38,0x00,0x00,/* 64 @ */
0x38,0x44,0x44,0x44,0x7c,0x44,0x44,0x00,0x00,/* 65 A */
0x78,0x44,0x44,0x78,0x44,0x44,0x78,0x00,0x00,/* 66 B */
0x38,0x44,0x40,0x40,0x40,0x44,0x38,0x00,0x00,/* 67 C */
0x70,0x48,0x44,0x44,0x44,0x48,0x70,0x00,0x00,/* 68 D */
0x7c,0x40,0x40,0x78,0x40,0x40,0x7c,0x00,0x00,/* 69 E */
/* 70 */
0x7c,0x40,0x40,0x78,0x40,0x40,0x40,0x00,0x00,/* 70 F */
0x38,0x44,0x40,0x4c,0x44,0x44,0x3c,0x00,0x00,/* 71 G */
0x44,0x44,0x44,0x7c,0x44,0x44,0x44,0x00,0x00,/* 72 H */
0x38,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,/* 73 I */
0x1c,0x08,0x08,0x08,0x48,0x48,0x30,0x00,0x00,/* 74 J */
0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x00,0x00,/* 75 K */
0x40,0x40,0x40,0x40,0x40,0x40,0x7c,0x00,0x00,/* 76 L */
0x44,0x6c,0x54,0x54,0x44,0x44,0x44,0x00,0x00,/* 77 M */
0x44,0x44,0x64,0x54,0x4c,0x44,0x44,0x00,0x00,/* 78 N */
0x38,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00,/* 79 O */
/* 80 */
0x78,0x44,0x44,0x78,0x40,0x40,0x40,0x00,0x00,/* 80 P */
0x38,0x44,0x44,0x44,0x54,0x48,0x34,0x00,0x00,/* 81 Q */
0x78,0x44,0x44,0x78,0x50,0x48,0x44,0x00,0x00,/* 82 R */
0x3c,0x44,0x40,0x38,0x04,0x44,0x78,0x00,0x00,/* 83 S */
0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,/* 84 T */
0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00,/* 85 U */
0x44,0x44,0x44,0x44,0x44,0x28,0x10,0x00,0x00,/* 86 V */
0x44,0x44,0x44,0x54,0x54,0x54,0x28,0x00,0x00,/* 87 W */
0x44,0x44,0x28,0x10,0x28,0x44,0x44,0x00,0x00,/* 88 X */
0x44,0x44,0x44,0x28,0x10,0x10,0x10,0x00,0x00,/* 89 Y */
/* 90 */
0x7c,0x04,0x08,0x10,0x20,0x40,0x7c,0x00,0x00,/* 90 Z */
0x28,0x00,0x38,0x44,0x7c,0x44,0x44,0x00,0x00,/* 91 A umlaut, was [ in ASCII */
0x28,0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00,/* 92 O umlaut, was \ in ASCII */
0x28,0x00,0x44,0x44,0x44,0x44,0x38,0x00,0x00,/* 93 U umlaut, was ] in ASCCI */
0x00,0x00,0x10,0x28,0x44,0x44,0x00,0x00,0x00,/* 94 ^ */
0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,/* 95 _ */
0x18,0x24,0x18,0x00,0x00,0x00,0x00,0x00,0x00,/* 96 degrees sym, was 'in ASCII */
0x00,0x00,0x38,0x48,0x48,0x48,0x34,0x00,0x00,/* 97 a */
0x40,0x40,0x78,0x44,0x44,0x44,0x38,0x00,0x00,/* 98 b */
0x00,0x00,0x3c,0x40,0x40,0x40,0x3c,0x00,0x00,/* 99 c */
/* 100 */
0x04,0x04,0x3c,0x44,0x44,0x44,0x3c,0x00,0x00,/* 100 d */
0x00,0x00,0x38,0x44,0x7c,0x40,0x3c,0x00,0x00,/* 101 e */
0x0c,0x10,0x10,0x7c,0x10,0x10,0x10,0x00,0x00,/* 102 f */
0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x38,0x00,/* 103 g */
0x40,0x40,0x58,0x64,0x44,0x44,0x44,0x00,0x00,/* 104 h */
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,/* 105 i */
0x10,0x00,0x30,0x10,0x10,0x10,0x10,0x60,0x00,/* 106 j */
0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00,0x00,/* 107 k */
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,/* 108 l */
0x00,0x00,0x68,0x54,0x54,0x54,0x54,0x00,0x00,/* 109 m */
/* 110 */
0x00,0x00,0x58,0x64,0x44,0x44,0x44,0x00,0x00,/* 110 n */
0x00,0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00,/* 111 o */
0x00,0x00,0x78,0x44,0x44,0x78,0x40,0x40,0x00,/* 112 p */
0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x04,0x00,/* 113 q */
0x00,0x00,0x58,0x64,0x40,0x40,0x40,0x00,0x00,/* 114 r */
0x00,0x00,0x3c,0x40,0x38,0x04,0x78,0x00,0x00,/* 115 s */
0x10,0x10,0x7c,0x10,0x10,0x10,0x0c,0x00,0x00,/* 116 t */
0x00,0x00,0x44,0x44,0x44,0x4c,0x34,0x00,0x00,/* 117 u */
0x00,0x00,0x44,0x44,0x44,0x28,0x10,0x00,0x00,/* 118 v */
0x00,0x00,0x44,0x44,0x54,0x54,0x28,0x00,0x00,/* 119 w */
/* 120 */
0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00,/* 120 x */
0x00,0x00,0x44,0x44,0x44,0x3c,0x04,0x38,0x00,/* 121 y */
0x00,0x00,0x7c,0x08,0x10,0x20,0x7c,0x00,0x00,/* 122 z */
0x28,0x00,0x38,0x48,0x48,0x48,0x34,0x00,0x00,/* 123 a umlaut, was { in ASCII */
0x28,0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00,/* 124 o umlaut, was | in ASCII */
0x28,0x00,0x44,0x44,0x44,0x4c,0x34,0x00,0x00,/* 125 u umlaut, was } in ASCII */
0x00,0x30,0x48,0x58,0x44,0x58,0x40,0x00,0x00,/* 126 beta, was ~ in ASCII */
0x00,0x7e,0x7e,0x7e,0x7e,0x7e,0x00,0x00,0x00,/* 127 blokje upper sepa, was DEL*/
/* end of characters */
/* first graphics */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
/* 130 */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
/* 140 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 150 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 160 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x00,0x00,0x00,
/* 170 */
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0x00,0x00,0x00,0xf0,0xf0,0xf0,
/* 180 */
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xf0,0xf0,0xf0,
/* 190 */
0xff,0xff,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
/* 200 */
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xff,0xff,0xff,
/* 210 */
0xff,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
/* 220 */
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
/* 230 */
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
/* 240 */
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xff,0xff,0xff,
0xff,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,
/* 250 */
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};/* end txtfont */

static unsigned char spanish_txtfont[256 * TXT_CHAR_HEIGHT]=
{
/* 0 */
0x59,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 0 ^@ */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 1 ^A */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 2 ^B */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 3 ^C */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 4 ^D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 5 ^E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 6 ^F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 7 ^G */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 8 ^H */ 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 9 ^I */
/* 10 */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 10 ^J */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 11 ^K */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 12 ^L */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 13 ^M */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 14 ^N */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 15 ^O */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 16 ^P */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 17 ^Q */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 18 ^R */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 19 ^S */
/* 20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 20 ^T */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 21 ^U */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 22 ^V */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 23 ^V */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 24 ^W */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 25 ^X */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 26 ^Y */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,/* 27 ^[ ESCAPE */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 28 ^\ FILE SEPARATOR */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 29 ^] GROUP SEPARATOR */
/* 30 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 30 ^^ RECORD SEPARATOR */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 31 ^_ UNIT SEPARATOT */
/* start of characters */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* 32 space */
0x40,0x40,0x40,0x40,0x00,0x00,0x40,0x00,0x00,/* 33 ! */
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,/* 34 ~ */
0x00,0x00,0x3c,0x40,0x40,0x40,0x3c,0x02,0x04,/* 35 ç, was # in ascii */
0x10,0x3c,0x50,0x38,0x14,0x78,0x10,0x00,0x00,/* 36 $ */
0x60,0x64,0x08,0x10,0x20,0x4c,0x0c,0x00,0x00,/* 37 % */
0x30,0x48,0x50,0x20,0x54,0x48,0x34,0x00,0x00,/* 38 & */
0x10,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,/* 39 ' */
/* 40 */
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/* 40 ( */
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00,0x00,/* 41 ) */
0x00,0x10,0x54,0x38,0x54,0x10,0x00,0x00,0x00,/* 42 * */
0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00,0x00,/* 43 + */
0x00,0x00,0x00,0x00,0x60,0x20,0x40,0x00,0x00,/* 44 , */
0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,/* 45 - */
0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,/* 46 . */
0x00,0x04,0x08,0x10,0x20,0x40,0x00,0x00,0x00,/* 47 / */
0x38,0x44,0x4c,0x54,0x64,0x44,0x38,0x00,0x00,/* 48 0 */
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,/* 49 1 */
/* 50 */
0x38,0x44,0x04,0x18,0x20,0x40,0x7c,0x00,0x00,/* 50 2 */
0x38,0x44,0x04,0x18,0x04,0x44,0x38,0x00,0x00,/* 51 3 */
0x18,0x28,0x48,0x48,0x7c,0x08,0x08,0x00,0x00,/* 52 4 */
0x7c,0x40,0x40,0x78,0x04,0x04,0x78,0x00,0x00,/* 53 5 */
0x38,0x40,0x40,0x78,0x44,0x44,0x38,0x00,0x00,/* 54 6 */
0x7c,0x44,0x08,0x10,0x10,0x10,0x10,0x00,0x00,/* 55 7 */
0x38,0x44,0x44,0x38,0x44,0x44,0x38,0x00,0x00,/* 56 8 */
0x38,0x44,0x44,0x3c,0x04,0x04,0x38,0x00,0x00,/* 57 9 */
0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00,0x00,/* 58 : */
0x00,0x60,0x60,0x00,0x60,0x20,0x40,0x00,0x00,/* 59 ; */
/* 60 */
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00,0x00,/* 60 < */
0x00,0x00,0x7c,0x00,0x7c,0x00,0x00,0x00,0x00,/* 61 = */
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,0x00,/* 62 > */
0x38,0x44,0x04,0x08,0x10,0x00,0x10,0x00,0x00,/* 63 ? */
0x08,0x00,0x00,0x08,0x08,0x08,0x08,0x00,0x00,/* 64 !*/
0x38,0x44,0x44,0x44,0x7c,0x44,0x44,0x00,0x00,/* 65 A */
0x78,0x44,0x44,0x78,0x44,0x44,0x78,0x00,0x00,/* 66 B */
0x38,0x44,0x40,0x40,0x40,0x44,0x38,0x00,0x00,/* 67 C */
0x70,0x48,0x44,0x44,0x44,0x48,0x70,0x00,0x00,/* 68 D */
0x7c,0x40,0x40,0x78,0x40,0x40,0x7c,0x00,0x00,/* 69 E */
/* 70 */
0x7c,0x40,0x40,0x78,0x40,0x40,0x40,0x00,0x00,/* 70 F */
0x38,0x44,0x40,0x4c,0x44,0x44,0x3c,0x00,0x00,/* 71 G */
0x44,0x44,0x44,0x7c,0x44,0x44,0x44,0x00,0x00,/* 72 H */
0x38,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,/* 73 I */
0x1c,0x08,0x08,0x08,0x48,0x48,0x30,0x00,0x00,/* 74 J */
0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x00,0x00,/* 75 K */
0x40,0x40,0x40,0x40,0x40,0x40,0x7c,0x00,0x00,/* 76 L */
0x44,0x6c,0x54,0x54,0x44,0x44,0x44,0x00,0x00,/* 77 M */
0x44,0x44,0x64,0x54,0x4c,0x44,0x44,0x00,0x00,/* 78 N */
0x38,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00,/* 79 O */
/* 80 */
0x78,0x44,0x44,0x78,0x40,0x40,0x40,0x00,0x00,/* 80 P */
0x38,0x44,0x44,0x44,0x54,0x48,0x34,0x00,0x00,/* 81 Q */
0x78,0x44,0x44,0x78,0x50,0x48,0x44,0x00,0x00,/* 82 R */
0x3c,0x44,0x40,0x38,0x04,0x44,0x78,0x00,0x00,/* 83 S */
0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,/* 84 T */
0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00,/* 85 U */
0x44,0x44,0x44,0x44,0x44,0x28,0x10,0x00,0x00,/* 86 V */
0x44,0x44,0x44,0x54,0x54,0x54,0x28,0x00,0x00,/* 87 W */
0x44,0x44,0x28,0x10,0x28,0x44,0x44,0x00,0x00,/* 88 X */
0x44,0x44,0x44,0x28,0x10,0x10,0x10,0x00,0x00,/* 89 Y */
/* 90 */
0x7c,0x04,0x08,0x10,0x20,0x40,0x7c,0x00,0x00,/* 90 Z */
0x08,0x10,0x38,0x48,0x48,0x48,0x34,0x00,0x00,/* 91 á, was [ in ASCII */
0x08,0x10,0x38,0x44,0x7c,0x40,0x3c,0x00,0x00,/* 92 é, was \ in ASCII */
0x20,0x40,0x60,0x20,0x20,0x20,0x70,0x00,0x00,/* 93 í, was ] in ASCCI */
0x08,0x10,0x38,0x44,0x44,0x44,0x38,0x00,0x00,/* 94 ó */
0x08,0x10,0x44,0x44,0x44,0x4c,0x34,0x00,0x00,/* 95 ú */
0x08,0x00,0x08,0x10,0x20,0x22,0x1c,0x00,0x00,/* 96 ¿, was 'in ASCII */
0x00,0x00,0x38,0x48,0x48,0x48,0x34,0x00,0x00,/* 97 a */
0x40,0x40,0x78,0x44,0x44,0x44,0x38,0x00,0x00,/* 98 b */
0x00,0x00,0x3c,0x40,0x40,0x40,0x3c,0x00,0x00,/* 99 c */
/* 100 */
0x04,0x04,0x3c,0x44,0x44,0x44,0x3c,0x00,0x00,/* 100 d */
0x00,0x00,0x38,0x44,0x7c,0x40,0x3c,0x00,0x00,/* 101 e */
0x0c,0x10,0x10,0x7c,0x10,0x10,0x10,0x00,0x00,/* 102 f */
0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x38,0x00,/* 103 g */
0x40,0x40,0x58,0x64,0x44,0x44,0x44,0x00,0x00,/* 104 h */
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,/* 105 i */
0x10,0x00,0x30,0x10,0x10,0x10,0x10,0x60,0x00,/* 106 j */
0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00,0x00,/* 107 k */
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,/* 108 l */
0x00,0x00,0x68,0x54,0x54,0x54,0x54,0x00,0x00,/* 109 m */
/* 110 */
0x00,0x00,0x58,0x64,0x44,0x44,0x44,0x00,0x00,/* 110 n */
0x00,0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00,/* 111 o */
0x00,0x00,0x78,0x44,0x44,0x78,0x40,0x40,0x00,/* 112 p */
0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x04,0x00,/* 113 q */
0x00,0x00,0x58,0x64,0x40,0x40,0x40,0x00,0x00,/* 114 r */
0x00,0x00,0x3c,0x40,0x38,0x04,0x78,0x00,0x00,/* 115 s */
0x10,0x10,0x7c,0x10,0x10,0x10,0x0c,0x00,0x00,/* 116 t */
0x00,0x00,0x44,0x44,0x44,0x4c,0x34,0x00,0x00,/* 117 u */
0x00,0x00,0x44,0x44,0x44,0x28,0x10,0x00,0x00,/* 118 v */
0x00,0x00,0x44,0x44,0x54,0x54,0x28,0x00,0x00,/* 119 w */
/* 120 */
0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00,/* 120 x */
0x00,0x00,0x44,0x44,0x44,0x3c,0x04,0x38,0x00,/* 121 y */
0x00,0x00,0x7c,0x08,0x10,0x20,0x7c,0x00,0x00,/* 122 z */
0x28,0x00,0x44,0x44,0x44,0x4c,0x34,0x00,0x00,/* 123 u umlaut, was { in ASCII */
0x32,0x4c,0x58,0x64,0x44,0x44,0x44,0x00,0x00,/* 124 ñ */
0x20,0x10,0x38,0x44,0x7c,0x40,0x3c,0x00,0x00,/* 125 è, was } in ASCII */
0x20,0x10,0x38,0x48,0x48,0x48,0x34,0x00,0x00,/* 126 à, was ~ in ASCII */
0x00,0x7e,0x7e,0x7e,0x7e,0x7e,0x00,0x00,0x00,/* 127 blokje upper sepa, was DEL*/
/* end of characters */
/* first graphics */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
/* 130 */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
/* 140 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 150 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 160 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x00,0x00,0x00,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x00,0x00,0x00,
/* 170 */
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x00,0x00,0x00,
0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x00,0x00,0x00,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0x00,0x00,0x00,0xf0,0xf0,0xf0,
/* 180 */
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xf0,0xf0,0xf0,
/* 190 */
0xff,0xff,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
/* 200 */
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xff,0xff,0xff,
/* 210 */
0xff,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
/* 220 */
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x00,0x00,0x00,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
/* 230 */
0xff,0xff,0xff,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x0f,0x0f,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
/* 240 */
0xf0,0xf0,0xf0,0x00,0x00,0x00,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x00,0x00,0x00,0xff,0xff,0xff,
0xff,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0xff,
0x00,0x00,0x00,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0xff,0xff,0xff,0xf0,0xf0,0xf0,0xff,0xff,0xff,
0x00,0x00,0x00,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,
/* 250 */
0xff,0xff,0xff,0x0f,0x0f,0x0f,0xff,0xff,0xff,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};/* end txtfont */

#endif

