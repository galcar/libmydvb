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

static char *__mydvb_running_status_table[MYDVB_RUNNING_STATUS_LEN] = {
	"undefined",
	"not running",
	"starts in a few seconds",
	"pausing",
	"running",
	"reserved",
	"reserved",
	"reserved"
};

static char *__mydvb_stream_type_table[MYDVB_STREAM_TYPE_LEN] = {
	"reserved",
	"ISO/IEC 11172-2 (MPEG-1 video)",
	"ITU-T Rec. H.262 and ISO/IEC 13818-2 (MPEG-2 higher rate interlaced video)",
	"ISO/IEC 11172-3 (MPEG-1 audio)",
	"ISO/IEC 13818-3 (MPEG-2 halved sample rate audio)",
	"ITU-T Rec. H.222 and ISO/IEC 13818-1 (MPEG-2 tabled data)",
	"ITU-T Rec. H.222 and ISO/IEC 13818-1 (MPEG-2 packetized data)",
	"ISO/IEC 13522 (MHEG)",
	"ITU-T Rec. H.222 and ISO/IEC 13818-1 DSM CC",
	"ITU-T Rec. H.222 and ISO/IEC 13818-1/11172-1 auxiliary data",
	"ISO/IEC 13818-6 DSM CC multiprotocol encapsulation",
	"ISO/IEC 13818-6 DSM CC U-N messages",
	"ISO/IEC 13818-6 DSM CC stream descriptors",
	"ISO/IEC 13818-6 DSM CC tabled data",
	"ISO/IEC 13818-1 auxiliary data",
	"ISO/IEC 13818-7 ADTS AAC (MPEG-2 lower bit-rate audio)",
	"ISO/IEC 14496-2 (MPEG-4 H.263 based video)",
	"ISO/IEC 14496-3 (MPEG-4 LOAS multi-format framed audio)",
	"ISO/IEC 14496-1 (MPEG-4 FlexMux)",
	"ISO/IEC 14496-1 (MPEG-4 FlexMux) in ISO/IEC 14496 tables",
	"ISO/IEC 13818-6 DSM CC synchronized download protocol",
	"Packetized metadata",
	"Sectioned metadata",
	"ISO/IEC 13818-6 DSM CC Data Carousel metadata",
	"ISO/IEC 13818-6 DSM CC Object Carousel metadata",
	"ISO/IEC 13818-6 Synchronized Download Protocol metadata",
	"ISO/IEC 13818-11 IPMP",
	"ITU-T Rec. H.264 and ISO/IEC 14496-10 (lower bit-rate video)",
	"ISO/IEC 14496-3 (MPEG-4 raw audio)",
	"ISO/IEC 14496-17 (MPEG-4 text)",
	"ISO/IEC 23002-3 (MPEG-4 auxiliary video)",
	"ISO/IEC 14496-10 SVC (MPEG-4 AVC sub-bitstream)",
	"ISO/IEC 14496-10 MVC (MPEG-4 AVC sub-bitstream)",
	"ITU-T Rec. T.800 and ISO/IEC 15444 (JPEG 2000 video)",
	"reserved",
	"reserved",
	"ITU-T Rec. H.265 and ISO/IEC 23008-2 (Ultra HD video)",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"reserved"
	/*
	66	0x42	Chinese Video Standard
	in a packetized stream
	67 - 126	0x43 - 0x7e	Reserved.
	127	0x7f	ISO/IEC 13818-11 IPMP (DRM)
	in a packetized stream
	128	0x80	ITU-T Rec. H.262 and ISO/IEC 13818-2 with DES-64-CBC encryption for DigiCipher II
	or PCM audio for Blu-ray
	in a packetized stream
	129	0x81	Dolby Digital (AC-3) up to six channel audio for ATSC and Blu-ray
	in a packetized stream
	130	0x82	SCTE subtitle
	or DTS 6 channel audio for Blu-ray
	in a packetized stream
	131	0x83	Dolby TrueHD lossless audio for Blu-ray
	in a packetized stream
	132	0x84	Dolby Digital Plus (enhanced AC-3) up to 16 channel audio for Blu-ray
	in a packetized stream
	133	0x85	DTS 8 channel audio for Blu-ray
	in a packetized stream
	134	0x86	SCTE-35[5] digital program insertion cue message
	or DTS 8 channel lossless audio for Blu-ray
	in a packetized stream
	135	0x87	Dolby Digital Plus (enhanced AC-3) up to 16 channel audio for ATSC
	in a packetized stream
	136 - 143	0x88 - 0x8F	Privately defined.
	144	0x90	Blu-ray Presentation Graphic Stream (subtitling)
	in a packetized stream
	145	0x91	ATSC DSM CC Network Resources table
	146 - 191	0x92 - 0xBF	Privately defined.
	192	0xC0	DigiCipher II text
	in a packetized stream
	193	0xC1	Dolby Digital (AC-3) up to six channel audio with AES-128-CBC data encryption
	in a packetized stream
	194	0xC2	ATSC DSM CC synchronous data
	or Dolby Digital Plus up to 16 channel audio with AES-128-CBC data encryption
	in a packetized stream
	195 - 206	0xC3 - 0xCE	Privately defined.
	207	0xCF	ISO/IEC 13818-7 ADTS AAC with AES-128-CBC frame encryption
	in a packetized stream
	208	0xD0	Privately defined.
	209	0xD1	BBC Dirac (Ultra HD video)
	in a packetized stream
	210	0xD2	Audio Video Standard AVS2 (Ultra HD video)
	in a packetized stream
	211	0xD3	Audio Video Standard AVS3 Audio
	in a packetized stream
	212	0xD4	Audio Video Standard AVS3 Video (Ultra HD video)
	in a packetized stream
	213 - 218	0xD5 - 0xDA	Privately defined.
	219	0xDB	ITU-T Rec. H.264 and ISO/IEC 14496-10 with AES-128-CBC slice encryption
	in a packetized stream
	220 - 233	0xDC - 0xE9	Privately defined.
	234	0xEA	Microsoft Windows Media Video 9 (lower bit-rate video)
	in a packetized stream
	235 - 255	0xEB - 0xFF	Privately defined.
	*/
};

