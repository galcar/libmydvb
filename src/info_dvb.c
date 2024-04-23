
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/dvb/frontend.h>

#include "tune.h"
#include "util.h"

#include "info_dvb.h"

INFO_STREAM *info_stream_new () {
	INFO_STREAM *stream = NULL;

	stream = (INFO_STREAM *) malloc (sizeof(INFO_STREAM));
	stream->program = NULL;
	stream->n = 0;
	stream->type = 0;
	stream->pid = 0;

	return stream;
}

void info_stream_free (INFO_STREAM *stream) {
	free (stream);
}

INFO_PROGRAM *info_program_new () {
	INFO_PROGRAM *program = NULL;

	program = (INFO_PROGRAM *) malloc (sizeof(INFO_PROGRAM));
	program->channel = NULL;
	program->n = 0;
	program->provider = NULL;
	program->service  = NULL;
	program->number = 0;
	program->pid = 0;
	program->type = 0;
	program->scrambled = 0;
	program->user_number = -1;
	program->streams_len = 0;
	program->streams = NULL;

	return program;
}

void info_program_free (INFO_PROGRAM *program) {
	int i;
	for (i=0; i < program->streams_len; i++) {
		info_stream_free (program->streams[i]);
	}
	free (program->streams);

	if (program->provider != NULL) {
		mydvb_text_free (program->provider);
		program->provider = NULL;
	}

	if (program->service != NULL) {
		mydvb_text_free (program->service);
		program->service = NULL;
	}

	free (program);
}

int info_program_add (INFO_PROGRAM *program, INFO_STREAM *stream) {
	if (program==NULL) {
		return -1;
	}
	if (stream == NULL) {
		return -1;
	}

	if(program->streams_len == 0) {
	    program->streams = (INFO_STREAM **) malloc(sizeof(INFO_STREAM *));
	} else {
	    program->streams = (INFO_STREAM **) realloc(program->streams, sizeof(INFO_STREAM *) * (program->streams_len + 1));
	}

	program->streams[program->streams_len] = stream;

	stream->program = program;

	program->streams_len ++;

	return 0;
}


INFO_CHANNEL *info_channel_new () {
	INFO_CHANNEL *canal = NULL;

	canal = (INFO_CHANNEL *) malloc (sizeof(INFO_CHANNEL));
	canal->n = 0;
	canal->programs_len = 0;
	canal->programs = NULL;

	return canal;
}

void info_channel_free (INFO_CHANNEL *canal) {
	int i;
	for (i=0; i < canal->programs_len; i++) {
		info_program_free (canal->programs[i]);
	}
	free (canal->programs);
	free (canal);
}

int info_channel_add (INFO_CHANNEL *canal, INFO_PROGRAM *program) {
	if (canal==NULL) {
		return -1;
	}
	if (program == NULL) {
		return -1;
	}

	if(canal->programs_len == 0) {
	    canal->programs = (INFO_PROGRAM **) malloc(sizeof(INFO_PROGRAM *));
	} else {
	    canal->programs = (INFO_PROGRAM **) realloc(canal->programs, sizeof(INFO_PROGRAM *) * (canal->programs_len + 1));
	}

	canal->programs[canal->programs_len] = program;

	program->channel = canal;

	canal->programs_len ++;

	return 0;
}

INFO_DVB *info_dvb_new () {
	INFO_DVB *info = NULL;

	info = (INFO_DVB *) malloc (sizeof(INFO_DVB));
	info->channels_len = 0;
	info->channels = NULL;

	info->user_number = 1;

	return info;
}

void info_dvb_free (INFO_DVB *info) {
	int i;

	if (info==NULL) {
		return;
	}

	for (i=0; i < info->channels_len; i++) {
		info_channel_free (info->channels[i]);
	}
	free (info->channels);
	free (info);
}

int info_dvb_add (INFO_DVB *dvb, INFO_CHANNEL *channel) {
	if (dvb==NULL) {
		return -1;
	}
	if (channel == NULL) {
		return -1;
	}

	if(dvb->channels_len == 0) {
	    dvb->channels = (INFO_CHANNEL **) malloc(sizeof(INFO_CHANNEL *));
	} else {
	    dvb->channels = (INFO_CHANNEL **) realloc(dvb->channels, sizeof(INFO_CHANNEL *) * (dvb->channels_len + 1));
	}

	dvb->channels[dvb->channels_len] = channel;
	dvb->channels_len ++;


	return 0;
}

