#ifndef __MYDVB_LOG_H__
#define __MYDVB_LOG_H__

#include <mydvb_tables.h>

typedef enum {
	MYDVB_LOG_LEVEL_DEBUG = 0,
	MYDVB_LOG_LEVEL_ERROR,
	MYDVB_LOG_LEVEL_INFO,
	MYDVB_LOG_LEVEL_NONE = 10
} mydvb_log_level_t;

char **mydvb_log_levels_table();
mydvb_log_level_t mydvb_log_parse_level (char *type);

void mydvb_log_open (mydvb_log_level_t log_level, const char *filename);
void mydvb_log_close ();

void mydvb_log_set_level (mydvb_log_level_t new_log_level);

void mydvb_log (mydvb_log_level_t log_level, const char *format, ...);

void mydvb_show_pat (MYDVB_PAT *pat);
void mydvb_show_sdt (MYDVB_SDT *sdt);

#endif
