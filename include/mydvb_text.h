#ifndef __MYDVB_TEXT_H__
#define __MYDVB_TEXT_H__

#include <stdio.h>
#include <stdint.h>

/**
 * iso name
 */
typedef struct {
	unsigned char	*	lang;	/* iso 639 language code used in name */
	unsigned int		length;	/* text length */
	uint32_t		*	text;	/* unicode string */
} mydvb_text;


mydvb_text *mydvb_text_create (const unsigned char *buf, size_t len);

mydvb_text *mydvb_text_dup (mydvb_text *txt);

void mydvb_text_free (mydvb_text *txt);

void mydvb_text_printf (mydvb_text *txt, FILE *f);

/** return a null terminated string in utf8, that must be freed */
char *mydvb_text_to_utf8 (mydvb_text *t);

#endif
