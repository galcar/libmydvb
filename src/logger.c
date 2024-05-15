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
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>

#include "logger.h"

#include "util.h"

#include "mydvb_text.h"

/* global variable for logger */
MYDVB_LOGGER *mydvb_logger = NULL;

static char *__mydvb_log_levels_table[] = {
		"debug",
		"error",
		"info",
		NULL
};

char **mydvb_log_levels_table() {
	return __mydvb_log_levels_table;
}

mydvb_log_level_t mydvb_log_parse_level (char *type) {

	mydvb_log_level_t i = 0;

	while (__mydvb_log_levels_table[i] != NULL) {
		if (strcasecmp (__mydvb_log_levels_table[i], type) == 0) {
			return i;
			break;
		}
		i++;
	}
	return MYDVB_LOG_LEVEL_NONE;
}

/**
 *
 */
void mydvb_log (mydvb_log_level_t log_level, const char *format, ...) {
	va_list ap;

	FILE *f = NULL;

	if (mydvb_logger == NULL) {
		return;
	}

	if (log_level < mydvb_logger->log_level) {
		return;
	}

	f = mydvb_logger->file;

	va_start(ap, format);
	vfprintf (f, format, ap);
	fprintf (f,"\n");
	va_end (ap);

	if (log_level <= MYDVB_LOG_LEVEL_ERROR && errno < 0) {
		/* show errno */
		fprintf (f, "Errno: %s\n", strerror (errno));
	}

	fflush (f);

}

void mydvb_log_open (mydvb_log_level_t log_level, const char *filename) {


	if (mydvb_logger != NULL) {
		mydvb_log_close ();
	}

	mydvb_logger = (MYDVB_LOGGER *) malloc (sizeof(MYDVB_LOGGER));

	mydvb_logger->log_level = log_level;

	if (filename == NULL) {

		mydvb_logger->file = stdout;

	} else {

		mydvb_logger->filename = strdup (filename);

		mydvb_logger->file = fopen (mydvb_logger->filename, "a");

	}

}


void mydvb_log_close () {

	if (mydvb_logger == NULL) {
		return;
	}

	if (mydvb_logger->filename != NULL) {
		free (mydvb_logger->filename);

		mydvb_logger->filename = NULL;

		fclose (mydvb_logger->file);

		mydvb_logger->file = NULL;
	}

	free (mydvb_logger);

	mydvb_logger = NULL;
}

void mydvb_log_set_level (mydvb_log_level_t new_log_level) {
	if (mydvb_logger) {
		mydvb_logger->log_level = new_log_level;
	}
}

void mydvb_hex_log (int log_level, const unsigned char *buffer, size_t len) {
	int i,j;
	
	if (log_level==1) {
		return;
	}	
	
	j=0;
	for (i=0; i < len; i++) {
		printf ("%.2x ", (unsigned char) buffer[i]);
		j++;
		if (j==16) {
			j=0;
			printf ("\n");
		}
	}
	if (j != 0) {
		printf ("\n");
	}
}

void mydvb_show_eit (const unsigned char *buffer, size_t len) {
	unsigned char table_id, ssi, version, cni, section_num, last_section_num;
	unsigned char segment_lsn, last_table_id;
	unsigned short section_len, number, ts_id, on_id;

	table_id = getbits (buffer, 0, 8);
	ssi = getbits (buffer, 8, 1); 
	section_len = getbits (buffer, 12, 12);
	number = getbits (buffer, 24, 16);
	version = getbits (buffer, 42, 5);
	cni = getbits (buffer, 47, 1);
	section_num = getbits (buffer, 48, 8);
	last_section_num = getbits (buffer, 56, 8);
	ts_id = getbits (buffer, 64, 16);
	on_id = getbits (buffer, 80, 16);
	segment_lsn = getbits (buffer, 96, 8);
	last_table_id = getbits (buffer, 104, 8);
	
	printf ("Total len: %d\n", len);
	printf ("Table id: %.2X\n", table_id);
	printf ("Section syntax indicator: %d\n", ssi);
	printf ("Section length %d\n", section_len);
	printf ("Service id %d\n", number);
	printf ("Version number %d\n", version);
	printf ("Current/Next indicator: %d\n", cni);
	printf ("Section number: %d\n", section_num);
	printf ("Last section number: %d\n", last_section_num);
	printf ("Transport Stream id: %d\n", ts_id);
	printf ("Original Network id: %d\n", on_id);
	printf ("Segment last section number: %d\n", segment_lsn);
	printf ("Last table id: %.2X\n", last_table_id);
	printf ("---\n");

}

