/*
 * This file is part of the libmydvb distribution (https://github.com/galcar/libmydvb).
 * Copyright (c) 2024 G. Alcaraz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <mydvb_text.h>


static uint32_t _mydvb_text_table_pages[][256] = {
		// table 0, latin alphabet (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003a,0x003b,0x003c,0x003d,0x003e,0x003f,
	0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004a,0x004b,0x004c,0x004d,0x004e,0x004f,
	0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005a,0x005b,0x005c,0x005d,0x005e,0x005f,
	0x0060,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006a,0x006b,0x006c,0x006d,0x006e,0x006f,
	0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007a,0x007b,0x007c,0x007d,0x007e,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0000,0x00a7,0x00a4,0x2018,0x201c,0x00ab,0x2190,0x2191,0x2192,0x2193,
	0x00b0,0x00b1,0x00b2,0x00b3,0x00d7,0x00b5,0x00b6,0x00b7,0x00f7,0x2019,0x201d,0x00bb,0x00bc,0x00bd,0x00be,0x00bf,
	0x0000,0x0300,0x0301,0x0302,0x0303,0x0304,0x0306,0x0307,0x0308,0x0000,0x030a,0x0327,0x0000,0x030b,0x0328,0x030c,
	0x2015,0x00b9,0x00ae,0x00a9,0x2122,0x266a,0x00ac,0x00a6,0x0000,0x0000,0x0000,0x0000,0x215b,0x215c,0x215d,0x215e,
	0x2126,0x00c6,0x0110,0x00aa,0x0126,0x0000,0x0132,0x013f,0x0141,0x00d8,0x0152,0x00ba,0x00de,0x0166,0x014a,0x0149,
	0x0138,0x00e6,0x0111,0x03b4,0x0127,0x0131,0x0133,0x0140,0x0142,0x00f8,0x0153,0x00df,0x00fe,0x0167,0x014b,0x00ad
	},
		// table 1, latin alphabet 1 (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003a,0x003b,0x003c,0x003d,0x003e,0x003f,
	0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004a,0x004b,0x004c,0x004d,0x004e,0x004f,
	0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005a,0x005b,0x005c,0x005d,0x005e,0x005f,
	0x0060,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006a,0x006b,0x006c,0x006d,0x006e,0x006f,
	0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007a,0x007b,0x007c,0x007d,0x007e,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0000,0x00a7,0x00a4,0x2018,0x201c,0x00ab,0x2190,0x2191,0x2192,0x2193,
	0x00b0,0x00b1,0x00b2,0x00b3,0x00d7,0x00b5,0x00b6,0x00b7,0x00f7,0x2019,0x201d,0x00bb,0x00bc,0x00bd,0x00be,0x00bf,
	0x0000,0x0300,0x0301,0x0302,0x0303,0x0304,0x0306,0x0307,0x0308,0x0000,0x030a,0x0327,0x0000,0x030b,0x0328,0x030c,
	0x2015,0x00b9,0x00ae,0x00a9,0x2122,0x266a,0x00ac,0x00a6,0x0000,0x0000,0x0000,0x0000,0x215b,0x215c,0x215d,0x215e,
	0x2126,0x00c6,0x0110,0x00aa,0x0126,0x0000,0x0132,0x013f,0x0141,0x00d8,0x0152,0x00ba,0x00de,0x0166,0x014a,0x0149,
	0x0138,0x00e6,0x0111,0x03b4,0x0127,0x0131,0x0133,0x0140,0x0142,0x00f8,0x0153,0x00df,0x00fe,0x0167,0x014b,0x00ad
	},
		// table 2, latin alphabet 2 (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003a,0x003b,0x003c,0x003d,0x003e,0x003f,
	0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004a,0x004b,0x004c,0x004d,0x004e,0x004f,
	0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005a,0x005b,0x005c,0x005d,0x005e,0x005f,
	0x0060,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006a,0x006b,0x006c,0x006d,0x006e,0x006f,
	0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007a,0x007b,0x007c,0x007d,0x007e,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0000,0x00a7,0x00a4,0x2018,0x201c,0x00ab,0x2190,0x2191,0x2192,0x2193,
	0x00b0,0x00b1,0x00b2,0x00b3,0x00d7,0x00b5,0x00b6,0x00b7,0x00f7,0x2019,0x201d,0x00bb,0x00bc,0x00bd,0x00be,0x00bf,
	0x0000,0x0300,0x0301,0x0302,0x0303,0x0304,0x0306,0x0307,0x0308,0x0000,0x030a,0x0327,0x0000,0x030b,0x0328,0x030c,
	0x2015,0x00b9,0x00ae,0x00a9,0x2122,0x266a,0x00ac,0x00a6,0x0000,0x0000,0x0000,0x0000,0x215b,0x215c,0x215d,0x215e,
	0x2126,0x00c6,0x0110,0x00aa,0x0126,0x0000,0x0132,0x013f,0x0141,0x00d8,0x0152,0x00ba,0x00de,0x0166,0x014a,0x0149,
	0x0138,0x00e6,0x0111,0x03b4,0x0127,0x0131,0x0133,0x0140,0x0142,0x00f8,0x0153,0x00df,0x00fe,0x0167,0x014b,0x00ad
	},
		// table 3, latin alphabet 3 (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003a,0x003b,0x003c,0x003d,0x003e,0x003f,
	0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004a,0x004b,0x004c,0x004d,0x004e,0x004f,
	0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005a,0x005b,0x005c,0x005d,0x005e,0x005f,
	0x0060,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006a,0x006b,0x006c,0x006d,0x006e,0x006f,
	0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007a,0x007b,0x007c,0x007d,0x007e,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0000,0x00a7,0x00a4,0x2018,0x201c,0x00ab,0x2190,0x2191,0x2192,0x2193,
	0x00b0,0x00b1,0x00b2,0x00b3,0x00d7,0x00b5,0x00b6,0x00b7,0x00f7,0x2019,0x201d,0x00bb,0x00bc,0x00bd,0x00be,0x00bf,
	0x0000,0x0300,0x0301,0x0302,0x0303,0x0304,0x0306,0x0307,0x0308,0x0000,0x030a,0x0327,0x0000,0x030b,0x0328,0x030c,
	0x2015,0x00b9,0x00ae,0x00a9,0x2122,0x266a,0x00ac,0x00a6,0x0000,0x0000,0x0000,0x0000,0x215b,0x215c,0x215d,0x215e,
	0x2126,0x00c6,0x0110,0x00aa,0x0126,0x0000,0x0132,0x013f,0x0141,0x00d8,0x0152,0x00ba,0x00de,0x0166,0x014a,0x0149,
	0x0138,0x00e6,0x0111,0x03b4,0x0127,0x0131,0x0133,0x0140,0x0142,0x00f8,0x0153,0x00df,0x00fe,0x0167,0x014b,0x00ad
	},
		// table 4, latin alphabet 4 (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,0x0028,0x0029,0x002a,0x002b,0x002c,0x002d,0x002e,0x002f,
	0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003a,0x003b,0x003c,0x003d,0x003e,0x003f,
	0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004a,0x004b,0x004c,0x004d,0x004e,0x004f,
	0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005a,0x005b,0x005c,0x005d,0x005e,0x005f,
	0x0060,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006a,0x006b,0x006c,0x006d,0x006e,0x006f,
	0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007a,0x007b,0x007c,0x007d,0x007e,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,0x00a1,0x00a2,0x00a3,0x20ac,0x00a5,0x0000,0x00a7,0x00a4,0x2018,0x201c,0x00ab,0x2190,0x2191,0x2192,0x2193,
	0x00b0,0x00b1,0x00b2,0x00b3,0x00d7,0x00b5,0x00b6,0x00b7,0x00f7,0x2019,0x201d,0x00bb,0x00bc,0x00bd,0x00be,0x00bf,
	0x0000,0x0300,0x0301,0x0302,0x0303,0x0304,0x0306,0x0307,0x0308,0x0000,0x030a,0x0327,0x0000,0x030b,0x0328,0x030c,
	0x2015,0x00b9,0x00ae,0x00a9,0x2122,0x266a,0x00ac,0x00a6,0x0000,0x0000,0x0000,0x0000,0x215b,0x215c,0x215d,0x215e,
	0x2126,0x00c6,0x0110,0x00aa,0x0126,0x0000,0x0132,0x013f,0x0141,0x00d8,0x0152,0x00ba,0x00de,0x0166,0x014a,0x0149,
	0x0138,0x00e6,0x0111,0x03b4,0x0127,0x0131,0x0133,0x0140,0x0142,0x00f8,0x0153,0x00df,0x00fe,0x0167,0x014b,0x00ad
	},
		// table 5, latin alphabet 5 (unicode values)
		{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0020,	0x0021,	0x0022,	0x0023,	0x0024,	0x0025,	0x0026,	0x0027,	0x0028,	0x0029,	0x002a,	0x002b,	0x002c,	0x002d,	0x002e,	0x002f,
	0x0030,	0x0031,	0x0032,	0x0033,	0x0034,	0x0035,	0x0036,	0x0037,	0x0038,	0x0039,	0x003a,	0x003b,	0x003c,	0x003d,	0x003e,	0x003f,
	0x0040,	0x0041,	0x0042,	0x0043,	0x0044,	0x0045,	0x0046,	0x0047,	0x0048,	0x0049,	0x004a,	0x004b,	0x004c,	0x004d,	0x004e,	0x004f,
	0x0050,	0x0051,	0x0052,	0x0053,	0x0054,	0x0055,	0x0056,	0x0057,	0x0058,	0x0059,	0x005a,	0x005b,	0x005c,	0x005d,	0x005e,	0x005f,
	0x0060,	0x0061,	0x0062,	0x0063,	0x0064,	0x0065,	0x0066,	0x0067,	0x0068,	0x0069,	0x006a,	0x006b,	0x006c,	0x006d,	0x006e,	0x006f,
	0x0070,	0x0071,	0x0072,	0x0073,	0x0074,	0x0075,	0x0076,	0x0077,	0x0078,	0x0079,	0x007a,	0x007b,	0x007c,	0x007d,	0x007e,	0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x00a0,	0x00a1,	0x00a2,	0x00a3,	0x00a4,	0x00a5,	0x00a6,	0x00a7,	0x00a8,	0x00a9,	0x00aa,	0x00ab,	0x00ac,	0x00ad,	0x00ae,	0x00af,
	0x00b0,	0x00b1,	0x00b2,	0x00b3,	0x00b4,	0x00b5,	0x00b6,	0x00b7,	0x00b8,	0x00b9,	0x00ba,	0x00bb,	0x00bc,	0x00bd,	0x00be,	0x00bf,
	0x00c0,	0x00c1,	0x00c2,	0x00c3,	0x00c4,	0x00c5,	0x00c6,	0x00c7,	0x00c8,	0x00c9,	0x00ca,	0x00cb,	0x00cc,	0x00cd,	0x00ce,	0x00cf,
	0x011e,	0x00d1,	0x00d2,	0x00d3,	0x00d4,	0x00d5,	0x00d6,	0x00d7,	0x00d8,	0x00d9,	0x00da,	0x00db,	0x00dc,	0x0130,	0x015e,	0x00df,
	0x00e0,	0x00e1,	0x00e2,	0x00e3,	0x00e4,	0x00e5,	0x00e6,	0x00e7,	0x00e8,	0x00e9,	0x00ea,	0x00eb,	0x00ec,	0x00ed,	0x00ee,	0x00ef,
	0x011f,	0x00f1,	0x00f2,	0x00f3,	0x00f4,	0x00f5,	0x00f6,	0x00f7,	0x00f8,	0x00f9,	0x00fa,	0x00fb,	0x00fc,	0x0131,	0x015f,	0x00ff
	}
};

mydvb_text *mydvb_text_create (const unsigned char *buf, size_t len) {

	mydvb_text *txt = NULL;

	int i = 0;
	const unsigned char *src = buf;
	uint32_t *dest;

	int table_code = 0;


	txt = (mydvb_text *) malloc (sizeof(mydvb_text));

	txt->lang = NULL;
	txt->length = 0;
	txt->text = NULL;

	if (buf == NULL || len == 0) {
		return txt; /* here, we always return an object */
	}

	txt->text = (uint32_t *) malloc (sizeof(uint32_t)*len);

	dest = txt->text;
	while (i < len) {
		if (i == 0) {
			if (*src < 0x20) {
				table_code = *src;
			}
		}
		if (table_code > 5) {
			break; // TODO: implement all other table codes
		}
		if (*src >= 0x20 && (*src < 0x80 || *src > 0x9f)) {
			*dest++ = _mydvb_text_table_pages[table_code][*src];
			txt->length ++;
		}
		src ++;
		i++;
	}

	if (txt->length == 0) {
		/* zap zero length texts */
		free (txt->text);
		txt->text = NULL;
	}

	return txt;
}

