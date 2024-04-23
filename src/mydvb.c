
/* example use 

	MYDVB_LIST	   *dvb_list;

	MYDVB_PROGRAM *program;
	MYDVB *mydvb;
	int channumber;
	INFO_DVB *info_dvb;
	char scan_file[256];

	// search for all dvb devices availables 
	dvb_list = mydvb_list_new ();

	// find the apropiate dvb device 
	mydvb = mydvb_list_get_dvbt (dvb_list);
	info_dvb = info_dvb_new ();
	sprintf (scan_file, "%s/.xine/dvbt_scan.txt", getenv("HOME"));
	info_dvb_load (scan_file, info_dvb);
	
	mydvb_init (mydvb, NULL);

		mydvb_tune_channel (&mydvb->mytune, info_dvb, channumber);

			// define interested events and callbacks
			listener.type = type;
			listener.ready.callback = ready_callback;
			listener.ready.data = extra data;

			mydvb_add_listener (mydvb, &listener);

			mydvb_start_decoder (mydvb);

			mydvb_main_loop (mydvb);
	
			mydvb_end_decoder (mydvb);

		mydvb_tune_end (&mydvb->mytune);

	mydvb_end (mydvb);

	info_dvb_free (info_dvb);

	mydvb_list_free (mydvb_list);

*/

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/dvb/dmx.h>

/* -------- public includes ------------- */

#include <mydvb.h>

#include <dynarray.h>

#include <mydvb_mhp_tables.h>

/* -------- local includes -------------- */

#include "tables.h"

#include "mhp_tables.h"

#include "events.h"

#include "logger.h"

#include "util.h"

#define MYDVB_MAX_ADAPTERS			4
#define MYDVB_MAX_DEVICES			4


/**
 * add a file descriptor to the poll system
 */
void __mydvb_add_poll (MYDVB *mydvb, int fd) {
	mydvb->pfd[mydvb->poll_len].fd = fd;
	mydvb->pfd[mydvb->poll_len].events = POLLPRI | POLLIN | POLLERR;
	mydvb->pfd[mydvb->poll_len].revents = 0;
	mydvb->poll_len++;
}

void __mydvb_list_add (MYDVB_LIST *l, MYDVB *mydvb) {
	int n = 0;

	if (l==NULL) {
		return;
	}

	if (mydvb == NULL) {
		return;
	}

	n = l->len;

	if(n == 0) {
	    l->list = (MYDVB **) malloc(sizeof(MYDVB *));
	} else {
	    l->list = (MYDVB **) realloc(l->list, sizeof(MYDVB *) * (n + 1));
	}

	l->list[n] = mydvb;

	n ++;
	l->len = n;

}

/* this builds a list of mydvb objects, it searchs in devices and adapters to do that*/
MYDVB_LIST *mydvb_list_new () {
	int adapter;
	int device;
	MYDVB *mydvb;
	MYDVB_LIST *l = NULL;


	l = (MYDVB_LIST *) malloc (sizeof (MYDVB_LIST));
	
	if (l==NULL) {
		return NULL;
	}

	l->len = 0;
	l->list = NULL;

	for (adapter=0; adapter < MYDVB_MAX_ADAPTERS; adapter ++) {
		for (device=0; device < MYDVB_MAX_DEVICES; device ++) {
			// ok, creamos el objeto mydvb
			mydvb = (MYDVB *) malloc (sizeof(MYDVB));
			if (mydvb_tune_init (&mydvb->mytune, adapter, device) == 0){ // ok
				// add to mydvb list
				mydvb_log (MYDVB_LOG_LEVEL_INFO, "Found device %s at adapter %d, device %d", mydvb->mytune.name, adapter, device);

				__mydvb_list_add (l,mydvb);
			} else {
				free (mydvb);
				mydvb = NULL;
			}
		}
	}	

	return l;
}

void mydvb_list_free (MYDVB_LIST *l) {
	int i;
	MYDVB *mydvb;

	if (l==NULL) {
		return;
	}

	if (l->list==NULL) {
		return;
	}

	for (i=0; i < l->len; i++) {
		mydvb = l->list[i];
		mydvb_tune_end (&mydvb->mytune);
		free (mydvb);
	}
	free (l->list);
	l->list = NULL;
	l->len  = 0;

	free (l);
	
}

