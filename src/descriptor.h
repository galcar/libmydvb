#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include <stdio.h>

#include <mydvb_descriptors.h>

#include <mydvb_tables.h>

#include "dynarray.h"

int 				parse_descriptor (TABLE_TYPE_T t, const unsigned char *buffer, mydvb_descriptor *des);

void 				free_descriptor (void *obj);

void 				save_descriptors (FILE *f, DYN_ARRAY *descriptors);


#endif