static char *__mydvb_service_type_table[MYDVB_SERVICE_TYPE_LEN] = {
	"reserved for future use",
	"digital television",
	"digital radio sound",
	"Teletext",
	"NVOD reference",
	"NVOD time-shifted",
	"mosaic",
	"FM radio",
	"DVB SRM",
	"reserved",
	"advanced codec digital radio sound",
	"advanced codec mosaic",
	"data broadcast",
	"reserved for Common Interface Usage",
	"RCS Map",
	"RCS FLS",
	"DVB MHP",
	"MPEG-2 HD digital television",
	"",
	"",
	"",
	"",
	"H.264/AVC SD digital television",
	"H.264/AVC SD NVOD time-shifted",
	"H.264/AVC SD SD NVOD reference",
	"H.264/AVC HD digital television",
	"H.264/AVC HD NVOD time-shifted",
	"H.264/AVC HD NVOD reference",
	"H.264/AVC frame compatible plano-stereoscopic HD digital television",
	"H.264/AVC frame compatible plano-stereoscopic HD NVOD time-shifted",
	"H.264/AVC frame compatible plano-stereoscopic HD NVOD reference",
	"HEVC digital television",
	"HEVC UHD digital television",
	/* 0x21 to 0x7f reserved for future use */
	"", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"",
	/* 0x80 to 0xfe user defined */
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "",
	/* 0xff reserved for future use */
	""
};

