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
#include "mydvb_date.h"

#include <string.h>
#include <stdlib.h>

#define bcdtoint(i) ((((i & 0xf0) >> 4) * 10) + (i & 0x0f))

mydvb_date *mydvb_date_create (const unsigned char *buf) {

	mydvb_date *d = (mydvb_date *) malloc (sizeof(mydvb_date));

	memcpy (d->date, buf, 5);

	return d;
}

void mydvb_date_free (mydvb_date *d) {
	if (d == NULL) {
		return;
	}
	free(d);
}

time_t mydvb_date_to_tm (mydvb_date *d, struct tm *tma) {
  unsigned int year, month, day, hour, min, sec;
  unsigned long int mjd;

  mjd = (unsigned int)(d->date[0] & 0xff) << 8;
  mjd +=(unsigned int)(d->date[1] & 0xff);
  hour =(unsigned char)bcdtoint(d->date[2] & 0xff);
  min = (unsigned char)bcdtoint(d->date[3] & 0xff);
  sec = (unsigned char)bcdtoint(d->date[4] & 0xff);
  year =(unsigned long)((mjd - 15078.2)/365.25);
  month=(unsigned long)((mjd - 14956.1 - (unsigned long)(year * 365.25))/30.6001);
  day = mjd - 14956 - (unsigned long)(year * 365.25) - (unsigned long)(month * 30.6001);

  if (month == 14 || month == 15) {
    month = month - 1 - 12;
    year ++;
  } else {
    month = month - 1;
  }

  memset (tma, 0, sizeof (struct tm));

  tma->tm_sec=sec;
  tma->tm_min=min;
  tma->tm_hour=hour;
  tma->tm_mday=day;
  tma->tm_mon=month-1;
  tma->tm_year=year;
  tma->tm_isdst=-1;

  //return mktime (tma);
  return timegm (tma);

}
