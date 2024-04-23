#ifndef __MYDVB_H__
#define __MYDVB_H__

#include <poll.h>

typedef struct _mydvb_engine MYDVB_ENGINE;

typedef struct _mydvb MYDVB;
typedef struct _mydvb_list MYDVB_LIST;

#include <dynarray.h>

#include <mydvb_info_dvb.h>

#include <mydvb_log.h>

#include <mydvb_tune.h>
#include <mydvb_tables.h>

#include <mydvb_text.h>

/*
original network id

0x0000 Reserved Reserved
0x0001 Astra Satellite Network 19,2°E Société Européenne des Satellites
0x0002 Astra Satellite Network 28,2°E Société Européenne des Satellites
0x0003 - 0x0019 Astra n (n =1-23) Société Européenne des Satellites
0x001A Quiero Televisión Quiero Televisión
0x001F Europe Online Networks (EON) Europe Online Networks S.A
0x0020 ASTRA Société Européenne des Satellites
0x0021 Hispasat Network 1 Hispasat S.A.
0x0022 Hispasat Network 2 Hispasat S.A.
0x0023 Hispasat Network 3 Hispasat S.A.
0x0024 Hispasat Network 4 Hispasat S.A.
0x0025 Hispasat Network 5 Hispasat S.A.
0x0026 Hispasat Network 6 Hispasat S.A.
0x0027 Hispasat 30°W Hispasat FSS
0x0028 Hispasat 30°W Hispasat DBS
0x0029 Hispasat 30°W Hispasat America
0x0030 Canal+ Satellite Network Canal+ SA°E
0x0031 Hispasat   VIA DIGITAL Hispasat S.A.
0x0032 Hispasat Network 7 Hispasat S.A.
0x0033 Hispasat Network 8 Hispasat S.A.
0x0034 Hispasat Network 9 Hispasat S.A.
0x0035 Nethold Main Mux System NetHold IMS
0x0036 TV Cabo TV Cabo Portugal
0x0037 STENTOR France Telecom, CNES and DGA
0x0038 OTE Hellenic Telecommunications Organization S.A.
0x0040 Croatian Post and Telecommunications HPT   Croatian Post and Telecommunications
0x0041 Mindport network Mindport
0x0046 1 degree W #1 Telenor
0x0047 1 degree W #2 Telenor
0x0048 STAR DIGITAL STAR DIGITAL A.S.
0x0049 Sentech Digital Satellite Sentech
0x0050 Croatian Radio and Television HRT   Croatian Radio and Television
0x0051 Havas Havas
0x0052 Osaka Yusen Satellite StarGuide Digital Networks
0x0055 Sirius Satellite System European Coverage NSAB (Teracom)
0x0058 (Thiacom 1 & 2 co-located 78.5°E) UBC Thailand
0x005E Sirius Satellite System Nordic Coverage NSAB
0x005F Sirius Satellite System FSS NSAB
0x0060 MSG MediaServices GmbH MSG MediaServices GmbH (0x0069) (Optus B3 156°E) (Optus Communications)
0x0070 BONUM1; 36 Degrees East NTV+ (0x0073) (PanAmSat 4 68.5°E) (Pan American Satellite System)
0x007E Eutelsat Satellite System at 7°E European Telecommunications Satellite Organization
0x007F Eutelsat Satellite System at 7°E EUTELSAT   European Telecommunications Satellite Organization
0x0085 BetaTechnik BetaTechnik
0x0090 National network TDF
0x00A0 National Cable Network News Datacom
0x00A1 News Satellite Network News Datacom
0x00A2 News Satellite Network News Datacom
0x00A3 News Satellite Network News Datacom
0x00A4 News Satellite Network News Datacom
0x00A5 News Satellite Network News Datacom
0x00A6 ART ART
0x00A7 Globecast France Telecom
0x00A8 Foxtel Foxtel
0x00A9 Sky New Zealand Sky New Zealand
0x00B0-0x00B3 TPS La Télévision Par Satellite
0x00B4 Telesat 107.3°W Telesat Canada
0x00B5 Telesat 111.1°W Telesat Canada
0x00B6 Telstra Saturn TelstraSaturn Limited
0x00BA Satellite Express   6 (80°E) Satellite Express
0x00C0-0x00CD Canal + Canal+
0x00EB Eurovision Network European Broadcasting Union
0x0100 ExpressVu ExpressVu Inc.
0x010E Eutelsat Satellite System at 10°E European Telecommunications Satellite Organization
0x010F Eutelsat Satellite System at 10°E EUTELSAT   European Telecommunications Satellite Organization
0x0110 Mediaset Mediaset 0x013E Eutelsat Satellite System 13°E European Telecommunications Satellite Organization
0x013F Eutelsat Satellite System at 13°E EUTELSAT   European Telecommunications Satellite Organization
0x016E Eutelsat Satellite System at 16°E European Telecommunications Satellite Organization
0x016F Eutelsat Satellite System at 16°E EUTELSAT   European Telecommunications Satellite Organization
0x022E Eutelsat Satellite System at 21.5°E EUTELSAT   European Telecommunications Satellite Organization
0x022F Eutelsat Satellite System at 21.5°E EUTELSAT   European Telecommunications Satellite Organization
0x026E Eutelsat Satellite System at 25.5°E EUTELSAT   European Telecommunications Satellite Organization
0x026F Eutelsat Satellite System at 25.5°E EUTELSAT   European Telecommunications Satellite Organization
0x029E Eutelsat Satellite System at 29°E European Telecommunications Satellite Organization
0x029F Eutelsat Satellite System at 28.5°E EUTELSAT   European Telecommunications Satellite Organization
0x02BE Arabsat Arabsat (Scientific Atlanta, Eutelsat)
0x036E Eutelsat Satellite System at 36°E European Telecommunications Satellite Organization
0x036F Eutelsat Satellite System at 36°E EUTELSAT   European Telecommunications Satellite Organization
0x03E8 Telia Telia, Sweden
0x047E Eutelsat Satellite System at 12.5°W EUTELSAT   European Telecommunications Satellite Organization
0x047F Eutelsat Satellite System at 12.5°W EUTELSAT   European Telecommunications Satellite Organization
0x048E Eutelsat Satellite System at 48°E European Telecommunications Satellite Organization
0x048F Eutelsat Satellite System at 48°E EUTELSAT   European Telecommunications Satellite Organization
0x052E Eutelsat Satellite System at 8°W EUTELSAT   European Telecommunications Satellite Organization
0x052F Eutelsat Satellite System at 8°W EUTELSAT   European Telecommunications Satellite Organization
0x0600 UPC Satellite UPC
0x0601 UPC Cable UPC
0x0602 Tevel Tevel Cable (Israel)
0x0800 Nilesat 101 Nilesat
0x0801 Nilesat 101 Nilesat
0x0880 MEASAT 1, 91.5°E MEASAT Broadcast Network Systems SDN. BHD. (Kuala Lumpur, Malaysia)

0x0882 MEASAT 2, 91.5°E MEASAT Broadcast Network Systems SDN. BHD. (Kuala Lumpur, Malaysia)
0x0883 MEASAT 2, 148.0°E Hsin Chi Broadcast Company Ltd.
0x088F MEASAT 3 MEASAT Broadcast Network Systems SDN. BHD. (Kuala Lumpur, Malaysia)
0x1000 Optus B3 156°E Optus Communications
0x1001 DISH Network Echostar Communications
0x1002 Dish Network 61.5 W Echostar Communications
0x1003 Dish Network 83 W Echostar Communications
0x1004 Dish Network 119 W Echostar Communications
0x1005 Dish Network 121 W Echostar Communications
0x1006 Dish Network 148 W Echostar Communications
0x1007 Dish Network 175 W Echostar Communications
0x1008 Dish Network W Echostar Communications
0x1009 Dish Network X Echostar Communications
0x100A Dish Network Y Echostar Communications
0x100B Dish Network Z Echostar Communications
0x1010 ABC TV Australian Broadcasting Corporation
0x1011 SBS SBS Australia
0x1012 Nine Network Australia Nine Network Australia
0x1013 Seven Network Australia Seven Network Limited
0x1014 Network TEN Australia Network TEN Limited
0x1015 WIN Television Australia WIN Television Pty Ltd
0x1016 Prime Television Australia Prime Television Limited
0x1017 Southern Cross Broadcasting Australia Southern Cross Broadcasting (Australia) Limited
0x1018 Telecasters Australia Telecasters Australia Limited
0x1019 NBN Australia NBN Limited
0x101A Imparja Television Australia Imparja Television Australia
0x101B Reserved for Australian broadcasters Reserved for Australian broadcasters
0x101C Reserved for Australian broadcasters Reserved for Australian broadcasters
0x101D Reserved for Australian broadcasters Reserved for Australian broadcasters
0x101E Reserved for Australian broadcasters Reserved for Australian broadcasters
0x101F Reserved for Australian broadcasters Reserved for Australian broadcasters
0x2000 Thiacom 1 & 2 co-located 78.5°E Shinawatra Satellite
0x2024 Australian Digital Terrestrial Television Australian Broadcasting Authority
0x20F6 Finnish Digital Terrestrial Television Telecommunicatoins Administratoin Centre, Finland
0x2114 German Digital Terrestrial Television IRT on behalf of the German DVB-T broadcasts
0x2174 Irish Digital Terrestrial Television Irish Telecommunications Regulator
0x2178 Israeli Digital Terrestrial Television BEZEQ (The Israel Telecommunication Corp Ltd.)
0x22BE Singapore Digital Terrestrial Television Singapore Broadcasting Authority
0x22D4 Spanish Digital Terrestrial Television  Spanish Broadcasting Regulator
0x22F1 Swedish Digital Terrestrial Television  Swedish Broadcasting Regulator
0x233A UK Digital Terrestrial Television Independent Television Commission
0x3000 PanAmSat 4 68.5°E Pan American Satellite System
0x5000 Irdeto Mux System Irdeto Test Laboratories
0x616D BellSouth Entertainment BellSouth Entertainment, Atlanta, GA, USA
0x6600 UPC Satellite UPC
0x6601 UPC Cable UPC
0xF000 Small Cable networks Small cable network network operators

0xF001 Deutsche Telekom Deutsche Telekom AG
0xF010 Telefónica Cable Telefónica Cable SA
0xF020 Cable and Wireless Communication Cable and Wireless Communications
0xF100 Casema Casema N.V.
0xFBFC MATAV MATAV (Israel)
0xFBFD Telia Kabel-TV Telia, Sweden
0xFBFE TPS La Télévision Par Satellite
0xFBFF Stream Stream Spa.
0xFC00 France Telecom Cable France Telecom
0xFC10 Rhône Vision Cable Rhône Vision Cable
0xFC41 France Telecom Cable France Telecom
0xFD00 National Cable Network Lyonnaise Communications
0xFE00 TeleDenmark Cable TV TeleDenmark
0xF750 Telewest Communications Cable Network Telewest Communications Plc
0xFEC0-FF00 Network Interface Modules Common Interface
0xFF00-FFFF Private_temporary_use ETSI

*/