/* esta es la tabla de los valores de texto asociados con los content_descriptor */
static char *__mydvb_desc_content_table[256] = {
		"","","","","","","","","","","","","","","","", /* 0x00 to 0x0f: undefined */
		/* movie drama */
		"movie/drama (general)", 		/* 0x10 */
		"detective/thriller", 			/* 0x11 */
		"adventure/western/war",		/* 0x12 */
		"science fiction/fantasy/horror",/* 0x13 */
		"comedy",
		"soap/melodrama/folkloric",
		"romance",
		"serious/classical/religious/historical movie/drama",
		"adult movie/drama",
		"", "", "", "", "", "", /* 0x19 to 0x1E: reserved */
		"",	/* 0x1f: user defined */

		/*News/Current affairs */
		"news/current affairs (general)",
		"news/weather report",
		"news magazine",
		"documentary",
		"discussion/interview/debate",
		"", "", "", "", "", "", "", "", "", "", /* 0x25 to 0x2e: reserved */
		"", /* 0x2f: user defined */

		/* Show/Game show: */
		"show/game show (general)",
		"game show/quiz/contest",
		"variety show",
		"talk show",
		"", "", "", "", "", "", "", "", "", "", "", /* 0x34 to 0x3e: reserved */
		"", /* 0x3f: user defined */

		/* Sports */
		"sports (general)",
		"special events (Olympic Games,World Cup etc.)",
		"sports magazines",
		"football/soccer",
		"tennis/squash",
		"team sports (excluding football)",
		"athletics",
		"motor sport",
		"water sport",
		"winter sports",
		"equestrian",
		"martial sports",
		"", "", "", /* 0x4c to 0x4e: reserved */
		"", /* 0x4f: user defined */

		/* Children's/Youth programmes */
		"children's/youth programmes (general)",
		"pre-school children's programmes",
		"entertainment programmes for 6 to14",
		"entertainment programmes for 10 to 16"
		"informational/educational/school programmes",
		"cartoons/puppets",
		"", "", "", "", "", "", "", "", "", /* 0x56 to 0x5e: reserver */
		"", /* 0x5f: user defined */

		/* Music/Ballet/Dance:*/
		"music/ballet/dance (general)",
		"rock/pop",
		"serious music/classical music",
		"folk/traditional music",
		"jazz",
		"musical/opera",
		"ballet",
		"", "", "", "", "", "", "", "", /* 0x67 to 0x6E: reserved */
		"", /* 0x6f: user defined */

		/* Arts/Culture (without music) */
		"arts/culture (without music, general)",
		"performing arts",
		"fine arts",
		"religion",
		"popular culture/traditional arts",
		"literature",
		"film/cinema",
		"experimental film/video",
		"broadcasting/press",
		"new media",
		"arts/culture magazines",
		"fashion",
		"", "", "", /* 0x7c to 0x7E: reserved */
		"", /* 0x7f: user defined */

		/* Social/Political issues/Economics */
		"social/political issues/economics (general)",
		"magazines/reports/documentary",
		"economics/social advisory",
		"remarkable people",
		"", "", "", "", "", "", "", "", "", "", "", /* 0x84 to 0x8e: reserved */
		"", /* 0x8f: user defined */

		/* Children's/Youth programmes */
		/* Education/ Science/Factual topics */
		"education/science/factual topics (general)",
		"nature/animals/environment",
		"technology/natural sciences",
		"medicine/physiology/psychology",
		"foreign countries/expeditions",
		"social/spiritual sciences",
		"further education",
		"languages",
		"", "", "", "", "", "", "", /* 0x98 to 0x9e: reserved */
		"" /* 0x9F: user defined */

		/* Leisure hobbies */
		"leisure hobbies (general)",
		"tourism/travel",
		"handicraft",
		"motoring",
		"fitness & health",
		"cooking",
		"advertisement/shopping",
		"gardening",
		"", "", "", "", "", "", "", /* 0xa8 to 0xaE:  reserved */
		"", /* 0xaf: user defined */

		"original language",
		"black & white",
		"unpublished",
		"live broadcast",
		"", "", "", "", "", "", "", "", "", "", "", /* 0xb4 to 0xbE: reserved */
		"", /* 0xBF: user defined */

		"","","","","","","","","","","","","","","","", /* 0xc0 to 0xcf: reserved */
		"","","","","","","","","","","","","","","","", /* 0xd0 to 0xdf: reserved */
		"","","","","","","","","","","","","","","","", /* 0xe0 to 0xef: reserved */

		"","","","","","","","","","","","","","","",""  /* 0xf0 to 0xff: user*/

};

char **mydvb_running_status_table () {
	return __mydvb_running_status_table;
}

char **mydvb_stream_type_table () {
	return __mydvb_stream_type_table;
}

char **mydvb_service_type_table() {
	return __mydvb_service_type_table;
}

char **mydvb_desc_content_table() {
	return __mydvb_desc_content_table;
}

/**
	funcion devuelve el fichero demux asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_demux_file (MYDVB_TUNE *tune) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/demux%d", tune->adapter, tune->demuxer);

	return s;
}


/**
	funcion devuelve el fichero dvr asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_dvr_file (MYDVB_TUNE *tune) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/dvr%d", tune->adapter, tune->demuxer);

	return s;
}

/**
	funcion devuelve el fichero frontend asociado al adaptador y dispositivo actual
	La cadena es estatica por lo que su contenido puede verse alterado en 
	sucesivas llamadas a la funcion
*/
char *__mydvb_get_frontend_file (MYDVB_TUNE *tune) {
	static char s[64];
	
	sprintf (s, "/dev/dvb/adapter%d/frontend%d", tune->adapter, tune->device);

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




/* localiza un mydvb adecuado para television digital terrestre */
MYDVB_TUNE *mydvb_get_dvbt (MYDVB_ENGINE *engine) {
	int i;
	MYDVB_TUNE *tune = NULL;

	if (engine==NULL) {
		return NULL;
	}

	for (i=0; i < dyn_array_get_size (engine->tuners); i++) {
		tune = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));

		if (mydvb_tune_is_dvbt (tune)) {
			break;
		}
	}

	return tune;
}