/* localiza un mydvb adecuado para television digital terrestre */
MYDVB *mydvb_list_get_dvbt (MYDVB_LIST *dvb_list) {
	int i;
	MYDVB *mydvb = NULL;

	if (dvb_list==NULL) {
		return NULL;
	}

	for (i=0; i < dvb_list->len; i++) {
		mydvb = dvb_list->list[i];
		if (mydvb_tune_is_dvbt (&mydvb->mytune)) {
			break;
		} 
	}

	return mydvb;
}

/* localiza un mydvb adecuado para transmisiones por satelite */
MYDVB *mydvb_list_get_dvbs (MYDVB_LIST *dvb_list) {
	int i;
	MYDVB *mydvb = NULL;

	if (dvb_list==NULL) {
		return NULL;
	}

	for (i=0; i < dvb_list->len; i++) {
		mydvb = dvb_list->list[i];
		if (mydvb_tune_is_dvbs (&mydvb->mytune)) {
			break;
		} 
	}

	return mydvb;
}

/* localiza un mydvb adecuado para television digital por cable */
MYDVB *mydvb_list_get_dvbc (MYDVB_LIST *dvb_list) {
	int i;
	MYDVB *mydvb = NULL;

	if (dvb_list==NULL) {
		return NULL;
	}

	for (i=0; i < dvb_list->len; i++) {
		mydvb = dvb_list->list[i];
		if (mydvb_tune_is_dvbc (&mydvb->mytune)) {
			break;
		} 
	}

	return mydvb;
}

/**
 * DVB initialization
*/
int mydvb_init (MYDVB *mydvb) {
	int i;


	mydvb->poll_len = 0;
	for (i = 0; i < 32; i++) {
		mydvb->pfd[i].fd = -1;
	}

	mydvb->listeners = dyn_array_new (sizeof(MYDVB_LISTENER));

	mydvb->mainloop = 0;

	/* initialization of decoding attributes */

	mydvb->fd_dvr 	= -1;

	mydvb->sct_fd_len = 0;
	for (i=0; i < 32; i++) {
		mydvb->sct_fd[i].fd = -1;
		mydvb->sct_fd[i].pid = 0;
	}

	mydvb->pes_fd_len = 0;
	for (i=0; i < 32; i++) {
		mydvb->pes_fd[i].fd = -1;
		mydvb->pes_fd[i].pid = 0;
	}

	mydvb->pat = NULL;
	mydvb->cat = NULL;
	mydvb->sdt = NULL;

	mydvb->aits = NULL;

	mydvb->ready = 0;

	/* add the tune filedescriptor to the poll system */
	__mydvb_add_poll (mydvb, mydvb->mytune.fd);

	return 0;
}

/**
 * dvb closing and free resources
*/
void mydvb_end (MYDVB *mydvb) {

	/* remove and free listeners */
	dyn_array_free (mydvb->listeners);

	mydvb->listeners = NULL;

}

/**
	funcion devuelve el fichero demux asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_demux_file (MYDVB *mydvb) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/demux%d", mydvb->mytune.adapter, mydvb->mytune.device);

	return s;
}

/**
	funcion devuelve el fichero video asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_video_file (MYDVB *mydvb) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/video%d", mydvb->mytune.adapter, mydvb->mytune.device);

	return s;
}

/**
	funcion devuelve el fichero dvr asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_dvr_file (MYDVB *mydvb) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/dvr%d", mydvb->mytune.adapter, mydvb->mytune.device);

	return s;
}

/**
	funcion devuelve el fichero frontend asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_frontend_file (MYDVB *mydvb) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/frontend%d", mydvb->mytune.adapter, mydvb->mytune.device);

	return s;
}

/**
 * add a pes filter and streams are send to the vdr device
 */
int __mydvb_add_stream_filter (int fd, int pid, int type, int input_type) {

	struct dmx_pes_filter_params dmx_p;

	memset(&dmx_p, 0, sizeof(dmx_p));

	// send stream pid to dvr device
	dmx_p.pid 			= pid;
	dmx_p.input 		= input_type;
	dmx_p.output 		= DMX_OUT_TS_TAP;
	dmx_p.pes_type 		= type;
	dmx_p.flags 		= DMX_IMMEDIATE_START;

	if (ioctl (fd, DMX_SET_PES_FILTER, &dmx_p)==-1) {
		return -1;
	}

	return 0;
}

