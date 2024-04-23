

/*
	Decoding of tables
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "tables.h"

#include "descriptor.h"

#include "logger.h"

#include "util.h"

#include <mydvb_date.h>

#define bcdtoint(i) ((((i & 0xf0) >> 4) * 10) + (i & 0x0f))

MYDVB_PAT *mydvb_new_pat (unsigned char *buffer, size_t len) {
	return mydvb_new_pat_if_changed (buffer, len, NULL);
}


MYDVB_PAT *mydvb_new_pat_if_changed (unsigned char *buffer, size_t len, MYDVB_PAT *pat) {
	const unsigned char *bufptr;
	int j;
	unsigned char new_version;
	unsigned short number;
	MYDVB_PROGRAM *program = NULL;
	int cont;

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding PAT");

	mydvb_hex_log (1, buffer, len);

	if (pat==NULL) {
		pat = (MYDVB_PAT *) malloc (sizeof(MYDVB_PAT));
		pat->ts_id = 0xffff;
		pat->version = 0xff;
		pat->nit = NULL;
		pat->programs = NULL;
	} 

	new_version = getbits (buffer, 42, 5);

	if (new_version == pat->version) { /* no hay cambios, regreso */
		return pat;
	}

	pat->ts_id			= getbits (buffer, 24, 16);
	pat->version   		= new_version;
	pat->section_n 		= getbits (buffer, 48, 8);
	pat->section_n_l 	= getbits (buffer, 56, 8);

	//pat->num_programs = (len - 12) / 4;
	dyn_array_free (pat->programs);
	pat->programs = dyn_array_new (sizeof (MYDVB_PROGRAM));
	mydvb_delete_nit (pat->nit);
	pat->nit = NULL;

	bufptr = buffer + 8;
	cont = len - 8;
	while (cont > 4) {
		number = getbits (bufptr, 0, 16);
		if (number==0) { /* esta es la nit */
			pat->nit = (MYDVB_NIT *) malloc (sizeof(MYDVB_NIT));
			pat->nit->pid = getbits (bufptr, 19, 13);
			pat->nit->network_id = 0xffff;
			pat->nit->version = 0xff;
			pat->nit->descriptors = NULL;
			pat->nit->transport_desc = NULL;
			/* possible nit callback here */
		} else { /* esto es un programa */
			program = dyn_array_add_empty (pat->programs);

			program->version = 0xff;
			program->number  = number;
			program->pmt_pid = getbits (bufptr, 19, 13);

			program->descriptors = NULL;
			program->streams = NULL;
			program->eit = NULL;
			for (j=0; j < 16; j++) {
				program->eit_sched[j] = NULL;
			}
			program->ready = 0;
			
			/* possible program callback here */
		}
		bufptr += 4;
		cont   -= 4;
	}

	/* los cuatro ultimos bytes son del crc */
		
	return pat;
}

void mydvb_delete_pat (MYDVB_PAT *pat) {
	int i;
	int len;
	MYDVB_PROGRAM *program;

	if (pat == NULL) {
		return;
	}

	/* delete the nit */
	mydvb_delete_nit (pat->nit);
	pat->nit = NULL;

	/* and delete all programs */
	len = dyn_array_get_size(pat->programs);
	for (i=0; i < len; i++) {
		program = dyn_array_get_data (pat->programs, i);
		mydvb_delete_program (program);
	}
	dyn_array_free (pat->programs);
	pat->programs = NULL;

	/* at least, free the pat */
	free (pat);
}

void mydvb_save_pat (FILE *f, MYDVB_PAT *pat) {
	/*
		unsigned short ts_id;
		unsigned char version;
		short section_n;
		short section_n_l;

		MYDVB_NIT *nit;

		DYN_ARRAY *programs;

	 */
	int i;

	fwrite (pat, sizeof(MYDVB_PAT), 1, f);

	mydvb_save_nit (f, pat->nit);

	for (i = 0; i < dyn_array_get_size (pat->programs); i++) {
		mydvb_save_program (f, dyn_array_get_data(pat->programs, i));
	}

}

void __free_transport_description (void *p) {

	mydvb_transport_description *transport_desc = (mydvb_transport_description *) p;

	if (transport_desc == NULL) {
		return;
	}

	dyn_array_free (transport_desc->descriptors);
	transport_desc->descriptors = NULL;
}