/* localiza un mydvb adecuado para transmisiones por satelite */
MYDVB_TUNE *mydvb_get_dvbs (MYDVB_ENGINE *engine) {
	int i;
	MYDVB_TUNE *tune = NULL;

	if (engine==NULL) {
		return NULL;
	}

	for (i=0; i < dyn_array_get_size (engine->tuners); i++) {
		tune = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));
		if (mydvb_tune_is_dvbs (tune)) {
			break;
		}
	}

	return tune;
}

/* find an appropiate tune for cable tv */
MYDVB_TUNE *mydvb_get_dvbc (MYDVB_ENGINE *engine) {
	int i;
	MYDVB_TUNE *tune = NULL;

	if (engine==NULL) {
		return NULL;
	}

	for (i=0; i < dyn_array_get_size (engine->tuners); i++) {
		tune = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));
		if (mydvb_tune_is_dvbc (tune)) {
			break;
		}
	}

	return tune;
}

/**
 * DVB initialization
*/
MYDVB *mydvb_new (mydvb_tuner_type_t type, int channel) {

	MYDVB *mydvb = (MYDVB *) malloc (sizeof (MYDVB));

	mydvb->type 	= type;
	mydvb->channel 	= channel;

	mydvb->pat = NULL;
	mydvb->cat = NULL;
	mydvb->sdt = NULL;

	mydvb->aits = dyn_array_new (sizeof(MYDVB_AIT *));

	mydvb->ready = 0;

	return mydvb;
}

/**
 * dvb closing and free resources
*/
void mydvb_end (MYDVB *mydvb) {

	dyn_array_free (mydvb->aits);

	free (mydvb);

}




int __mydvb_add_pes (MYDVB_TUNE *tune, int pid, int type) {

	int fd;
	char *s;
	int et;

	if (tune == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (tune);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error opening demux data stream %s: %s", s, strerror(errno));
		return -1;
	}

	et = __mydvb_add_stream_filter (fd, pid, type, DMX_IN_FRONTEND);
	if (et==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error adding stream filter: %s", strerror(errno));
		close (fd);
		return -1;
	}

	/* add it to the pes demux filedescriptors table */
	tune->pes_fd[tune->pes_fd_len].pid = pid;
	tune->pes_fd[tune->pes_fd_len].fd = fd;
	tune->pes_fd_len ++;

	return 0;

}

void __mydvb_remove_pes (MYDVB_TUNE *tune, int pid) {

	if (tune == NULL) {
		return;
	}

	for (int i = 0; i < tune->pes_fd_len; i++) {

		if (tune->pes_fd[i].pid == pid && tune->pes_fd[i].fd != -1) {

			ioctl (tune->pes_fd[i].fd, DMX_STOP);

			close (tune->pes_fd[i].fd);
			tune->pes_fd[i].fd = -1;
			tune->pes_fd[i].pid = 0;

			for (int j=i+1; j < tune->pes_fd_len; j++) {
				tune->pes_fd[j-1].pid = tune->pes_fd[j].pid;
				tune->pes_fd[j-1].fd  = tune->pes_fd[j].fd;
			}
			tune->pes_fd_len --;
		}
	}
}

int __mydvb_add_sct (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, int pid, int tid) {

	int fd;
	char *s;
	int et;

	if (engine == NULL || tune == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (tune);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error opening demux data table %s: %s", s, strerror(errno));
		return -1;
	}

	et = __mydvb_add_table_filter (fd, pid, tid);

	if (et==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error adding table filter: %s", strerror(errno));
		close (fd);
		return -1;
	}

	mydvb_add_poll (engine, fd);

	/* add it to the sct demux filedescriptors table */
	tune->sct_fd[tune->sct_fd_len].pid = pid;
	tune->sct_fd[tune->sct_fd_len].fd = fd;
	tune->sct_fd_len ++;

	return 0;

}

void __mydvb_remove_sct (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, int pid) {

	if (engine == NULL || tune == NULL) {
		return;
	}

	for (int i = 0; i < tune->sct_fd_len; i++) {

		if (tune->sct_fd[i].pid == pid && tune->sct_fd[i].fd != -1) {

			mydvb_remove_poll (engine, tune->sct_fd[i].fd);

			ioctl (tune->sct_fd[i].fd, DMX_STOP);

			close (tune->sct_fd[i].fd);
			tune->sct_fd[i].fd = -1;
			tune->sct_fd[i].pid = 0;

			for (int j=i+1; j < tune->sct_fd_len; j++) {
				tune->sct_fd[j-1].pid = tune->sct_fd[j].pid;
				tune->sct_fd[j-1].fd  = tune->sct_fd[j].fd;
			}
			tune->sct_fd_len --;
		}
	}
}