/**
 * add multiple table id table filter
 */
int __mydvb_add_table_filters (int fd, int pid, int t_id, unsigned char mask) {
	struct dmx_sct_filter_params dmx_s;

	// add pat table
	memset(&dmx_s, 0, sizeof(dmx_s));
	dmx_s.pid = pid;
	dmx_s.filter.filter[0] = t_id;
	dmx_s.filter.mask[0]   = mask;
	dmx_s.timeout = 0;
	dmx_s.flags = DMX_IMMEDIATE_START;

	if (ioctl(fd, DMX_SET_FILTER, &dmx_s)==-1) {

		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error in add_table_filters, setting dmx_set_filter");
		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Dmx values: %d, %d, %d, %d\n", fd, pid, t_id, mask);

		return -1;
	}

	return 0;
}

/**
 * Add a single table id table filter
 */
int __mydvb_add_table_filter (int fd, int pid, int t_id) {
	int r;

	r = __mydvb_add_table_filters (fd, pid, t_id, 0xff);

	return r;

}



/**
 * remove a file descriptor from the poll system
 */
void __mydvb_remove_poll (MYDVB *mydvb, int fd) {
	int i, j;
	
	if (mydvb == NULL) {
		return;
	}

	for (i=0; i < mydvb->poll_len; i++) {

		if (mydvb->pfd[i].fd == fd) { /* ok, encontrado */

			mydvb->poll_len --;

			for (j=i; j < mydvb->poll_len; j++) {

				mydvb->pfd[j].fd = mydvb->pfd[j+1].fd;
			}

			return;
		}
	}

}

int __mydvb_add_pes (MYDVB *mydvb, int pid, int type) {

	int fd;
	char *s;
	int et;

	if (mydvb == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (mydvb);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		perror ("Error opening demux data stream");
		return -1;
	}

	et = __mydvb_add_stream_filter (fd, pid, type, DMX_IN_FRONTEND);
	if (et==-1) {
		perror ("Error adding stream filter");
		close (fd);
		return -1;
	}

	/* add it to the pes demux filedescriptors table */
	mydvb->pes_fd[mydvb->pes_fd_len].pid = pid;
	mydvb->pes_fd[mydvb->pes_fd_len].fd = fd;
	mydvb->pes_fd_len ++;

	return 0;

}

int __mydvb_add_sct (MYDVB *mydvb, int pid, int tid) {

	int fd;
	char *s;
	int et;

	if (mydvb == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (mydvb);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		perror ("Error opening demux data table");
		return -1;
	}

	et = __mydvb_add_table_filter (fd, pid, tid);

	if (et==-1) {
		perror ("Error adding table filter");
		close (fd);
		return -1;
	}

	__mydvb_add_poll (mydvb, fd);

	/* add it to the sct demux filedescriptors table */
	mydvb->sct_fd[mydvb->sct_fd_len].pid = pid;
	mydvb->sct_fd[mydvb->sct_fd_len].fd = fd;
	mydvb->sct_fd_len ++;

	return 0;

}

int __mydvb_add_scts (MYDVB *mydvb, int pid, int tid, unsigned char mask) {

	int fd;
	char *s;
	int et;

	if (mydvb == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (mydvb);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		perror ("Error opening demux data table");
		return -1;
	}

	//
	et = __mydvb_add_table_filters (fd, pid, tid, mask);

	if (et==-1) {
		perror ("Error adding table filter");
		close (fd);
		return -1;
	}

	__mydvb_add_poll (mydvb, fd);

	/* add it to the sct demux filedescriptors table */
	mydvb->sct_fd[mydvb->sct_fd_len].pid = pid;
	mydvb->sct_fd[mydvb->sct_fd_len].fd = fd;
	mydvb->sct_fd_len ++;

	return 0;

}



/**
 * Decoding startup: main tables startup: PAT, SDT, CAT, EIT
 */