/* ---------- Network Information Table (NIT) ---------------- */
int mydvb_edit_nit (MYDVB_NIT *nit, const unsigned char *buffer, size_t len) {

	unsigned char new_version;
	int dl, counter, counter2;
	const unsigned char *bufptr;
	mydvb_descriptor *descriptor;
	mydvb_transport_description *ts_desc;

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding NIT");

	mydvb_hex_log (1, buffer, len);

	if (nit==NULL) {
		return 0;
	}

	new_version = getbits (buffer, 42, 5);

	if (nit->version == new_version) {
		// bye bye unchanged
		return 0;
	}

	nit->network_id 	= getbits (buffer, 24, 16);	
	nit->version 		= new_version;
	nit->section_n 		= getbits (buffer, 48, 8);
	nit->section_n_l 	= getbits (buffer, 56, 8);

	/* network descriptors */
	dyn_array_free (nit->descriptors);
	nit->descriptors = dyn_array_new (sizeof (mydvb_descriptor));
	dyn_array_set_free_callback (nit->descriptors, free_descriptor);

	counter  	= getbits (buffer, 68, 12);
	bufptr 		= buffer + 10;
	while (counter > 0) {

		descriptor = dyn_array_add_empty (nit->descriptors);
		dl = parse_descriptor (NIT, bufptr, descriptor);
		/* possible event callback here */

		bufptr 	+= dl;
		counter -= dl;
	}

	/* network transport descriptions */
	dyn_array_free (nit->transport_desc);
	nit->transport_desc = dyn_array_new (sizeof(mydvb_transport_description));
	dyn_array_set_free_callback (nit->transport_desc, __free_transport_description);

	counter = getbits (bufptr, 4, 12);
	bufptr += 2;
	while (counter > 0) {
		ts_desc = dyn_array_add_empty (nit->transport_desc);
		ts_desc->ts_id 			 = getbits (bufptr, 0, 16);
		ts_desc->original_net_id = getbits (bufptr,	16, 16);

		ts_desc->descriptors = dyn_array_new (sizeof (mydvb_descriptor));
		dyn_array_set_free_callback (ts_desc->descriptors, free_descriptor);

		counter2 = getbits (bufptr, 36, 12);
		counter -= 6;
		bufptr += 6;
		
		/* descriptores */
		while (counter2 > 0) {
			descriptor = dyn_array_add_empty (ts_desc->descriptors);
			dl = parse_descriptor (NIT, bufptr, descriptor);
			/* possible nit descriptor callback here */			

			counter2 -= dl;	
			bufptr   += dl;
			counter  -= dl;
		}
		/* possible transport description callback here */
	}
	
	return 1;
}

void mydvb_delete_nit (MYDVB_NIT *nit) {

	if (nit==NULL) {
		return;
	}

	dyn_array_free (nit->descriptors);
	nit->descriptors = NULL;

	dyn_array_free (nit->transport_desc);
	nit->transport_desc = NULL;

	free (nit);
}

void mydvb_save_nit (FILE *f, MYDVB_NIT *nit) {
/*
typedef struct {
	unsigned short network_id;
	unsigned char version;
	short section_n;
	short section_n_l;

	DYN_ARRAY *descriptors;

	DYN_ARRAY *transport_desc;

	unsigned short pid;

} MYDVB_NIT;
 */
	if (nit == NULL) {
		return;
	}

	fwrite (nit, sizeof(MYDVB_NIT), 1, f);

	save_descriptors (f, nit->descriptors);

	save_descriptors (f, nit->transport_desc);

}