int __mydvb_add_scts (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, int pid, int tid, unsigned char mask) {

	int fd;
	char *s;
	int et;

	if (engine == NULL || tune == NULL) {
		return -1;
	}

	s = __mydvb_get_demux_file (tune);

	fd = open (s, O_RDWR);
	if (fd==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error opening demux %s: %s", s, strerror(errno));
		return -1;
	}

	//
	et = __mydvb_add_table_filters (fd, pid, tid, mask);

	if (et==-1) {
		mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error adding table filter: %s", strerror(errno));
		close (fd);
		return -1;
	}

	mydvb_add_poll (engine, fd);

	/* add it to the sct demux filedescriptors table */
	tune->sct_fd[tune->sct_fd_len].pid = pid;
	tune->sct_fd[tune->sct_fd_len].fd = fd;
	tune->sct_fd_len ++;

	return 0;

}



/**
 * Decoding startup: main tables startup: PAT, SDT, CAT, EIT
 */
void mydvb_start_decoder (MYDVB_ENGINE *engine, MYDVB_TUNE *tune) {

	int i;
	MYDVB *mydvb = NULL;
	MYDVB_EVENT event;


	// find the decoder
	for (i=0; i < dyn_array_get_size (engine->channels); i++) {
		MYDVB *decoder = *((MYDVB **) dyn_array_get_data (engine->channels, i));

		if (decoder->type == tune->type && decoder->channel == tune->channel) {
			// found
			mydvb = decoder;
			break;
		}
	}

	if (mydvb == NULL) { // not found, create a new one
		mydvb = mydvb_new (tune->type, tune->channel);
		dyn_array_add (engine->channels, &mydvb);
	}

	/* add required tables */

	/* add the PAT table */
	__mydvb_add_sct (engine, tune, 0, 0);

	__mydvb_add_pes (tune, 0, DMX_PES_OTHER);

	/* add the CAT table */
	//__mydvb_add_sct (engine, tune, 0x01, 0x01);

	/* add the SDT table */
	__mydvb_add_sct (engine, tune, 0x11, 0x42);

	// start with the eit table
	__mydvb_add_scts (engine, tune, 0x12, 0x4e, 0xe0);

	/* start with the tdt table */
	//__mydvb_add_sct (engine, tune, 0x14, 0x70);

	/* vdr receives data from this program/channel */
	char *s = __mydvb_get_dvr_file (tune);
	tune->fd_dvr = open (s, O_RDONLY | O_NONBLOCK);

	mydvb_add_poll (engine, tune->fd_dvr);

	tune->mydvb = mydvb;

	if (mydvb_ready(mydvb)) {
		event.type = MYDVB_EVENT_READY;
		event.ready.tuner = tune;
		mydvb_notify_event (engine, &event);
	}

}


/**
 * Stop decoding
 */
void mydvb_stop_decoder (MYDVB_ENGINE *engine, MYDVB_TUNE *tune) {
	int i;

	if (engine == NULL || tune == NULL) {
		return;
	}

	/* close the vdr */
	if (tune->fd_dvr != -1) {

		mydvb_remove_poll (engine, tune->fd_dvr);

		close (tune->fd_dvr);
		tune->fd_dvr = -1;
	}

	/* stop and close all sct demux descriptors */
	for (i=0; i < tune->sct_fd_len; i++) {
		if (tune->sct_fd[i].fd != -1) {

			mydvb_remove_poll (engine, tune->sct_fd[i].fd);

			ioctl (tune->sct_fd[i].fd, DMX_STOP);
			close (tune->sct_fd[i].fd);
			tune->sct_fd[i].fd = -1;
			tune->sct_fd[i].pid = 0;
		}
	}
	tune->sct_fd_len = 0;

	/* stop and close all pes demux descriptors */
	for (i=0; i < tune->pes_fd_len; i++) {
		if (tune->pes_fd[i].fd != -1) {

			mydvb_remove_poll (engine, tune->pes_fd[i].fd);

			ioctl (tune->pes_fd[i].fd, DMX_STOP);
			close (tune->pes_fd[i].fd);
			tune->pes_fd[i].fd = -1;
			tune->pes_fd[i].pid = 0;
		}
	}
	tune->pes_fd_len = 0;

	tune->mydvb = NULL;

}

