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
#include <mydvb.h>

#include "descriptor.h"

/**
 * check if a program is valid. a program is valid if it has, at least, one stream
 */
int mydvb_is_program_valid (MYDVB_PROGRAM *program) {

	int len;

	if (program==NULL) {
		return 0; // not valid
	}

	/* a program is valid if it has, at least, one stream */
	len = dyn_array_get_size (program->streams);
	if (len >= 1) {
		return 1;
	}
	
	return 0;
}

MYDVB *mydvb_get_channel (MYDVB_ENGINE *engine, mydvb_tuner_type_t type, int channel) {
	if (engine == NULL) {
		return NULL;
	}

	for (int i = 0; i < dyn_array_get_size (engine->channels); i++) {
		MYDVB *mydvb = *((MYDVB **) dyn_array_get_data (engine->channels,i));

		if (mydvb->type == type && mydvb->channel == channel) {
			return mydvb;
		}
	}

	return NULL;
}

/**
 * return the program at position pos in channel of a tuner type
 */
MYDVB_PROGRAM *mydvb_get_program (MYDVB_ENGINE *engine, mydvb_tuner_type_t type, int channel, int pos) {

	if (engine==NULL) {
		return NULL;
	}

	MYDVB *mydvb = mydvb_get_channel (engine, type, channel);

	return mydvb_get_program_in_channel (mydvb, pos);

}

/** 
 * return the program at position pos in channel
 */
MYDVB_PROGRAM *mydvb_get_program_in_channel (MYDVB *mydvb, int pos) {

	if (mydvb==NULL) {
		return NULL;
	}

	if (mydvb->pat==NULL) {
		return NULL;
	}

	int len = dyn_array_get_size (mydvb->pat->programs);
	if (len == 0 || pos >= len) {
		return NULL;
	}

	return dyn_array_get_data (mydvb->pat->programs, pos);

}


/**
 * return the stream at position pos in program
 */
MYDVB_STREAM *mydvb_get_stream (MYDVB_PROGRAM *program, int pos) {

	if (program==NULL) {
		return NULL;
	}

	int len = dyn_array_get_size (program->streams);
	if (len==0 || pos >= len) {
		return NULL;
	}
	
	return dyn_array_get_data (program->streams, pos);
	
}

/**
 * Returns the service name of the program at position pos in channel
 */
mydvb_text *mydvb_get_service_name (MYDVB *mydvb, int pos) {
	int j;

	MYDVB_PROGRAM *program;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor = NULL;
	int len, len2;

	program = mydvb_get_program_in_channel (mydvb, pos);
	if (program==NULL) {
		return NULL;
	}
	
	len = dyn_array_get_size (mydvb->sdt->service_description);
	for (j=0; j < len; j++) {
		sd = dyn_array_get_data (mydvb->sdt->service_description, j);
		if (sd->number == program->number) {
			len2 = dyn_array_get_size (sd->descriptors);
			descriptor = mydvb_descriptor_find (sd->descriptors, 72);
			if (descriptor != NULL) {
				return descriptor->service.service_name;
			}
			break;
		}
	}

	return NULL;
}

/**
 * returns the provider name of the program at position pos in channel
 */
mydvb_text *mydvb_get_provider_name (MYDVB *mydvb, int pos) {
	int j;

	MYDVB_PROGRAM *program;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor = NULL;
	int len, len2;

	program = mydvb_get_program_in_channel (mydvb, pos);
	if (program==NULL) {
		return NULL;
	}

	len = dyn_array_get_size (mydvb->sdt->service_description);
	for (j=0; j < len; j++) {
		sd = dyn_array_get_data (mydvb->sdt->service_description, j);
		if (sd->number == program->number) {
			len2 = dyn_array_get_size (sd->descriptors);
			descriptor = mydvb_descriptor_find (sd->descriptors, 72);
			if (descriptor != NULL) {
				return descriptor->service.provider_name;
			}
			break;
		}
	}

	return NULL;
}




int mydvb_get_len_audio_stream (MYDVB_PROGRAM *program) {

	MYDVB_STREAM *stream;
	int j, len;	
	int cont = 0;

	if (program == NULL) {
		return 0;
	}

	len = dyn_array_get_size (program->streams);
	for (j=0; j < len; j++) {
		stream = dyn_array_get_data (program->streams, j);
		if (stream==NULL) {
			continue;
		}
		if (stream->type == 0x03 || stream->type == 0x04) { /* this is an audio stream */
			cont ++;
		}
	}
	
	return cont;
	
}

int mydvb_get_len_video_stream (MYDVB_PROGRAM *program) {

	MYDVB_STREAM *stream;
	int j, len;	
	int cont = 0;

	if (program == NULL) {
		return 0;
	}

	len = dyn_array_get_size (program->streams);
	for (j=0; j < len; j++) {
		stream = dyn_array_get_data (program->streams, j);
		if (stream==NULL) {
			continue;
		}
		if (stream->type == 0x01 || stream->type == 0x02) { /* this is a video stream */
			cont ++;
		}
	}
	
	return cont;
	
}

int mydvb_get_len_txt_stream (MYDVB_PROGRAM *program) {

	MYDVB_STREAM *stream;
	int j, len;	
	int cont = 0;

	if (program == NULL) {
		return 0;
	}

	len = dyn_array_get_size (program->streams);
	for (j=0; j < len; j++) {
		stream = dyn_array_get_data (program->streams, j);
		if (stream==NULL) {
			continue;
		}
		if (stream->type == 0x06) { /* this is a teletext stream */
			cont ++;
		}
	}
	
	return cont;
	
}

unsigned char mydvb_stream_get_component_tag (MYDVB_STREAM *stream) {
	mydvb_descriptor *d;
	unsigned char ctag = 0x00;
	
	d = mydvb_descriptor_find (stream->descriptors, 0x52);
	if (d!=NULL) {
		ctag = d->stream_id.component_tag;
		
	}
	
	return ctag;

}

mydvb_service_description *mydvb_get_service_description (MYDVB *mydvb, int service_number) {

	int len, i;
	mydvb_service_description *sd = NULL;

	if (mydvb == NULL || mydvb->sdt == NULL) {
		return NULL;
	}

	len = dyn_array_get_size (mydvb->sdt->service_description);

	for (i = 0; i < len; i++) {

		sd = dyn_array_get_data (mydvb->sdt->service_description, i);

		if (sd->number == service_number) {
			return sd;
		}
	}

	return NULL;
}