void info_dvb_remove (INFO_DVB *dvb, INFO_CHANNEL *channel) {

	INFO_CHANNEL *aux = NULL;

	int i, j;

	if (dvb==NULL) {
		return;
	}

	if (channel == NULL) {
		return;
	}

	for (i = 0; i < dvb->channels_len; i++) {

		aux = dvb->channels[i];

		if (aux == channel) {
			// found

			dvb->channels_len --;

			if (dvb->channels_len == 0) {

				free (dvb->channels);
				dvb->channels = NULL;

			} else {

				for (j = i; j < dvb->channels_len; j++) {
					dvb->channels[j] = dvb->channels[j+1];
				}

				/* memory reallocation */
				dvb->channels = (INFO_CHANNEL **) realloc(dvb->channels, sizeof(INFO_CHANNEL *) * dvb->channels_len);

		    }

			break;
		}
	}
}

INFO_PROGRAM *info_dvb_find (INFO_DVB *dvb, int channel, int service) {
	int i,j;

	INFO_CHANNEL *ichannel;
	INFO_PROGRAM *iprogram;

	if (dvb == NULL) {
		return NULL;
	}

	for (i=0; i < dvb->channels_len; i++) {
		ichannel = dvb->channels[i];

		if (ichannel == NULL) {
			continue;
		}

		if (ichannel->n == channel) {

			for (j=0; j < ichannel->programs_len; j++) {
				iprogram = ichannel->programs[j];
				if (iprogram==NULL) {
					continue;
				}
				if (iprogram->n == service) {
					return iprogram;
				}
			}

			break;
		}
	}

	return NULL;
}

/*
 * Merge dvb2 into dvb1:
 */
void info_dvb_merge (INFO_DVB *dvb1, INFO_DVB *dvb2) {

	INFO_CHANNEL *ichannel1 = NULL, *ichannel2 = NULL;

	int i, j;

	if (dvb1 == NULL) {
		return;
	}

	if (dvb2 == NULL) {
		return;
	}

	while (dvb2->channels_len > 0) {

		ichannel2 = dvb2->channels[0];

		for (j = 0; j < dvb1->channels_len; j++) {

			ichannel1 = dvb1->channels[j];

			if (ichannel1->n == ichannel2->n) {

				// encontrado, remove it
				info_dvb_remove (dvb1, ichannel1);

				info_channel_free (ichannel1);

				break;
			}
		}

		info_dvb_add (dvb1, ichannel2);
		/* reasign user number to programs of channel */
		for (j = 0; j < ichannel2->programs_len; j++) {
			ichannel2->programs[j]->user_number = dvb1->user_number++;
		}

		info_dvb_remove (dvb2, ichannel2);

	}

}

static void _info_dvb_parse_p (int *value, const mydvb_tune_param *lista, char *token) {
	int i=0;

	while (lista[i].name!=NULL) {
		if (strcasecmp (lista[i].name, token)==0) {
			*value = lista[i].value;
			break;
		}
		i++;
	}
}