int mydvb_start_program (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, MYDVB_PROGRAM *program) {

	int r = 0;
	MYDVB_EVENT event;

	if (engine == NULL || tune == NULL || program == NULL) {
		return -1;
	}

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Starting program number %d with pid %d", program->number, program->pmt_pid);

	// add the pmt table descriptor to decoding
	r = __mydvb_add_sct (engine, tune, program->pmt_pid, 0x02);

	// add the pmt pid to pes
	r = __mydvb_add_pes (tune, program->pmt_pid, DMX_PES_OTHER);

	// event callback
	if (program->ready) {
		event.type = MYDVB_EVENT_PROGRAM;
		event.program.program = program;
		event.program.tuner = tune;

		mydvb_notify_event (engine, &event);
	}

	return 0;
}

void mydvb_stop_program (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, MYDVB_PROGRAM *program) {

	if (engine == NULL || tune == NULL || program == NULL) {
		return;
	}

	for (int i = 0; i < dyn_array_get_size (program->streams); i++) {
		MYDVB_STREAM  *stream = dyn_array_get_data (program->streams, i);
		mydvb_stop_stream (tune, stream);
	}

	__mydvb_remove_pes (tune, program->pmt_pid);

	__mydvb_remove_sct (engine, tune, program->pmt_pid);

}

int mydvb_start_stream (MYDVB_TUNE *tune, MYDVB_STREAM *stream) {

	int r;

	if (tune == NULL || stream == NULL) {
		return -1;
	}

	r = __mydvb_add_pes (tune, stream->pid, DMX_PES_OTHER);

	return r;

}

void mydvb_stop_stream (MYDVB_TUNE *tune, MYDVB_STREAM *stream) {

	if (tune == NULL || stream == NULL) {
		return;
	}

	__mydvb_remove_pes (tune, stream->pid);

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
			mydvb_add_poll (mydvb, stream->fd);
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

int __read_table (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, int fd, unsigned char *buffer) {

	int i;
	int n = 0;
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

	MYDVB *mydvb = tune->mydvb;

	if (mydvb == NULL) return 0;

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

					__mydvb_add_sct (engine, tune, mydvb->pat->nit->pid, 0x40);

				}

				if (!mydvb->ready) {
					if (mydvb_ready (mydvb)) {
						event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

						event->type = MYDVB_EVENT_READY;
						event->ready.tuner = tune;

						mydvb_notify_event (engine, event);

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

									mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Program number %d edited: stream %d, type %d, pid %d", number, j,
									 	stream->type,
									 	stream->pid);

									if (stream->type==0x05) { /* a MHP application stream */
										//__add_ait_decoder (mydvb, stream);
									}

								}

								program->ready = 1;

								// event callback
								event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

								event->type = MYDVB_EVENT_PROGRAM;
								event->program.program = program;
								event->program.tuner = tune;

								mydvb_notify_event (engine, event);

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
				if (mydvb_ready (mydvb)) {
					event = (MYDVB_EVENT *) malloc (sizeof(MYDVB_EVENT));

					event->type = MYDVB_EVENT_READY;
					event->ready.tuner = tune;

					mydvb_notify_event (engine, event);

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
 * find and return the first available tuner (status = NOOP, references = 0)
 */
MYDVB_TUNE *mydvb_get_available_tuner (MYDVB_ENGINE *engine, mydvb_tuner_type_t type) {
	int i;

	for (i = 0; i < dyn_array_get_size (engine->tuners); i++) {
		MYDVB_TUNE *t = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));

		if (t->type == type && t->status == TUNER_STATUS_NOOP && t->references == 0) {
			return t;
		}
	}

	return NULL;
}


MYDVB_TUNE *mydvb_tuner_channel (MYDVB_ENGINE *engine, INFO_CHANNEL *ichannel) {

	int r;
	int i;

	MYDVB_EVENT event;

	MYDVB_TUNE *mytune = NULL;

	for (i = 0; i < dyn_array_get_size (engine->tuners); i++) {
		MYDVB_TUNE *t = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));

		if (t->status == TUNER_STATUS_LOCK && t->type == ichannel->type && t->channel == ichannel->n) {
			t->references ++;
			return t;
		}
	}

	for (i = 0; i < dyn_array_get_size (engine->tuners); i++) {
		MYDVB_TUNE *t = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, i));

		if (t->type == ichannel->type && t->status == TUNER_STATUS_NOOP) {
			t->references ++;
			mytune = t;
			break;
		}
	}

	if (mytune == NULL) { // no tuner available,
		return NULL;
	}

	mydvb_tune_open (mytune);
	/* add the tune filedescriptor to the poll system */
	mydvb_add_poll (engine, mytune->fd);

	memcpy (&mytune->p, &ichannel->p, sizeof(struct dvb_frontend_parameters));

	/* notify listeners about trying tuning */
	event.type = MYDVB_TUNER_EVENT;
	event.tuner.status = TUNER_STATUS_TRYING;
	event.tuner.tuner = mytune;

	mydvb_notify_event (engine, &event);

	/* tune */
	r = mydvb_tune_frontend (mytune);

	return mytune;

}