int mydvb_edit_program (MYDVB_PROGRAM *pmt, const unsigned char *buffer, size_t len) {
	const unsigned char *bufptr;
	int counter, counter2, dl;
	MYDVB_STREAM *stream;
	unsigned char new_version;
	mydvb_descriptor *descriptor;

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding PMT");

	mydvb_hex_log (1, buffer, len);

	if (pmt==NULL) {
		return 0;
	}

	new_version = getbits (buffer, 42,5);

	if (pmt->version == new_version) {
		// bye bye
		return 0; // unchanged
	}

	pmt->version 		= new_version;
	pmt->section_n 	= getbits (buffer, 48, 8); /* shall be 0x00 */
	pmt->section_n_l 	= getbits (buffer, 56, 8); /* shall be 0x00 */

	pmt->pcr_pid = getbits (buffer, 67, 13);

	/* pmt descriptors */

	dyn_array_free (pmt->descriptors);
	pmt->descriptors = dyn_array_new (sizeof (mydvb_descriptor));
	dyn_array_set_free_callback (pmt->descriptors, free_descriptor);
	
	counter2  = getbits (buffer, 84, 12);
	bufptr = buffer + 12;
	counter = len - 12;
	while (counter2 > 0) {
		descriptor = dyn_array_add_empty (pmt->descriptors);
		dl = parse_descriptor (PMT, bufptr, descriptor);

		/* possible descriptor callback here */

		counter2 -= dl;
		bufptr += dl;
		counter -= dl;
	}

	/* streams */
	dyn_array_free (pmt->streams);
	pmt->streams = dyn_array_new (sizeof (MYDVB_STREAM));
	while (counter > 4) {
		stream = dyn_array_add_empty (pmt->streams);

		stream->type   		= getbits (bufptr, 0, 8);
		stream->pid    		= getbits (bufptr, 11, 13);

		/* descriptores del stream */
		stream->descriptors = dyn_array_new(sizeof (mydvb_descriptor));
		dyn_array_set_free_callback (stream->descriptors, free_descriptor);
		
		counter2 = getbits (bufptr, 28, 12);
		bufptr += 5;
		counter -= 5;
		while (counter2 > 0) {
			/* leemos los descriptores del stream */
			descriptor = dyn_array_add_empty (stream->descriptors);

			dl = parse_descriptor (PMT, bufptr, descriptor);
			/* posible descriptor callback here */

			counter2 -= dl;
			bufptr += dl;
			counter -= dl;
		}
		/* possible stream callback here */
	}
	pmt->ready = 1; // la pmt para este programa esta lista

	/* crc 32 bits*/
	
	return 1; // changed
}

void mydvb_delete_program (MYDVB_PROGRAM *pmt) {
	MYDVB_STREAM *s;
	int i;
	int len;
	
	if (pmt == NULL) {
		return;
	}

	dyn_array_free (pmt->descriptors);
	pmt->descriptors = NULL;

	len = dyn_array_get_size (pmt->streams);
	for (i=0; i < len; i++) {
		s = dyn_array_get_data (pmt->streams, i);

		dyn_array_free (s->descriptors);
		s->descriptors = NULL;
	}
	dyn_array_free (pmt->streams);
	pmt->streams = NULL;

}

void mydvb_save_program (FILE *f, MYDVB_PROGRAM *program) {
	/*
	unsigned short number;
	unsigned short pmt_pid;

	unsigned char version;

	short section_n;
	short section_n_l;

	unsigned short pcr_pid;

	DYN_ARRAY *descriptors;

	DYN_ARRAY *streams;

	MYDVB_EIT *eit;
	MYDVB_EIT *eit_sched[16];

	int ready;

	 *
	 */

	int i;

	if (program == NULL) {
		return;
	}

	fwrite (program, sizeof(MYDVB_PROGRAM), 1, f);

	save_descriptors (f, program->descriptors);

	for (i=0; i < dyn_array_get_size (program->streams); i++) {
		mydvb_save_stream (f, dyn_array_get_data (program->streams, i));
	}

}

void mydvb_save_stream (FILE *f, MYDVB_STREAM *stream) {
	/*
		unsigned char type;
		unsigned short pid;

		DYN_ARRAY *descriptors;

	*/

	if (stream == NULL) {
		return;
	}

	fwrite (stream, sizeof(MYDVB_STREAM), 1, f);

	save_descriptors (f, stream->descriptors);

}

MYDVB_SDT *mydvb_new_sdt_if_changed (const unsigned char *buffer, size_t len, MYDVB_SDT *sdt) {
	const unsigned char *bufptr;
	int count;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor;
	unsigned char new_version=0;

	int descriptors_len;
	int dl;

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding SDT");

	mydvb_hex_log (1, buffer, len);

	if (sdt==NULL) {
		sdt = (MYDVB_SDT *) malloc (sizeof(MYDVB_SDT));
		sdt->ts_id = 0xffff;
		sdt->version = 0xff;
		sdt->service_description = NULL;

	}

	new_version = getbits (buffer, 42, 5);
	if (new_version == sdt->version) { 
		// ok, la sdt es la misma q ya tenemos, bye bye
		return sdt;
	}

	sdt->ts_id	   			= getbits (buffer, 24, 16);
	sdt->version   			= new_version;
	sdt->original_net_id 	= getbits (buffer, 64, 16);

	dyn_array_free (sdt->service_description);

	sdt->service_description = dyn_array_new (sizeof(mydvb_service_description));
	dyn_array_set_free_callback (sdt->service_description, free_service_description);

	bufptr = buffer + 11;
	count = len - 11;
	while (count > 4) {
	
		sd = dyn_array_add_empty (sdt->service_description);

		sd->number 				= getbits (bufptr, 0, 16); /* service id */
		sd->eit					= getbits (bufptr, 22, 1); /* eit flag */
		sd->eit_pw				= getbits (bufptr, 23, 1); /* eit present/following flag */
		sd->running_status 	= getbits (bufptr, 24, 3);
		sd->scram 				= getbits (bufptr, 27, 1);

		// read service_descriptor table
		descriptors_len 	= getbits (bufptr, 28, 12);
		sd->descriptors	= dyn_array_new (sizeof (mydvb_descriptor));
		dyn_array_set_free_callback (sd->descriptors, free_descriptor);
		
		bufptr += 5; 
		count  -= 5;
		while (descriptors_len > 0) {
			//descriptor
			descriptor = dyn_array_add_empty (sd->descriptors);

			dl = parse_descriptor (SDT, bufptr, descriptor);
			/* possible descriptor callback here */

			bufptr += dl;
			count  -= dl;
			descriptors_len -= dl;				
		}			
		/* possible service description callback here */
	}

	return sdt;
}