mydvb_text *mydvb_text_dup (mydvb_text *txt) {
	mydvb_text *dup = NULL;

	if (txt == NULL) {
		return NULL;
	}

	dup = (mydvb_text *) malloc (sizeof(mydvb_text));

	dup->lang 	= NULL;
	dup->length = txt->length;
	if (txt->length == 0) {
		dup->text = NULL;
	} else {
		dup->text 	= (uint32_t *) malloc (sizeof(uint32_t)*txt->length);

		memcpy (dup->text, txt->text, sizeof(uint32_t)*txt->length);
	}

	return dup;
}

void mydvb_text_free (mydvb_text *txt) {

	if (txt == NULL) {
		return;
	}

	if (txt->lang != NULL) {
		free (txt->lang);
		txt->lang = NULL;
	}

	if (txt->text != NULL) {
		free (txt->text);
		txt->text = NULL;
	}

	txt->length = 0;

	free (txt);

}

void mydvb_text_printf (mydvb_text *txt, FILE *f) {
	unsigned char *data = NULL;
	int i = 0;

	if (txt == NULL || f == NULL) {
		return;
	}

	if (txt->length == 0 || txt->text == NULL) {
		return;
	}

	data = (unsigned char *) txt->text;
	for (i = 0; i < txt->length; i++) {
		fputc (*data++, f);
	}
}