void mydvb_start_decoder (MYDVB *mydvb) {

	char *s;
	int i;

	if (mydvb == NULL) {
		return;
	}

	/* initialization of decoding attributes */
	mydvb->fd_dvr 	= -1;

	mydvb->sct_fd_len = 0;
	for (i=0; i < 32; i++) {
		mydvb->sct_fd[i].fd = -1;
		mydvb->sct_fd[i].pid = 0;
	}

	mydvb->pes_fd_len = 0;
	for (i=0; i < 32; i++) {
		mydvb->pes_fd[i].fd = -1;
		mydvb->pes_fd[i].pid = 0;
	}

	mydvb->pat = NULL;
	mydvb->cat = NULL;
	mydvb->sdt = NULL;

	mydvb->aits = dyn_array_new (sizeof(MYDVB_AIT *));

	mydvb->ready = 0;



	/* now, add required tables */

	/* add the PAT table */
	__mydvb_add_sct (mydvb, 0, 0);

	__mydvb_add_pes (mydvb, 0, DMX_PES_OTHER);

	/* add the CAT table */
	//__mydvb_add_sct (mydvb, 0x01, 0x01);

	/* add the SDT table */
	__mydvb_add_sct (mydvb, 0x11, 0x42);

	// start with the eit table
	__mydvb_add_scts (mydvb, 0x12, 0x4e, 0xe0);

	/* start with the tdt table */
	//__mydvb_add_sct (mydvb, 0x14, 0x70);

	/* vdr receives data from this program/channel */
	s = __mydvb_get_dvr_file (mydvb);
	mydvb->fd_dvr = open (s, O_RDONLY | O_NONBLOCK);

	__mydvb_add_poll (mydvb, mydvb->fd_dvr);

}


/**
 * Stop decoding
 */
void mydvb_end_decoder (MYDVB *mydvb) {
	int i;

	if (mydvb==NULL) {
		return;
	}

	mydvb->ready = 0;

	/* close the vdr */
	if (mydvb->fd_dvr != -1) {

		__mydvb_remove_poll (mydvb, mydvb->fd_dvr);

		close (mydvb->fd_dvr);
		mydvb->fd_dvr = -1;
	}

	/* stop and close all sct demux descriptors */
	for (i=0; i < mydvb->sct_fd_len; i++) {
		if (mydvb->sct_fd[i].fd != -1) {

			__mydvb_remove_poll (mydvb, mydvb->sct_fd[i].fd);

			ioctl (mydvb->sct_fd[i].fd, DMX_STOP);
			close (mydvb->sct_fd[i].fd);
			mydvb->sct_fd[i].fd = -1;
			mydvb->sct_fd[i].pid = 0;
		}
	}
	mydvb->sct_fd_len = 0;

	/* stop and close all pes demux descriptors */
	for (i=0; i < mydvb->pes_fd_len; i++) {
		if (mydvb->pes_fd[i].fd != -1) {

			__mydvb_remove_poll (mydvb, mydvb->pes_fd[i].fd);

			ioctl (mydvb->pes_fd[i].fd, DMX_STOP);
			close (mydvb->pes_fd[i].fd);
			mydvb->pes_fd[i].fd = -1;
			mydvb->pes_fd[i].pid = 0;
		}
	}
	mydvb->pes_fd_len = 0;

	mydvb_delete_pat (mydvb->pat);
	mydvb->pat = NULL;

	mydvb_delete_sdt (mydvb->sdt);
	mydvb->sdt = NULL;

	mydvb_delete_cat (mydvb->cat);
	mydvb->cat = NULL;

	dyn_array_free (mydvb->aits);
	mydvb->aits = NULL;

}



int mydvb_start_program (MYDVB *mydvb, MYDVB_PROGRAM *program) {

	int r = 0;

	if (mydvb == NULL || program == NULL) {
		return -1;
	}

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Starting program %d %d", program->number, program->pmt_pid);

	// add the pmt table descriptor to decoding
	r = __mydvb_add_sct (mydvb, program->pmt_pid, 0x02);

	// add the pmt pid to pes
	r = __mydvb_add_pes (mydvb, program->pmt_pid, DMX_PES_OTHER);

	return 0;
}

void mydvb_stop_program (MYDVB *mydvb, MYDVB_PROGRAM *program) {

	return;
}

int mydvb_start_stream (MYDVB *mydvb, MYDVB_STREAM *stream) {

	int r;

	if (mydvb == NULL || stream == NULL) {
		return -1;
	}

	r = __mydvb_add_pes (mydvb, stream->pid, DMX_PES_OTHER);

	return r;

}

void mydvb_stop_stream (MYDVB_STREAM *stream) {

	if (stream == NULL) {
		return;
	}

	stream->pid;

}

