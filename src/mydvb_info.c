
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

/** 
 * esta funcion devuelve un programa o servicio por numero
 */
MYDVB_PROGRAM *mydvb_get_program_by_number (MYDVB *mydvb, int number) {
	MYDVB_PROGRAM *program = NULL;
	int len;

	if (mydvb==NULL) {
		return NULL;
	}

	if (mydvb->pat==NULL) {
		return NULL;
	}

	len = dyn_array_get_size (mydvb->pat->programs); 
	if (len==0) {
		return NULL;
	}
	if (number >= len) {
		return NULL;
	}

	program = dyn_array_get_data (mydvb->pat->programs, number);
	return program;

}


/**
 * esta funcion selecciona un stream por numero, para ello busca el stream 
 * numero "number" dentro del programa
 */
MYDVB_STREAM *mydvb_get_stream_by_number (MYDVB_PROGRAM *program, int number) {
	MYDVB_STREAM *stream = NULL;
	int len;

	if (program==NULL) {
		return NULL;
	}

	len = dyn_array_get_size (program->streams);
	if (len==0) {
		return NULL;
	}
	
	if (number >= len) {
		return NULL;
	}
	
	stream = dyn_array_get_data (program->streams, number);
	return stream;	
	
}

/**
 * Devuelve el nombre del servicio o programa con el número indicado
 */
mydvb_text *mydvb_get_service_name (MYDVB *mydvb, int number) {
	int j;

	MYDVB_PROGRAM *program;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor = NULL;
	int len, len2;

	program = mydvb_get_program_by_number (mydvb, number);
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

mydvb_text *mydvb_get_provider_name (MYDVB *mydvb, int number) {
	int j;

	MYDVB_PROGRAM *program;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor = NULL;
	int len, len2;

	program = mydvb_get_program_by_number (mydvb, number);
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