/**
 * Tune a channel of services (programs) based on the information of channels
 */
MYDVB_TUNE *mydvb_tuner_channel_by_number (MYDVB_ENGINE *engine, INFO_DVB *info_dvb, int channel_number) {

	int i;
	INFO_CHANNEL *ichannel = NULL;

	if (engine == NULL || info_dvb == NULL) {
		return NULL;
	}

	for (i=0; i < info_dvb->channels_len; i++) {

		ichannel = info_dvb->channels[i];

		if (ichannel->n==channel_number) {
			// ok, found

			return mydvb_tuner_channel (engine, ichannel);

		}
	}

	return NULL;
}

void mydvb_tuner_release (MYDVB_ENGINE *engine, MYDVB_TUNE *tuner) {

	if (engine == NULL || tuner == NULL) {
		return;
	}

	if (tuner->references <= 0) {
		return;
	}

	tuner->references --;

	if (tuner->references == 0) {

		tuner->data = NULL;

		mydvb_stop_decoder (engine, tuner);

		mydvb_remove_poll (engine, tuner->fd);

		mydvb_tune_close (tuner);

	}
}

void __tune_free (void *p) {
	MYDVB_TUNE *tune = *((MYDVB_TUNE **) p);

	mydvb_tune_close (tune);
}

/** engine **/
void mydvb_engine_init (MYDVB_ENGINE *engine) {

	int adapter;
	int device;
	MYDVB_TUNE *tune;
	int i;


	engine->poll_len = 0;
	for (i = 0; i < 32; i++) {
		engine->pfd[i].fd = -1;
	}

	engine->listener_cnt = 100;
	engine->listeners = dyn_array_new (sizeof(MYDVB_LISTENER));

	engine->mainloop = 0;

	/* initialize channels */
	engine->channels = dyn_array_new (sizeof (MYDVB *));

	/* load available tunes */

	engine->tuners = dyn_array_new (sizeof (MYDVB_TUNE *));
	dyn_array_set_free_callback (engine->tuners, __tune_free);

	for (adapter=0; adapter < MYDVB_MAX_ADAPTERS; adapter ++) {
		for (device=0; device < MYDVB_MAX_DEVICES; device ++) {
			// ok, create tunes
			tune = (MYDVB_TUNE *) malloc (sizeof(MYDVB_TUNE));
			if (mydvb_tune_init (tune, adapter, device) == 0){ // ok
				// add to mydvb list
				mydvb_log (MYDVB_LOG_LEVEL_INFO, "Found device %s at adapter %d, device %d", tune->name, adapter, device);

				dyn_array_add (engine->tuners, &tune);

			} else {
				free (tune);
				tune = NULL;
			}
		}
	}
}

void mydvb_engine_end (MYDVB_ENGINE *engine) {

	if (engine == NULL) {
		return;
	}

	dyn_array_free (engine->listeners);
	engine->listeners = NULL;

	dyn_array_free (engine->channels);
	engine->channels = NULL;

	dyn_array_free (engine->tuners);
	engine->tuners = NULL;

}

/**
 *
 * the dvb main loop
 */
