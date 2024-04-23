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

#ifndef __INFO_DVB__
#define __INFO_DVB__

#include <mydvb_info_dvb.h>


/*
	mrl: dvbt://channel_number/program_index
	example:
		dvbt://57/2
		will play the second program (audio, video and teletext streams) of
		the channel number 57
	
	the plugin always loads then channel list from disc at startup
	
	
*/


INFO_STREAM *info_stream_new ();
void info_stream_free (INFO_STREAM *stream);

INFO_PROGRAM *info_program_new ();
void info_program_free (INFO_PROGRAM *program);
int info_program_add (INFO_PROGRAM *program, INFO_STREAM *stream);

INFO_CHANNEL *info_channel_new ();
void info_channel_free (INFO_CHANNEL *canal);
int info_channel_add (INFO_CHANNEL *canal, INFO_PROGRAM *program);


#endif
