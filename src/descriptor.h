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