int info_dvb_load (const char *name, INFO_DVB *dvb) {

	FILE *f;
	char linea[1024];
	char *s;
	int st = 0;
	int v;
	INFO_CHANNEL *canal = NULL;
	INFO_PROGRAM *program = NULL;
	INFO_STREAM *stream = NULL;

	f = fopen (name, "r");
	if (!f) {
		return -1;	
	}

	fgets (linea, sizeof(linea), f);
	while (!feof(f)) {
		if (linea[0]!='-') {
			// se crea un nuevo registro para el canal:
			canal = info_channel_new();
			// parsing channel
			st = 0;
			s = tokenizer (linea, ":");
			while (s!=NULL) {
				switch (st) {
					case 0: // channel number
						v = atoi (s);
						canal->n = v;
						break;
					case 1: // channel frequency
						v = atoi (s);
						canal->p.frequency = v;
						break;
					case 2: // inversion
						_info_dvb_parse_p (&v, inversion_list, s);
						canal->p.inversion = v;
						break;
					case 3: // bandwith
						_info_dvb_parse_p (&v, bw_list, s);
						canal->p.u.ofdm.bandwidth = v;
						break;
					case 4: // fec
						_info_dvb_parse_p (&v, fec_list, s);
						canal->p.u.ofdm.code_rate_HP = v;
						break;
					case 5: // fec
						_info_dvb_parse_p (&v, fec_list, s);
						canal->p.u.ofdm.code_rate_LP = v;
						break;
					case 6: // qam
						_info_dvb_parse_p (&v, constellation_list, s);
						canal->p.u.ofdm.constellation = v;
						break;
					case 7: // transmision
						_info_dvb_parse_p (&v, transmissionmode_list, s);
						canal->p.u.ofdm.transmission_mode = v;
						break;
					case 8: // guard
						_info_dvb_parse_p (&v, guard_list, s);
						canal->p.u.ofdm.guard_interval = v;
						break;
					case 9: // hierarchy
						_info_dvb_parse_p (&v, hierarchy_list, s);
						canal->p.u.ofdm.hierarchy_information = v;
						break;
				}
				s = tokenizer (NULL, ":");
				st ++;
			}
			info_dvb_add (dvb, canal);
		} else if (linea[1]!='-') {
			program = info_program_new ();
			st = 0;
			s = tokenizer (linea+1,":");
			while (s!=NULL) {
				if (s[0]!='\0') { // cadena vacia
					switch (st) {
						case 0: // program index (in current channel)
							v = atoi (s);
							program->n = v;
							break;
						case 1: // provider
							program->provider = mydvb_text_create (s, strlen(s));//(unsigned char *) strdup (s);
							break;
						case 2: // service
							program->service = mydvb_text_create (s, strlen(s)); // (unsigned char *) strdup (s);
							break;
						case 3: // program number
							v = atoi(s);
							program->number = v;
							break;
						case 4: // program pid
							v = atoi(s);
							program->pid = v;
							break;
						case 5: /* program type */
							v = atoi(s);
							program->type = v;
							break;
						case 6: /* scrambled */
							v = atoi (s);
							program->scrambled = v;
							break;
						case 7: /* player/user channel number */
							v = atoi (s);
							program->user_number = v;
							if (v > dvb->user_number) {
								dvb->user_number = v+1;
							}
							break;
					}
				}
				s = tokenizer (NULL, ":");
				st ++;
			}
			// tras la lectura enganchamos el programa al canal
			info_channel_add (canal, program);
		} else {
			stream = info_stream_new ();
			st = 0;
			s = tokenizer (linea, ":");
			while (s!=NULL) {
				if (s[0]!='\0') { // cadena vacia
					switch (st) {
						case 0: // stream index
							v = atoi (s);
							stream->n = v;
							break;
						case 1: // stream type
							v = atoi (s);
							stream->type = v;
							break;
						case 2: // stream pid
							v = atoi (s);
							stream->pid = v;
							break;
					}
				}
				s = tokenizer (NULL,":");
				st ++;
			}
			info_program_add (program, stream);
		}
		fgets (linea, sizeof(linea), f);
	}
	fclose (f);

	return 0;
}