void mydvb_delete_sdt (MYDVB_SDT *sdt) {

	if (sdt == NULL) {
		return;
	}

	dyn_array_free (sdt->service_description);
	sdt->service_description = NULL;

	free (sdt);
}

void free_service_description (void *obj) {

	mydvb_service_description *sd = (mydvb_service_description *) obj;

	dyn_array_free (sd->descriptors);
	sd->descriptors = NULL;
}

/* ----------------- EIT ------------------ */

void __mydvb_free_eit_entry (void *p) {
	MYDVB_EIT_ENTRY *eit_e = (MYDVB_EIT_ENTRY *) p;

	if (eit_e == NULL) {
		return;
	}
	mydvb_date_free (eit_e->start_time);
	eit_e->start_time = NULL;
}

void mydvb_new_eit_if_changed (MYDVB_PAT *pat, const unsigned char *buffer, size_t len) {
	const unsigned char *bufptr;
	int count;
	unsigned short number;
	unsigned char new_version=0;
	MYDVB_EIT_ENTRY *eit_e = NULL;
	MYDVB_EIT *eit = NULL;
	MYDVB_PROGRAM *program =NULL;
	int table_id;
	unsigned char section_number;

	mydvb_descriptor *descriptor;
	int descriptors_len;
	int dl;

	int i;
	int dyn_len;
	int encontrado;
		


	if (pat==NULL) {
		return;
	}
	if (pat->programs==NULL) {
		return;
	}

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT");
	//mydvb_show_eit (buffer, len);
	
	/* obtiene el numero de programa y extrae la EIT de dicho programa */
	number = getbits (buffer, 24, 16);
	encontrado = 0;
	dyn_len = dyn_array_get_size (pat->programs);
	for (i=0; i < dyn_len; i++) {
		program = dyn_array_get_data (pat->programs, i);
		if (program->number == number) {
			encontrado = 1;
			break;
		}
	}

	if (!encontrado) {
		/* service not found */
		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT program %d not found", number);
		return;
	}

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> Program found");

	/* find the eit table */
	table_id = getbits (buffer, 0, 8);
	switch (table_id) {
		case 0x4e: /* actual ts, present/following */
			eit = program->eit;
			break;
		case 0x50 ... 0x5f: /* actual ts, schedule */
			eit = program->eit_sched[table_id & 0x0f];
			break;
		default: /* para otros ts no se procesa */
			//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> other TS(%d) not implemented", table_id);
			return;
	}

	new_version = getbits (buffer, 42, 5);

	section_number = getbits (buffer, 48, 8);

	if (eit != NULL && new_version == eit->version && eit->incomplete == 0) {
		/* there is a completed eit with the same version */
		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT eit exists and is complete");
		return;
	}

	if (eit==NULL) {
		/* there is not eit, we have to create one */
		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> new eit with table_id %d", table_id);

		eit = (MYDVB_EIT *) malloc (sizeof(MYDVB_EIT));

		eit->entries = NULL;
		eit->number = number;
		eit->version = 0xFF;

		switch (table_id) {
			case 0x4e: /* actual ts, present/following */
				program->eit = eit;
				break;
			case 0x50 ... 0x5f: /* actual ts, schedule */
				program->eit_sched[table_id & 0x0f] = eit;
				break;
			default: /* ??? */
				free (eit);
				return;
		}

	}

	section_number = getbits (buffer, 48, 8);

	if (new_version != eit->version) {
		/* there is an eit but with a differente version number */
		eit->version = new_version;
		eit->incomplete = 2;
		eit->initial_section_number = section_number;

		dyn_array_free (eit->entries);
		eit->entries = NULL;
	}

	if (section_number == eit->initial_section_number) {
		eit->incomplete --;

		if (eit->incomplete == 0) {
			return;
		}
	}


	if (eit->entries==NULL) {
		eit->entries = dyn_array_new (sizeof(MYDVB_EIT_ENTRY));
		dyn_array_set_free_callback (eit->entries, __mydvb_free_eit_entry);
	}

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> version %d, section %d", new_version, section_number);

	bufptr = buffer + 14;
	count = len - 14;
	while (count > 4) {
		eit_e = dyn_array_add_empty (eit->entries);

		eit_e->event_id 		= getbits (bufptr, 0, 16);

		eit_e->start_time		= mydvb_date_create (bufptr + 2);
		eit_e->duration 		= bcdtoint(bufptr[7])*3600 
			+ bcdtoint(bufptr[8])*60
			+ bcdtoint(bufptr[9]);
		eit_e->running_status 	= getbits (bufptr, 80, 3);
		eit_e->scram 			= getbits (bufptr, 83, 1);

		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> event %d, running %d", eit_e->event_id ,eit_e->running_status);

		/* descriptores */
		eit_e->descriptors 		= dyn_array_new(sizeof (mydvb_descriptor));
		dyn_array_set_free_callback (eit_e->descriptors, free_descriptor);
		descriptors_len 		= getbits (bufptr, 84, 12);
		// lee service_descriptor table
		bufptr += 12; 
		count  -= 12;
		while (descriptors_len > 0) {
			//descriptor
			descriptor = dyn_array_add_empty (eit_e->descriptors);

			/*
			mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> event %d, parsing descriptor %X %d"
					, eit_e->event_id
					, bufptr[0]
					, bufptr[1]+2);
			*/

			dl = parse_descriptor (EIT, bufptr, descriptor);

			//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> parsed ok");

			/* posible descriptor callback here */
			bufptr += dl;
			count  -= dl;
			descriptors_len -= dl;				
		}			
		/* posible event entry callback here */
		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT -> event ok");
	}
	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding EIT ok");
}

