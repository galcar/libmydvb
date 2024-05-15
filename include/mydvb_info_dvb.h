/**
 * API Reducida de informacion DVB
 * Permite representar, almacena y acceder a la información suministrada por un sistema
 * DVB. La información se organiza de la siguiente manera:
 * Un sistema DVB (INFO_DVB) contiene una serie de canales (INFO_CHANNEL). Cada canal 
 * tiene una serie de programas o servicios (INFO_PROGRAM), formado por uno o 
 * varios flujos (INFO_STREAM)
 * Cada canal se identifica por un número, al igual que cada programa dentro del canal.
 * Los flujos de cada programa se seleccionan en base a los criterios de usuario, siendo
 * habitual la reproducción simultánea de los flujos de audio y vídeo, mientras que los
 * flujos de teletexto se seleccionan mediante entrada del usuario.
 * La información se obtiene mediante un "scan" de la señal DVB por todos los canales
 * posibles. Cada vez que se detecta un canal se extrae la información de programas y
 * flujos. Al terminar el "scan", se guarda la información en un fichero de texto.
 * Usando el fichero de texto se puede reconstruir la información de canales, programas y
 * flujos para su utilización en una guía de Canales.
*/

#ifndef __MYDVB_INFO_DVB__
#define __MYDVB_INFO_DVB__

#include <linux/dvb/frontend.h>

#include <mydvb_text.h>
#include <mydvb_tune.h>

/*
	mrl: dvbt://channel_number/program_index
	example:
		dvbt://57/2
		will play the second program (audio, video and teletext streams) of
		the channel number 57
	
	the plugin always loads then channel list from disc at startup
	
	
*/

typedef struct _info_stream INFO_STREAM;
typedef struct _info_program INFO_PROGRAM;
typedef struct _info_channel INFO_CHANNEL;

struct _info_stream {
	INFO_PROGRAM *program;	// program of the stream
	int n;					// number of stream in program (starting from 0)
	int type; 				// type of stream
	int pid; 				// pid of packets
};

struct _info_program {
	INFO_CHANNEL *channel;		// channel of the program
	int n; 						// number of program in channel (starting from 0)
	mydvb_text *provider;
	mydvb_text *service;
	
	int number; // program number
	int pid;	// pid of packets

	int type;			/* type of service (see mydvb_service_type_table) */
	char scrambled;		/* scrambled 1: yes, 0: no */

	int user_number;	/* player/user number, assigned by the user, unique in the system */

	int streams_len;
	INFO_STREAM **streams;
		
};

struct _info_channel {
	mydvb_tuner_type_t				type; // type of channel (dvbs, dvbc, dvbt...)
	int 							n;	// number of channel
	struct dvb_frontend_parameters 	p; // parameters of the channel
	
	int 							programs_len;
	INFO_PROGRAM 					**programs;
	
};

typedef struct {
	
	int channels_len;
	INFO_CHANNEL **channels;

	int user_number; // global user number counter (max of all user number)

} INFO_DVB;


INFO_DVB *info_dvb_new ();
void info_dvb_free (INFO_DVB *info);

int info_dvb_load (const char *name, INFO_DVB *dvb);
int info_dvb_save (const char *name, INFO_DVB *dvb);

int info_dvb_save_channels_conf (const char *name, INFO_DVB *dvb);

int info_dvb_add (INFO_DVB *dvb, INFO_CHANNEL *channel);
void info_dvb_remove (INFO_DVB *dvb, INFO_CHANNEL *channel);

INFO_PROGRAM *info_dvb_find (INFO_DVB *dvb, mydvb_tuner_type_t type, int channel, int service);

INFO_PROGRAM *info_dvb_get_by_user_number (INFO_DVB *dvb, int user_number);

void info_dvb_move_channel (INFO_DVB *dvb, int chn_from, int chn_to);

void info_dvb_merge (INFO_DVB *dvb1, INFO_DVB *dvb2);

#endif
