#ifndef __MYDVB_DATE_H__
#define __MYDVB_DATE_H__

#include <time.h>

typedef struct {
	unsigned char	date[5];	/* date in raw format */
} mydvb_date;

mydvb_date *mydvb_date_create (const unsigned char *buf);

void mydvb_date_free (mydvb_date *d);

time_t mydvb_date_to_tm (mydvb_date *d, struct tm *tma);

#endif
