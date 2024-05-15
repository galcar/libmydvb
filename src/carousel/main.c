/* compilar con:

gcc -I/home/guiller/minipc/src/kernel/linux-2.6.10/include `xml2-config --cflags` -c -o main.o main.c
gcc -o main main.o `xml2-config --libs` libmydvb.a

*/
#include <stdio.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/dvb/frontend.h>
#include <linux/dvb/video.h>

#include "mydvb.h"
#include "dynarray.h"

#include "libdsmcc.h"

void data_reader (MYDVB *mydvb, unsigned char *buffer, int length, void *data) {
	
	struct dsmcc_status *st = (struct dsmcc_status *) data;

	dsmcc_receive(st, buffer, length);
		
}

void add_st (struct dsmcc_status *st, int pid) {
	struct pid_buffer *aux, *buf;

	aux = (struct pid_buffer *)malloc(sizeof(struct pid_buffer));
	aux->pid = pid;
	aux->pointer_field = 0;
	aux->in_section = 0;
	aux->cont = -1;
	aux->next = NULL;
		
	if (st->buffers!=NULL) {
		buf = st->buffers;
		while (buf->next != NULL) {
			buf = buf->next;
		}
		buf->next = aux;	
	} else {
		st->buffers = aux;
	}
	
}

int main (int argc, char *args[]) {
	MYDVB_LIST *l;
	MYDVB *mydvb;
	INFO_DVB *info_dvb;
	MYDVB_PROGRAM *program;
	char scan_file[1024];
	int channel = 0;
	int kb;
	xmlDocPtr doc;
	
	int i;
	MYDVB_STREAM *stream;
	
	MYDVB_LISTENER *listener = NULL;
	struct dsmcc_status *st;
	
	
	channel = atoi(args[1]);

	l = mydvb_list_new ();

	mydvb = mydvb_list_get_dvbt (l);
	if (mydvb!=NULL) {
		info_dvb = info_dvb_new ();
		sprintf (scan_file, "%s/.xine/dvbt_scan.txt", getenv("HOME"));
		info_dvb_load (scan_file, info_dvb);	
	
		st = dsmcc_open("test", stdout);	
	
		/* empezamos */
		mydvb_init (mydvb, NULL);			
		
		/* sintoniza el canal deseado */
		mydvb_tuner_channel (&mydvb->mytune, info_dvb, channel);

		/* arranca el decoder */
		mydvb_start_decoder (mydvb);
		
		while (!mydvb_ready(mydvb));
		
		program = mydvb_get_program_by_number (mydvb, 1);
		
		for (i=0; i < dyn_array_get_size (program->streams); i++) {
			stream = (MYDVB_STREAM *) dyn_array_get_data (program->streams, i);
			printf ("STREAM, pid: %d, type: %2X\n", stream->pid, stream->type);
			if (stream->type==0x0B) {
				add_st (st, stream->pid);
				listener = (MYDVB_LISTENER *) malloc (sizeof(MYDVB_LISTENER));
				listener->stream = stream;
				listener->data_reader = data_reader;
				listener->data = st;
				mydvb_add_stream_listener (mydvb, listener);
			}
		}		
		
		
		mydvb_start_program_stream (mydvb, program);
		
		kb = getc(stdin);
		while (kb!=122) {	 /* 'z' */
			printf ("TECLA: %d\n", kb);
			if (kb==120) {	/* 'x' */
				doc = mydvb_epg_sched_xml (mydvb);
				xmlSaveFormatFileEnc("epg.xml", doc, "UTF-8", 1);
			} else if (kb==119) { /* 'w' */
						
			}
			kb = getc(stdin);
		}
		
		mydvb_end_program_stream (mydvb, program);
		
		/* termina el decoder */
		mydvb_stop_decoder (mydvb);

		/* finaliza la sintonización */
		mydvb_tune_end (&mydvb->mytune);		

		/* fin */
		mydvb_end (mydvb);
		
		dsmcc_close(st);
		dsmcc_free(st);	
		
		/* limpieza */
		info_dvb_free (info_dvb);
	}
	
	mydvb_list_free (l);

	return 0;

}