/* ---------------- CAT --------------------- */
/* decodifica la tabla de acceso condicional */
MYDVB_CAT *mydvb_new_cat_if_changed (const unsigned char *buffer, size_t len, MYDVB_CAT *cat) {
	unsigned char new_version;
	const unsigned char *bufptr;
	int counter;
	int dl;
	mydvb_descriptor *descriptor;

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding CAT");

	mydvb_hex_log (1, buffer, len);


	if (cat==NULL) {
		cat = (MYDVB_CAT *) malloc (sizeof(MYDVB_CAT));
		cat->descriptors = NULL;
		cat->version 	 = 0xff;
	}

	new_version = getbits (buffer, 42, 5);
	
	if (cat->version==new_version) {
		return cat;
	}

	cat->version = new_version;
	cat->section_n 	= getbits (buffer, 48, 8);	
	cat->section_n_l 	= getbits (buffer, 56, 8);

	/* descriptors */
	dyn_array_free (cat->descriptors);
	cat->descriptors = dyn_array_new (sizeof (mydvb_descriptor));
	dyn_array_set_free_callback (cat->descriptors, free_descriptor);

	bufptr = buffer + 7;
	counter = len - 7;
	while (counter > 4) {

		descriptor = dyn_array_add_empty (cat->descriptors);
		dl = parse_descriptor (CAT, bufptr, descriptor);
		/* possible event callback here */
		/* si existe el descriptor CA_descriptor (is138181, pp. 76), entonces este
			descriptor contiene el PID de los paquetes que llevan la EMM */

		bufptr 	+= dl;
		counter -= dl;
	}

	return cat;

}

void mydvb_delete_cat (MYDVB_CAT *cat) {

	if (cat==NULL) {
		return;
	}

	dyn_array_free (cat->descriptors);

	free (cat);
	
}

/* ---------------------- TDT -------------------- */
/* decodifica la tabla de fecha y hora (TDT) */
void mydvb_tdt (const unsigned char *buffer, size_t len, MYDVB_TDT *tdt) {

	mydvb_date *d;
	struct tm tm;

	//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Decoding TDT");

	mydvb_hex_log (1, buffer, len);

	d = mydvb_date_create (buffer+3);

	tdt->t = mydvb_date_to_tm (d, &tm);

	mydvb_date_free (d);
	/* possible callback here */	
}