int mydvb_read_table (MYDVB *mydvb, int fd, unsigned char *buffer) {

	int i;
	int n;
	unsigned char *tmpbuffer;
	char *s;
	int section_len;
	size_t total_len;
	int number;
	int table_id;
	MYDVB_PROGRAM *program = NULL;
	MYDVB_PAT *old_pat;
	int j;
	int len, len2;
	MYDVB_STREAM *stream = NULL;
	MYDVB_EVENT *event = NULL;
	MYDVB_AIT *ait = NULL, **ait_p = NULL;

	n = 0;

	// ok data available
	tmpbuffer = buffer;
	n = read (fd, tmpbuffer, 3);

	section_len = getbits (tmpbuffer,12,12);
	total_len = section_len + 3;

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Table found, id %X, %d-%d", tmpbuffer[0], n, total_len);

	n += read (fd, tmpbuffer + 3, section_len);

	table_id = tmpbuffer[0];

	/* ahora comprobamos y hacemos segun el tipo de tabla */
	switch (table_id) {
		case 0x00: // PAT

			old_pat = mydvb->pat;

			mydvb->pat = mydvb_new_pat_if_changed (tmpbuffer, total_len, mydvb->pat);

			if (mydvb->pat!=NULL && mydvb->pat != old_pat) {

				mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "PAT found with version: %d", mydvb->pat->version);

				len = dyn_array_get_size (mydvb->pat->programs);
				for (j=0; j < len; j++) {

					program = dyn_array_get_data (mydvb->pat->programs, j);

					mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Program %d, number: %d, pid %d", j, program->number, program->pmt_pid);

				}
				if (mydvb->pat->nit) {

					mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "NIT found with pid %d", mydvb->pat->nit->pid);

					__mydvb_add_sct (mydvb, mydvb->pat->nit->pid, 0x40);

				}

				if (!mydvb->ready) {
					mydvb_ready (mydvb);
					if (mydvb->ready) {
						event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

						event->type = MYDVB_EVENT_READY;

						mydvb_notify_event (mydvb, event);

						free (event);
					}
				}
			}
			break;
		case 0x01: /* CAT */

			mydvb->cat = mydvb_new_cat_if_changed (tmpbuffer, total_len, mydvb->cat);

			break;

		case 0x02: // Program map section
			if (mydvb->pat) {
				number = getbits (tmpbuffer, 24, 16);

				// looks for number in current pat -> programs

				len = dyn_array_get_size (mydvb->pat->programs);
				for (i=0; i < len; i++) {
					program = dyn_array_get_data (mydvb->pat->programs, i);
					if (program!=NULL) {
						if (program->number == number) { // found, change it

							j = mydvb_edit_program (program, tmpbuffer, total_len);
							if (j) {
								mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Program number %d is ready", number);

								len2 = dyn_array_get_size (program->streams);
								for (j=0; j < len2; j++) {

									stream = dyn_array_get_data (program->streams, j);

									/*
									mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Program number %d edited: stream %d, type %d, pid %d\n", i, j,
									 	stream->type,
									 	stream->pid);
									 */

									if (stream->type==0x05) { /* a MHP application stream */
										//__add_ait_decoder (mydvb, stream);
									}

								}

								// event callback
								event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

								event->type = MYDVB_EVENT_PROGRAM;

								event->program.program = program;

								mydvb_notify_event (mydvb, event);

								free (event);


							}
							break;
						}
					}
				}
			}
			break;
		case 0x40: /* NIT current network */
			mydvb_edit_nit (mydvb->pat->nit, tmpbuffer, total_len);

			break;
		case 0x41: /* NIT other network */
			break;
		case 0x42:
		case 0x46: /* SDT */
			mydvb->sdt = mydvb_new_sdt_if_changed (tmpbuffer, total_len, mydvb->sdt);

			/* notify listeners */
			if (!mydvb->ready) {
				mydvb_ready (mydvb);
				if (mydvb->ready)  {
					event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

					event->type = MYDVB_EVENT_READY;

					mydvb_notify_event (mydvb, event);

					free (event);
				}
			}

			break;
		case 0x4e ... 0x6f: /* EIT */
			mydvb_new_eit_if_changed (mydvb->pat, tmpbuffer, total_len);
			break;
		case 0x70: /* TDT */
			mydvb_tdt (tmpbuffer, total_len, &mydvb->tdt);
			break;
		case 0x74: /* AIT - Application Information Table: Multimedia Home Platform*/

			//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "AIT table found");

			mydvb_hex_log (1, tmpbuffer, total_len);
			len = dyn_array_get_size (mydvb->aits);
			for (i=0; i < len; i++) {
				ait_p = (MYDVB_AIT **) dyn_array_get_data (mydvb->aits, i);
				ait = *ait_p;
				if (ait->fd==fd) {
					ait = mydvb_new_ait_if_changed (tmpbuffer, total_len, ait);
					*ait_p = ait;
				}
			}

			break;
		case 0x80:
		case 0x81:
			/* ECM (CA_Message_section) */
			break;
		default: // no more tables for now
			break;
	}

	return n;
}