void mydvb_show_pat (MYDVB_PAT *pat) {
	MYDVB_PROGRAM *program;
	int len, i;

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "PAT: Transport Stream ID: %d; Version: %d; Sections: %d-%d", pat->ts_id, pat->version, pat->section_n, pat->section_n_l);
	
	/* print nit */
	if (pat->nit!=NULL) {
		mydvb_show_nit (pat->nit);	
	}
	
	len = dyn_array_get_size (pat->programs);
	for (i=0; i < len; i++) {
		program = (MYDVB_PROGRAM *) dyn_array_get_data (pat->programs, i);
		mydvb_show_program (program);
	}
}

void mydvb_show_nit (MYDVB_NIT *nit) {
	mydvb_transport_description *td;
	int len, i;

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "-NIT: Network ID: %d; Version: %d; PID: %d; Sections: %d-%d", nit->network_id, nit->version, nit->pid, nit->section_n, nit->section_n_l);
	
	mydvb_show_descriptors (nit->descriptors, 2);

	len = dyn_array_get_size (nit->transport_desc);
	mydvb_log (MYDVB_LOG_LEVEL_INFO, "--Transport Description (total %d)", len);
	for (i=0; i < len; i++) {
		td = (mydvb_transport_description *) dyn_array_get_data (nit->transport_desc, i);
		if (td==NULL) {
			continue;
		}
		mydvb_log (MYDVB_LOG_LEVEL_INFO, "---Transport Description Stream ID: %d, Original Network ID: %d", td->ts_id, td->original_net_id);

		mydvb_show_descriptors (td->descriptors, 4);
	}

}

void mydvb_show_program (MYDVB_PROGRAM *program) {
	MYDVB_STREAM *stream;
	int i, len;

	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Program number: %d", program->number);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"--pid: %d", program->pmt_pid);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"--Version: %d", program->version);
	
	/*

	unsigned short pcr_pid;	

	*/
	mydvb_show_descriptors (program->descriptors, 2);	
	
	len = dyn_array_get_size (program->streams);
	for (i=0; i < len; i++) {
		stream = (MYDVB_STREAM *) dyn_array_get_data (program->streams, i);
		mydvb_show_stream (stream);
	}
	//MYDVB_EIT *eit;				/* programa actual y siguiente */
	//MYDVB_EIT *eit_sched[16];	/* planificacion de la programacion, hasta 16 dias? */


}

void mydvb_show_stream (MYDVB_STREAM *stream) {


	mydvb_log (MYDVB_LOG_LEVEL_INFO,"---Stream Type: %2X %s", stream->type, mydvb_stream_type_table()[stream->type]);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"----pid: %d", stream->pid);

	mydvb_show_descriptors (stream->descriptors, 4);

}

void mydvb_show_sdt (MYDVB_SDT *sdt) {
	int i, len;
	mydvb_service_description *sd;

	if (sdt==NULL) {
		return;
	}

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "SDT: Transport Stream ID: %d", sdt->ts_id);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Version %d", sdt->version);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Original Network ID: %d", sdt->original_net_id);

	
	len = dyn_array_get_size (sdt->service_description);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"--Services (total %d)", len);
	for (i=0; i < len; i++) {
		sd = (mydvb_service_description *) dyn_array_get_data (sdt->service_description, i);
		
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"--Service Description Number: %d", sd->number);
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"---Running Status: %d %s", sd->running_status, mydvb_running_status_table()[sd->running_status]);
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"---Scrambled: %d", sd->scram);
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"---EIT: %d", sd->eit);
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"---EIT Present/Following: %d", sd->eit_pw);
		
		mydvb_show_descriptors (sd->descriptors, 3);
		
	} 
	
}

void mydvb_show_aits (DYN_ARRAY *aits) {
	int i, len;
	MYDVB_AIT *ait, **ait_p;
	
	len = dyn_array_get_size (aits);
	for (i=0; i < len; i++) {
		ait_p = (MYDVB_AIT **) dyn_array_get_data (aits, i);
		ait = *ait_p;
		mydvb_show_ait (ait);
	}
}