int info_dvb_save (const char *name, INFO_DVB *dvb) {

	int i,j,k;
	INFO_CHANNEL *ichannel = NULL;
	INFO_PROGRAM *iprogram = NULL;
	INFO_STREAM *istream = NULL;
	struct dvb_frontend_parameters *p;
	FILE *f;

	char *utf8 = NULL;

	if (name==NULL || dvb==NULL) {
		return -1;
	}

	f = fopen (name, "w");

	if (!f) {
		return -1;
	}

	for (i=0; i < dvb->channels_len; i++) {
		ichannel = dvb->channels[i];

		if (ichannel!=NULL) {
			p = &ichannel->p;
		
			fprintf (f, "%d:%lu:%s:%s:%s:%s:%s:%s:%s:%s\n",
				(p->frequency / 1000000 - 306)  / 8,
				(unsigned long) p->frequency,
				mydvb_tune_param_search (inversion_list, p->inversion),
			    mydvb_tune_param_search (bw_list, p->u.ofdm.bandwidth),
				mydvb_tune_param_search (fec_list, p->u.ofdm.code_rate_HP),
				mydvb_tune_param_search (fec_list, p->u.ofdm.code_rate_LP),
			    mydvb_tune_param_search (constellation_list, p->u.ofdm.constellation),
			    mydvb_tune_param_search (transmissionmode_list, p->u.ofdm.transmission_mode),
			    mydvb_tune_param_search (guard_list, p->u.ofdm.guard_interval),
			    mydvb_tune_param_search (hierarchy_list, p->u.ofdm.hierarchy_information));
			
			for (j=0; j < ichannel->programs_len; j++) {
				iprogram = ichannel->programs[j];
				if (iprogram!=NULL) {
					fprintf (f,"-%d", // -program_number
							iprogram->n
							);

					fprintf (f,":");
					utf8 = mydvb_text_to_utf8 (iprogram->provider);
					fprintf (f, utf8); // provider
					if (utf8) free (utf8);

					fprintf (f,":");
					utf8 = mydvb_text_to_utf8 (iprogram->service);
					fprintf (f, utf8);
					if (utf8) free (utf8);

					fprintf (f,":%d:%d:%d:%d:%d\n", // number:pmt_pid:type:scrambled:user_number
							iprogram->number,
							iprogram->pid,
							iprogram->type,
							iprogram->scrambled,
							iprogram->user_number
							);

					for (k=0; k < iprogram->streams_len; k++) {
						istream = iprogram->streams[k];
						if (istream!=NULL) {
							fprintf (f,"--%d:%d:%d\n", 
								istream->n,
								istream->type,
								istream->pid);							
						}
					}
				}
			}
		}
	}

	fclose (f);
	
	return 0;
}

/**
 * Save the dvb information using channels.conf format
 */
int info_dvb_save_channels_conf (const char *name, INFO_DVB *dvb) {

	int i,j,k;
	INFO_CHANNEL *ichannel = NULL;
	INFO_PROGRAM *iprogram = NULL;
	INFO_STREAM *istream = NULL;
	struct dvb_frontend_parameters *p;
	FILE *f;

	int apid;
	int vpid;

	char *utf8;

	if (name==NULL || dvb==NULL) {
		return -1;
	}

	f = fopen (name, "w");

	if (!f) {
		return -1;
	}

	for (i=0; i < dvb->channels_len; i++) {
		ichannel = dvb->channels[i];

		if (ichannel!=NULL) {
			p = &ichannel->p;
			
			for (j=0; j < ichannel->programs_len; j++) {
				iprogram = ichannel->programs[j];
				if (iprogram!=NULL) {

				  vpid = 0;
				  apid = 0;

				  for (k=0; k < iprogram->streams_len; k++) {
					istream = iprogram->streams[k];
					if (istream!=NULL) {
					   if (istream->type == 0x01 || istream->type == 0x02) { // video stream
						if (vpid == 0) vpid = istream->pid;
					   } else if (istream->type == 0x03 || istream->type == 0x4) { // audio stream
						if (apid == 0) apid = istream->pid;
					   }							
					}
				  }	

				  utf8 = mydvb_text_to_utf8 (iprogram->service);

				  fprintf (f, "%s:%lu:%s:%s:%s:%s:%s:%s:%s:%s:%d:%d:%d\n",
				    (unsigned char *) (utf8 ==NULL?"":utf8),
					(unsigned long) p->frequency,
					mydvb_tune_param_search (inversion_list, p->inversion),
			      	mydvb_tune_param_search (bw_list, p->u.ofdm.bandwidth),
					mydvb_tune_param_search (fec_list, p->u.ofdm.code_rate_HP),
					mydvb_tune_param_search (fec_list, p->u.ofdm.code_rate_LP),
			      	mydvb_tune_param_search (constellation_list, p->u.ofdm.constellation),
			       	mydvb_tune_param_search (transmissionmode_list, p->u.ofdm.transmission_mode),
			       	mydvb_tune_param_search (guard_list, p->u.ofdm.guard_interval),
			       	mydvb_tune_param_search (hierarchy_list, p->u.ofdm.hierarchy_information),
					vpid,
					apid,
					iprogram->number
					);

				  if (utf8) free (utf8);
				}
			}
		}
	}

	fclose (f);
	
	return 0;
}