/**
 *
 * the dvb main loop
 */
void mydvb_main_loop (MYDVB *mydvb) {

	int r;
	int j=0;
	int i;

	off_t n=0;
	static unsigned char buffer[2*4096]; /* twice the maximum size of a table section */

	MYDVB_EVENT event;

	int enc = 0;

	struct dvb_frontend_event f_event;

	long long t_out = current_timestamp_ms ();

	mydvb->mainloop = 1;

	while (mydvb->mainloop) {
		
		r = poll (mydvb->pfd, mydvb->poll_len, 1000);

		if (r > 0) {

			for (j=0; j < mydvb->poll_len; j++) {

				if (mydvb->pfd[j].revents & POLLPRI || mydvb->pfd[j].revents & POLLIN) {

					/* check for dvr event */
					if (mydvb->pfd[j].fd == mydvb->fd_dvr) {

						n = read (mydvb->pfd[j].fd, buffer, sizeof(buffer));
						if (n!=-1) {

							/* notify listeners */
							event.type = MYDVB_EVENT_STREAM;

							event.stream.buffer = buffer;
							event.stream.length = n;

							mydvb_notify_event (mydvb, &event);

						}

					/* check for frontend event */
					} else if (mydvb->pfd[j].fd == mydvb->mytune.fd) {

						memset (&f_event, 0, sizeof (struct dvb_frontend_event));

						if (ioctl(mydvb->mytune.fd, FE_GET_EVENT, &f_event) == 0) {

							mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Frontend Status %X", f_event.status);

							if (f_event.status & FE_TIMEDOUT) {

								if (mydvb->mytune.status == TUNE_STATUS_TRYING) {

									mydvb->mytune.status = TUNE_STATUS_FAILED;

									// fire event
									event.type = MYDVB_TUNE_EVENT;

									event.tune.status = mydvb->mytune.status;

									mydvb_notify_event (mydvb, &event);
								}

							} else //if (f_event.status & FE_HAS_LOCK) {
								if ((f_event.status & 0x1F) == 0x1F) { // locked and ok

									if (mydvb->mytune.status == TUNE_STATUS_TRYING) {

										mydvb->mytune.status = TUNE_STATUS_LOCK;

										// fire event
										event.type = MYDVB_TUNE_EVENT;

										event.tune.status = mydvb->mytune.status;

										mydvb_notify_event (mydvb, &event);
									}

							}

						} else {

							mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error getting status from frontend: %s", strerror(errno));

						}

					} else {

						enc = 0;

						/* check for tables */
						for (i = 0; i < mydvb->sct_fd_len; i++) {

							if (mydvb->pfd[j].fd == mydvb->sct_fd[i].fd) {

								/* decode table */

								mydvb_read_table (mydvb, mydvb->pfd[j].fd, buffer);

								enc = 1;

								break;
							}
						}

						if (!enc) {
							/* this is, probably, an external event */
							event.type = MYDVB_EVENT_EXT_FD;

							event.external.fd = mydvb->pfd[j].fd;

							mydvb_notify_event (mydvb, &event);
						}

					}
					
				}
			}

		} else if (r == 0) {
			//time out

			//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Time out in main loop");

			if (mydvb->mytune.status == TUNE_STATUS_TRYING) {

				/* check time out of device */

				long long ms = current_timestamp_ms ();

				if (ms - mydvb->mytune.start_ms >= 3000) { /* the tuner has 3 seconds for locking */

					mydvb->mytune.status = TUNE_STATUS_FAILED;

					// fire event
					event.type = MYDVB_TUNE_EVENT;

					event.tune.status = mydvb->mytune.status;

					mydvb_notify_event (mydvb, &event);
				}

			}

		} else {
			// error
			mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error in main_loop, getting data");
		}

		long long t_now = current_timestamp_ms ();
		if (t_now - t_out >= 1000) {
			t_out = t_now;

			event.type = MYDVB_TIMEOUT_EVENT;

			event.timeout.count = t_now;

			mydvb_notify_event (mydvb, &event);
		}

	}

}