void mydvb_show_ait (MYDVB_AIT *ait) {
	int i, len;
	MYDVB_AIT_APP *app;

	mydvb_log (MYDVB_LOG_LEVEL_INFO,"AIT");
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-pid %d", ait->pid);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Version %d", ait->version);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Test Application Flag: %d", ait->test_app_flag);
	mydvb_log (MYDVB_LOG_LEVEL_INFO,"-Application Type: %d", ait->app_type);

	mydvb_show_descriptors (ait->descriptors, 1);

	len = dyn_array_get_size (ait->apps);
	for (i=0; i < len; i++) {
		app = (MYDVB_AIT_APP *) dyn_array_get_data (ait->apps, i);

		mydvb_log (MYDVB_LOG_LEVEL_INFO,"--APPL: Ctrl Code %d", app->ctrl_code);
		mydvb_show_descriptors (app->descriptors, 2);
	}	

}

char *guiones (int n) {
	static char *s1="-";
	static char *s2="--";
	static char *s3="---";
	static char *s4="----";
	static char *s5="-----";
	static char *s6="------";
	static char *s7="-------";
	static char *s8="--------";
	
	switch (n) {
		case 1:
			return s1;
		case 2:
			return s2;
		case 3:
			return s3;
		case 4:
			return s4;
		case 5:
			return s5;
		case 6:
			return s6;
		case 7:
			return s7;
		case 8:
			return s8;
			
	}
	
	return "";
		
}

void mydvb_show_descriptors (DYN_ARRAY *descriptors, int n) {
	int len, i;
	mydvb_descriptor *d;
	
	mydvb_log (MYDVB_LOG_LEVEL_INFO, "%sDescriptors", guiones(n));
	len = dyn_array_get_size (descriptors);
	for (i=0; i < len; i++) {
		d = (mydvb_descriptor *) dyn_array_get_data (descriptors, i);
		mydvb_show_descriptor (d, n+1);
	}

}

void mydvb_show_descriptor (mydvb_descriptor *d, int n) {

	char *s = mydvb_descriptor_type_table ()[d->info.type];

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "%sDescriptor type %02x (%s)", guiones (n), d->info.type, s);
	switch (d->info.type) {
		case 0x48:
			mydvb_show_0x48_descriptor (&d->service, n+1);
			break;
		case 0x52:
			mydvb_show_0x52_descriptor (&d->stream_id, n+1);
			break;			
		case 0x66:
			mydvb_show_0x66_descriptor (&d->data_broadcast_id, n+1);
			break;
	}
}

void mydvb_show_0x48_descriptor (mydvb_service_descriptor *d, int n) {
	char *utf;

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "%sservice type: %d %s",
		guiones(n),
		d->service_type, mydvb_service_type_table()[d->service_type]);
	if (d->provider_name!=NULL) {
		utf = mydvb_text_to_utf8 (d->provider_name);
		mydvb_log (MYDVB_LOG_LEVEL_INFO, "%sprovider name: %s",
			guiones(n),
			utf);
		free (utf);
	}
	if (d->service_name!=NULL) {
		utf = mydvb_text_to_utf8 (d->service_name);
		mydvb_log (MYDVB_LOG_LEVEL_INFO, "%sservice name: %s",
			guiones(n),
			utf);
		free (utf);
	}
}

void mydvb_show_0x52_descriptor (mydvb_stream_id_descriptor *d, int n) {

	mydvb_log (MYDVB_LOG_LEVEL_INFO,"%scomponent tag: %02x", guiones(n), d->component_tag);

}

void mydvb_show_0x66_descriptor (mydvb_data_broadcast_id_descriptor *d, int n) {

	mydvb_log (MYDVB_LOG_LEVEL_INFO,"%sdata broadcast id: %04x", guiones(n), d->data_broadcast_id);
	if (d->data_broadcast_id==0x00f0) {
		mydvb_mhp_data_broadcast_id_descriptor *dbd = (mydvb_mhp_data_broadcast_id_descriptor *) d;
		int len, i;
		
		len = dbd->apps_len;		
		mydvb_log (MYDVB_LOG_LEVEL_INFO,"%sinfo (total %d): ", guiones(n), len);
		for (i=0; i < len; i++) {
			mydvb_log (MYDVB_LOG_LEVEL_INFO, "%d ", dbd->apps[i]);
		}
	}

}