#define MYDVB_RUNNING_STATUS_LEN 	8

static char *mydvb_running_status_table[MYDVB_RUNNING_STATUS_LEN] = {
	"undefined",
	"not running",
	"starts in a few seconds",
	"pausing",
	"running",
	"reserved",
	"reserved",
	"reserved"
};

#define MYDVB_STREAM_TYPE_LEN	15

static char *mydvb_stream_type_table[MYDVB_STREAM_TYPE_LEN] = {
	"reserved",
	"video",
	"video",
	"audio",
	"audio",
	"MHP-AIT",
	"Teletext",
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"DSM-CC (Object Caroussel)",
	"reserved",
	"reserved",
	"reserved"
};

/* strings literal for service types (descriptor 0x48) */
#define MYDVB_SERVICE_TYPE_LEN	256

static char *mydvb_service_type_table[MYDVB_SERVICE_TYPE_LEN] = {
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
	"advanced codec SD digital television",
	"advanced codec SD NVOD time-shifted",
	"advanced codec SD NVOD reference",
	"advanced codec HD digital television",
	"advanced codec HD NVOD time-shifted",
	"advanced codec HD NVOD reference",
	"advanced codec frame compatible plano-stereoscopic HD digital television",
	"advanced codec frame compatible plano-stereoscopic HD NVOD time-shifted",
	"advanced codec frame compatible plano-stereoscopic HD NVOD reference",
	/* 0x1f to 0x7f reserved for future use */
	"", "", "", "", "", "", "", "", "", "",
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
static char *mydvb_desc_content_table[256] = {
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


typedef enum {
	MYDVB_SEARCH_NOOP,
	MYDVB_SEARCH_SEARCHING,
	MYDVB_SEARCH_CANCELLED,
	MYDVB_SEARCH_COMPLETED,
	MYDVB_SEARCH_STOPPED
} mydvb_search_status_t;

/* --------------------------------------------------------------- */
/* Events section
/* --------------------------------------------------------------- */

typedef enum {
	MYDVB_EVENT_READY,
	MYDVB_EVENT_PROGRAM,
	MYDVB_EVENT_EXT_FD,
	MYDVB_EVENT_STREAM,
	MYDVB_TUNE_EVENT,
	MYDVB_SEARCH_EVENT,
	MYDVB_TIMEOUT_EVENT
} mydvb_event_type_t;



typedef struct {
	mydvb_event_type_t 			type;

} MYDVB_EVENT_DATA_READY;

typedef struct {
	mydvb_event_type_t 			type;

	MYDVB_PROGRAM 				*program;

} MYDVB_EVENT_PROGRAM_READY;

typedef struct {
	mydvb_event_type_t 			type;

	int 						fd;

} MYDVB_EVENT_EXTERNAL_FD;

typedef struct {

	mydvb_event_type_t 			type;

	//unsigned int				pid;

	unsigned char				*buffer;

	int							length;

} MYDVB_EVENT_DATA_STREAM;

typedef struct {
	mydvb_event_type_t 			type;

	TUNE_STATUS_T				status;

} MYDVB_EVENT_TUNE;

typedef struct {
	mydvb_event_type_t			type;

	mydvb_search_status_t		status;

	INFO_DVB					*info_dvb;

} MYDVB_EVENT_SEARCH;

typedef struct {
	mydvb_event_type_t			type;

	long long 					count;	/* current time in ms */

} MYDVB_EVENT_TIMEOUT;

typedef union {

	mydvb_event_type_t 			type;
		
	MYDVB_EVENT_DATA_READY		ready;
	MYDVB_EVENT_PROGRAM_READY 	program;

	MYDVB_EVENT_EXTERNAL_FD		external;

	MYDVB_EVENT_DATA_STREAM		stream;

	MYDVB_EVENT_TUNE			tune;

	MYDVB_EVENT_SEARCH			search;

	MYDVB_EVENT_TIMEOUT			timeout;

} MYDVB_EVENT;




struct FILE_PID {
	unsigned short 	pid;

	int 			fd;

};

struct _mydvb {

	MYDVB_TUNE						mytune;		/* tuner */


	/* global attributes */

	struct pollfd 					pfd[32];	/* the poll file descriptor table */
	int 							poll_len;	/* len of pfd */

	/* event management */
	DYN_ARRAY  						*listeners;
	int								event_propagation;

	int 							mainloop;	/* main loop in progress */


	/* decoding attributes */

	int 							fd_dvr;		/* descriptor del vdr */

	int								sct_fd_len; /* demux sct descriptors len */
	struct FILE_PID					sct_fd[32]; /* demux sct descriptors table */

	int								pes_fd_len;	/* demux pes descriptors len */
	struct FILE_PID					pes_fd[32];	/* demux pes descriptors table */

	MYDVB_PAT 						*pat;		/* the pat table */
	MYDVB_CAT 						*cat;		/* the cat table */
	MYDVB_SDT 						*sdt;		/* the sdt table */
	MYDVB_TDT 						tdt;		/* tdt table */

	DYN_ARRAY						*aits;		/* aits tables */

	int 							ready;		/* all basic elements (pat, sdt and programs) are ready */

};

struct _mydvb_list {
	MYDVB **list;
	int len;	
};

/* pending: */
struct _mydvb_engine {
	struct pollfd 					pfd[64];	/* the poll file descriptor table */
	int 							poll_len;	/* len of pfd */

	/* event management */
	DYN_ARRAY  						*listeners;
	int								event_propagation;

	int 							mainloop;	/* main loop in progress */

	DYN_ARRAY						*tunes;
};

struct _mydvb2 {
	/* decoding attributes */

	int 							fd_dvr;		/* descriptor del vdr */

	int								sct_fd_len; /* demux sct descriptors len */
	struct FILE_PID					sct_fd[32]; /* demux sct descriptors table */

	int								pes_fd_len;	/* demux pes descriptors len */
	struct FILE_PID					pes_fd[32];	/* demux pes descriptors table */

	MYDVB_PAT 						*pat;		/* the pat table */
	MYDVB_CAT 						*cat;		/* the cat table */
	MYDVB_SDT 						*sdt;		/* the sdt table */
	MYDVB_TDT 						tdt;		/* tdt table */

	DYN_ARRAY						*aits;		/* aits tables */

	int 							ready;		/* all basic elements (pat, sdt and programs) are ready */

};

/* -------- tune scan ---------------- */
typedef struct {
	MYDVB_TUNE_SCAN_CTRL 		*scan;
	INFO_DVB 					*dvb_info;

	mydvb_search_status_t		status;

	/* for restore later */
	DYN_ARRAY					*listeners;

} TUNE_SCAN_INFO;

/* ------------------------------------------- */
/* Get dvb devices and initialization functions */
/* ------------------------------------------- */

MYDVB_LIST *mydvb_list_new ();
void mydvb_list_free (MYDVB_LIST *l);

MYDVB *mydvb_list_get_dvbt (MYDVB_LIST *dvb_list);
MYDVB *mydvb_list_get_dvbs (MYDVB_LIST *dvb_list);
MYDVB *mydvb_list_get_dvbc (MYDVB_LIST *dvb_list);

int mydvb_init (MYDVB *mydvb);
void mydvb_end (MYDVB *mydvb);

/* ------------------------------------------- */
/* Tunning functions */
/* ------------------------------------------- */

int mydvb_tune_channel (MYDVB *mydvb, INFO_CHANNEL *ichannel);
int mydvb_tune_channel_by_number (MYDVB *mydvb, INFO_DVB *info_dvb, int channel_number);

TUNE_SCAN_INFO *mydvb_tune_search (MYDVB *mydvb, MYDVB_TUNE_SCAN_PARAMETER *tune_param);


/* ------------------------------------------- */
/* Decoding functions */
/* ------------------------------------------- */

void mydvb_start_decoder (MYDVB *mydvb);
void mydvb_end_decoder (MYDVB *mydvb);


int mydvb_start_program (MYDVB *mydvb, MYDVB_PROGRAM *program);
void mydvb_stop_program (MYDVB *mydvb, MYDVB_PROGRAM *program);
int mydvb_start_stream (MYDVB *mydvb, MYDVB_STREAM *stream);
void mydvb_stop_stream (MYDVB_STREAM *stream);

/* ------------------------------------------- */
/* Main loop functions */
/* ------------------------------------------- */
void mydvb_main_loop (MYDVB *mydvb);

void mydvb_end_main_loop (MYDVB *mydvb);

int mydvb_ready (MYDVB *mydvb);

int mydvb_eit_ready (MYDVB *mydvb, int prog_num);


/* ------------------------------------------- */
/* Listeners and events */
/* ------------------------------------------- */

int  mydvb_register_listener 		(MYDVB *mydvb, int type, void (*callback) (MYDVB *, MYDVB_EVENT *, void *), void *data);
int  mydvb_register_ext_listener	(MYDVB *mydvb, int fd, void (*callback) (MYDVB *, MYDVB_EVENT *, void *), void *data);
void mydvb_remove_listener 			(MYDVB *mydvb, int id);

void mydvb_event_stop_propagation (MYDVB *mydvb);

/* ------------------------------------------- */
/* Information from dvb data */
/* ------------------------------------------- */
int mydvb_is_program_valid (MYDVB_PROGRAM *program);

MYDVB_PROGRAM *mydvb_get_program_by_number (MYDVB *mydvb, int number);
MYDVB_STREAM *mydvb_get_stream_by_number (MYDVB_PROGRAM *program, int number);

mydvb_text *mydvb_get_service_name (MYDVB *mydvb, int number);
mydvb_text *mydvb_get_provider_name (MYDVB *mydvb, int number);

int mydvb_get_len_audio_stream (MYDVB_PROGRAM *program);
int mydvb_get_len_video_stream (MYDVB_PROGRAM *program);
int mydvb_get_len_txt_stream (MYDVB_PROGRAM *program);

unsigned char mydvb_stream_get_component_tag (MYDVB_STREAM *stream);

mydvb_service_description *mydvb_get_service_description (MYDVB *mydvb, int service_number);

#endif