/**
 * set the main loop flat to 0, indicating that the main loop must terminate
 */
void mydvb_end_main_loop (MYDVB *mydvb) {

	mydvb->mainloop = 0;
}


int mydvb_ready (MYDVB *mydvb) {

	if (mydvb==NULL) {
		return 0;
	}

	if (mydvb->ready) {
		return mydvb->ready;
	}

	if (mydvb->pat !=NULL) {
		if (mydvb->sdt!=NULL) {
			mydvb->ready = 1;
		}
	}

	return mydvb->ready;
}

int mydvb_eit_ready (MYDVB *mydvb, int prog_num) {
	int i;
	MYDVB_PROGRAM *program = NULL;
	MYDVB_EIT *eit;

	if (mydvb==NULL) {
		return 0;
	}

	if (mydvb->pat !=NULL) {
		if (mydvb->sdt!=NULL) {

			program = dyn_array_get_data (mydvb->pat->programs, prog_num);
			if (program==NULL) {
				return 0;
			}

			for (i=0; i < 16; i++) {
				eit = program->eit_sched[i];
				if (eit==NULL) {
					continue;
				}
				if (eit->incomplete==0) {
					/* almenos una de las 16 tablas esta completa */
					return 1;
				}
			}

		}
	}

	return 0;
}

void __add_ait_decoder (MYDVB *mydvb, MYDVB_STREAM *stream) {
	int len, i, encontrado;
	MYDVB_AIT *ait, **ait_p;
	char *s;
	
	encontrado = 0;
	
	len = dyn_array_get_size (mydvb->aits);
	for (i=0; i < len; i++) {
		ait_p = (MYDVB_AIT **) dyn_array_get_data (mydvb->aits, i);
		ait = *ait_p;
		if (ait->pid == stream->pid) {
			encontrado = 1;
			break;
		}
	}
	
	/*
	if (!encontrado) {
											
		s = __mydvb_get_demux_file (mydvb);
		stream->fd = open (s, O_RDWR);// | O_NONBLOCK);
		if (stream->fd != -1) {
			__mydvb_add_poll (mydvb, stream->fd);
			__mydvb_add_table_filter (stream->fd, stream->pid, 0x74);		
													
			ait = mydvb_new_ait ();
			ait->pid = stream->pid;
			ait->fd  = stream->fd;
			ait_p = (MYDVB_AIT **) dyn_array_add_empty (mydvb->aits);
			*ait_p = ait;																					
		}
	}
	*/
}






/* ------------- end tune scan --------------- */

int mydvb_tune_channel (MYDVB *mydvb, INFO_CHANNEL *ichannel) {

	int r;

	MYDVB_EVENT event;

	MYDVB_TUNE *mytune;


	mytune = &mydvb->mytune;

	memcpy (&mytune->p, &ichannel->p, sizeof(struct dvb_frontend_parameters));

	/* notify listeners about trying tuning */
	event.type = MYDVB_TUNE_EVENT;
	event.tune.status = TUNE_STATUS_TRYING;

	mydvb_notify_event (mydvb, &event);


	/* tune */

	r = mydvb_tune_frontend (mytune);

}

/**
 * Tune a channel of services (programs) based on the information of channels
 */
int mydvb_tune_channel_by_number (MYDVB *mydvb, INFO_DVB *info_dvb, int channel_number) {

	int i;
	int r = -1;
	INFO_CHANNEL *ichannel = NULL;

	if (mydvb == NULL || info_dvb == NULL) {
		return -1;
	}

	for (i=0; i < info_dvb->channels_len; i++) {

		ichannel = info_dvb->channels[i];

		if (ichannel->n==channel_number) {
			// ok, found

			r = mydvb_tune_channel (mydvb, ichannel);

			break;
		}
	}

	return r;
}
