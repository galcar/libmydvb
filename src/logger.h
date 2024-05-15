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
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

#include <mydvb_log.h>

#include <mydvb_tables.h>
#include <mydvb_mhp_tables.h>
#include <dynarray.h>
#include <mydvb_descriptors.h>


typedef struct {

	char 				*filename;

	FILE 				*file;

	mydvb_log_level_t 	log_level;

} MYDVB_LOGGER;

void mydvb_hex_log (int log_level, const unsigned char *buffer, size_t len);
void mydvb_show_eit (const unsigned char *buffer, size_t len);

void mydvb_show_nit (MYDVB_NIT *nit);
void mydvb_show_program (MYDVB_PROGRAM *program);
void mydvb_show_stream (MYDVB_STREAM *stream);

void mydvb_show_aits (DYN_ARRAY *aits);
void mydvb_show_ait (MYDVB_AIT *ait);

void mydvb_show_descriptors (DYN_ARRAY *descriptors, int n);
void mydvb_show_descriptor (mydvb_descriptor *d, int n);
void mydvb_show_0x48_descriptor (mydvb_service_descriptor *d, int n);
void mydvb_show_0x52_descriptor (mydvb_stream_id_descriptor *d, int n);
void mydvb_show_0x66_descriptor (mydvb_data_broadcast_id_descriptor *d, int n);

#endif