char *_unicode_to_utf8 (uint32_t *in, unsigned int length) {

	int i;
	uint32_t u;

	int ut8_len = 0;

	char *utf8;
	char *out;

	if (in == NULL || length == 0) {
		return NULL;
	}

	for (i = 0; i < length; i++) {
		u = in[i];
		if (u <= 0x7f)
			ut8_len += 1;
		else if (u <= 0x07FF)
			ut8_len += 2;
		else if (u <= 0xFFFF)
			ut8_len += 3;
		else if (u <= 0x10FFFF)
			ut8_len += 4;
		else
			ut8_len += 3;
	}

	utf8 = (char *) malloc (ut8_len+1);

	out = utf8;

	for (i = 0; i < length; i++) {

		u = in[i];

		if (u <= 0x7f) {
			*out++ = (char) u;

		} else if (u <= 0x07FF) {
		    // 2-byte unicode
		    *out++ = (char) (((u >> 6) & 0x1F) | 0xC0);
		    *out++ = (char) (((u >> 0) & 0x3F) | 0x80);

		} else if (u <= 0xFFFF) {
		    // 3-byte unicode
			*out++ = (char) (((u >> 12) & 0x0F) | 0xE0);
			*out++ = (char) (((u >>  6) & 0x3F) | 0x80);
			*out++ = (char) (((u >>  0) & 0x3F) | 0x80);

		} else if (u <= 0x10FFFF) {
		    // 4-byte unicode
			*out++ = (char) (((u >> 18) & 0x07) | 0xF0);
			*out++ = (char) (((u >> 12) & 0x3F) | 0x80);
			*out++ = (char) (((u >>  6) & 0x3F) | 0x80);
			*out++ = (char) (((u >>  0) & 0x3F) | 0x80);

		} else {
		    // error - use replacement character
		    *out++ = (char) 0xEF;
		    *out++ = (char) 0xBF;
		    *out++ = (char) 0xBD;
		}
	}

	*out = '\0';

	return utf8;

}

char *mydvb_text_to_utf8 (mydvb_text *t) {
	if (t == NULL) {
		return NULL;
	}
	return _unicode_to_utf8 (t->text, t->length);
}


