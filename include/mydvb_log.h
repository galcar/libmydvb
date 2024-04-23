#ifndef __MYDVB_LOG_H__
#define __MYDVB_LOG_H__

#include <stdio.h>

typedef enum {
	MYDVB_LOG_LEVEL_DEBUG,
	MYDVB_LOG_LEVEL_ERROR,
	MYDVB_LOG_LEVEL_INFO
} mydvb_log_level_t;


void mydvb_log_open (mydvb_log_level_t log_level, const char *filename);
void mydvb_log_close ();

void mydvb_log (mydvb_log_level_t log_level, const char *format, ...);


#endif