void mydvb_main_loop (MYDVB_ENGINE *engine) {

	int r;
	int j=0;
	int i;
	int k;

	off_t n=0;
	static unsigned char buffer[2*4096]; /* twice the maximum size of a table section */

	MYDVB_TUNE *tune;

	MYDVB_EVENT event;

	int enc = 0;

	struct dvb_frontend_event f_event;

	long long t_out = current_timestamp_ms ();

	engine->mainloop = 1;

	while (engine->mainloop) {

		r = poll (engine->pfd, engine->poll_len, 1000);

		if (r > 0) {

			for (j=0; j < engine->poll_len; j++) {

				if (engine->pfd[j].revents & POLLPRI || engine->pfd[j].revents & POLLIN) {

					enc = 0;

					for (k=0; k < dyn_array_get_size (engine->tuners); k++) {
						tune = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, k));

						/* check for dvr event */
						if (engine->pfd[j].fd == tune->fd_dvr) {

							enc = 1;

							n = read (engine->pfd[j].fd, buffer, sizeof(buffer));
							if (n!=-1) {

								/* notify listeners */
								event.type = MYDVB_EVENT_STREAM;

								event.stream.buffer = buffer;
								event.stream.length = n;

								event.stream.tuner = tune;

								mydvb_notify_event (engine, &event);

							}

							break;

						/* check for frontend event */
						} else if (engine->pfd[j].fd == tune->fd) {

							enc = 1;

							memset (&f_event, 0, sizeof (struct dvb_frontend_event));

							if (ioctl(tune->fd, FE_GET_EVENT, &f_event) == 0) {

								mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Frontend Status %X, tune status %d: %d", f_event.status, tune->status, f_event.status & FE_TIMEDOUT);

								if (f_event.status & FE_TIMEDOUT) {

									if (tune->status == TUNER_STATUS_TRYING) {

										tune->status = TUNER_STATUS_FAILED;

										// fire event
										event.type = MYDVB_TUNER_EVENT;
										event.tuner.status = tune->status;
										event.tuner.tuner = tune;

										mydvb_notify_event (engine, &event);
									}

								} else //if (f_event.status & FE_HAS_LOCK) {
									if ((f_event.status & 0x1F) == 0x1F) { // locked and ok

										if (tune->status == TUNER_STATUS_TRYING) {

											tune->status = TUNER_STATUS_LOCK;

											// fire event
											event.type = MYDVB_TUNER_EVENT;
											event.tuner.status = tune->status;
											event.tuner.tuner = tune;

											mydvb_notify_event (engine, &event);
										}

								}

							} else {

								mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error getting status from frontend: %s", strerror(errno));

							}

							break;

						} else {

							/* check for tables */
							for (i = 0; i < tune->sct_fd_len; i++) {

								if (engine->pfd[j].fd == tune->sct_fd[i].fd) {

									/* decode table */

									__read_table (engine, tune, engine->pfd[j].fd, buffer);

									enc = 1;

									break;
								}
							}

							if (enc == 1) {
								break;
							}
						}
					}

					if (!enc) {
						/* this is, probably, an external event */
						event.type = MYDVB_EVENT_EXT_FD;

						event.external.fd = engine->pfd[j].fd;

						mydvb_notify_event (engine, &event);
					}

				}
			}

		} else if (r == 0) {

			// time out
			// do nothing for now

		} else {
			// error
			mydvb_log (MYDVB_LOG_LEVEL_ERROR, "Error in main_loop, getting data");
		}

		long long t_now = current_timestamp_ms ();
		if (t_now - t_out >= 1000) {
			t_out = t_now;

			// notify tune timeout
			for (k=0; k < dyn_array_get_size (engine->tuners); k++) {
				MYDVB_TUNE *tune = *((MYDVB_TUNE **) dyn_array_get_data (engine->tuners, k));

				if (tune->status == TUNER_STATUS_TRYING) {

					/* check time out tunning of device */
					if (t_now - tune->start_ms >= 3000) { /* the tuner has 3 seconds for locking */

						tune->status = TUNER_STATUS_FAILED;

						// fire event
						event.type = MYDVB_TUNER_EVENT;
						event.tuner.status = tune->status;
						event.tuner.tuner = tune;

						mydvb_notify_event (engine, &event);
					}

				}
			}


			// notify general time out event

			event.type = MYDVB_TIMEOUT_EVENT;

			event.timeout.count = t_now;

			mydvb_notify_event (engine, &event);

		}

	}

}

/**
 * set the main loop flat to 0, indicating that the main loop must terminate
 */
void mydvb_end_main_loop (MYDVB_ENGINE *engine) {

	engine->mainloop = 0;
}

/**
 * add a file descriptor to the poll system
 */
void mydvb_add_poll (MYDVB_ENGINE *engine, int fd) {

	if (engine == NULL || fd == -1) {
		return;
	}

	engine->pfd[engine->poll_len].fd = fd;
	engine->pfd[engine->poll_len].events = POLLPRI | POLLIN | POLLERR;
	engine->pfd[engine->poll_len].revents = 0;
	engine->poll_len++;

}



/**
 * remove a file descriptor from the poll system
 */
void mydvb_remove_poll (MYDVB_ENGINE *engine, int fd) {
	int i, j;

	if (engine == NULL || fd == -1) {
		return;
	}

	for (i=0; i < engine->poll_len; i++) {

		if (engine->pfd[i].fd == fd) { /* ok, encontrado */

			engine->poll_len --;

			for (j=i; j < engine->poll_len; j++) {

				engine->pfd[j].fd = engine->pfd[j+1].fd;
			}

			return;
		}
	}

}

