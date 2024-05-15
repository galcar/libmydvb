#ifndef __MYDVB_H__
#define __MYDVB_H__

#include <poll.h>

typedef struct _mydvb_engine MYDVB_ENGINE;

typedef struct _mydvb MYDVB;


#include <dynarray.h>

#include <mydvb_info_dvb.h>

#include <mydvb_log.h>

#include <mydvb_tune.h>
#include <mydvb_tables.h>

#include <mydvb_text.h>


#define MYDVB_RUNNING_STATUS_LEN 	8
#define MYDVB_STREAM_TYPE_LEN	66
#define MYDVB_SERVICE_TYPE_LEN	256


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
	MYDVB_TUNER_EVENT,
	MYDVB_SEARCH_EVENT,
	MYDVB_TIMEOUT_EVENT
} mydvb_event_type_t;



typedef struct {
	mydvb_event_type_t 			type;

	MYDVB_TUNE					*tuner;

} MYDVB_EVENT_DATA_READY;

typedef struct {
	mydvb_event_type_t 			type;

	MYDVB_PROGRAM 				*program;

	MYDVB_TUNE					*tuner;

} MYDVB_EVENT_PROGRAM_READY;

typedef struct {
	mydvb_event_type_t 			type;

	int 						fd;

} MYDVB_EVENT_EXTERNAL_FD;

typedef struct {

	mydvb_event_type_t 			type;

	unsigned char				*buffer;

	int							length;

	MYDVB_TUNE					*tuner;

} MYDVB_EVENT_DATA_STREAM;

typedef struct {
	mydvb_event_type_t 			type;

	mydvb_tuner_status_t		status;

	MYDVB_TUNE					*tuner;

} MYDVB_EVENT_TUNER;

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

	MYDVB_EVENT_TUNER			tuner;

	MYDVB_EVENT_SEARCH			search;

	MYDVB_EVENT_TIMEOUT			timeout;

} MYDVB_EVENT;


struct _mydvb {

	mydvb_tuner_type_t				type;  // cable, dvb-t, dvb-s

	int								channel; // channel number of this decoder

	/* decoding tables */

	MYDVB_PAT 						*pat;		/* the pat table */
	MYDVB_CAT 						*cat;		/* the cat table */
	MYDVB_SDT 						*sdt;		/* the sdt table */
	MYDVB_TDT 						tdt;		/* tdt table */

	DYN_ARRAY						*aits;		/* aits tables */

	int 							ready;		/* all basic elements (pat, sdt and programs) are ready */

};


struct _mydvb_engine {
	struct pollfd 					pfd[64];	/* the poll file descriptor table */
	int 							poll_len;	/* len of pfd */

	/* event management */
	DYN_ARRAY  						*listeners;
	int								listener_cnt;
	int								event_propagation;

	int 							mainloop;	/* main loop in progress */

	DYN_ARRAY						*tuners;

	DYN_ARRAY						*channels; // list of mydvb objects
};



/* -------- tuner scan ---------------- */
typedef struct {
	MYDVB_TUNE					*tune;
	MYDVB_TUNER_SCAN_CTRL 		*scan;
	INFO_DVB 					*dvb_info;

	mydvb_search_status_t		status;

	/* for restore later */
	DYN_ARRAY					*listeners;

} TUNER_SCAN_INFO;

/** constants data table */
char **mydvb_running_status_table ();
char **mydvb_stream_type_table ();
char **mydvb_service_type_table();
char **mydvb_desc_content_table();


/* ------------------------------------------- */
/* dvb devices */
/* ------------------------------------------- */

MYDVB_TUNE *mydvb_get_dvbt (MYDVB_ENGINE *engine);
MYDVB_TUNE *mydvb_get_dvbs (MYDVB_ENGINE *engine);
MYDVB_TUNE *mydvb_get_dvbc (MYDVB_ENGINE *engine);



/* ------------------------------------------- */
/* Tuner functions */
/* ------------------------------------------- */

MYDVB_TUNE *mydvb_get_available_tuner (MYDVB_ENGINE *engine, mydvb_tuner_type_t type);

MYDVB_TUNE *mydvb_tuner_channel (MYDVB_ENGINE *engine, INFO_CHANNEL *ichannel);

MYDVB_TUNE *mydvb_tuner_channel_by_number (MYDVB_ENGINE *engine, INFO_DVB *info_dvb, int channel_number);

void mydvb_tuner_release (MYDVB_ENGINE *engine, MYDVB_TUNE *tuner);

TUNER_SCAN_INFO *mydvb_tuner_search (MYDVB_ENGINE *engine, MYDVB_TUNER_SCAN_PARAMETER *tuner_param);


/* ------------------------------------------- */
/* Decoding functions */
/* ------------------------------------------- */

void mydvb_start_decoder (MYDVB_ENGINE *engine, MYDVB_TUNE *tune);
void mydvb_stop_decoder (MYDVB_ENGINE *engine, MYDVB_TUNE *tune);


int mydvb_start_program (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, MYDVB_PROGRAM *program);
void mydvb_stop_program (MYDVB_ENGINE *engine, MYDVB_TUNE *tune, MYDVB_PROGRAM *program);

int mydvb_start_stream (MYDVB_TUNE *tune, MYDVB_STREAM *stream);
void mydvb_stop_stream (MYDVB_TUNE *tune, MYDVB_STREAM *stream);


int mydvb_init (MYDVB *mydvb);
void mydvb_end (MYDVB *mydvb);

int mydvb_ready (MYDVB *mydvb);

int mydvb_eit_ready (MYDVB *mydvb, int prog_num);


/* ------------------------------------------- */
/* Listeners and events */
/* ------------------------------------------- */

int mydvb_register_listener (MYDVB_ENGINE *engine, int type, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data);
int mydvb_register_ext_listener (MYDVB_ENGINE *engine, int fd, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data);
void mydvb_remove_listener (MYDVB_ENGINE *engine, int id);

void mydvb_event_stop_propagation (MYDVB_ENGINE *engine);


/* ------------------------------------------- */
/* Extract information from dvb data */
/* ------------------------------------------- */

int mydvb_is_program_valid (MYDVB_PROGRAM *program);

MYDVB *mydvb_get_channel (MYDVB_ENGINE *engine, mydvb_tuner_type_t type, int channel);
MYDVB_PROGRAM *mydvb_get_program (MYDVB_ENGINE *engine, mydvb_tuner_type_t type, int channel, int pos);
MYDVB_PROGRAM *mydvb_get_program_in_channel (MYDVB *mydvb, int pos);
MYDVB_STREAM *mydvb_get_stream (MYDVB_PROGRAM *program, int pos);

mydvb_text *mydvb_get_service_name (MYDVB *mydvb, int pos);
mydvb_text *mydvb_get_provider_name (MYDVB *mydvb, int pos);

int mydvb_get_len_audio_stream (MYDVB_PROGRAM *program);
int mydvb_get_len_video_stream (MYDVB_PROGRAM *program);
int mydvb_get_len_txt_stream (MYDVB_PROGRAM *program);

unsigned char mydvb_stream_get_component_tag (MYDVB_STREAM *stream);

mydvb_service_description *mydvb_get_service_description (MYDVB *mydvb, int service_number);


/* ------------------------------------------- */
/* Engine */
/* ------------------------------------------- */

void mydvb_engine_init (MYDVB_ENGINE *engine);

void mydvb_engine_end (MYDVB_ENGINE *engine);

void mydvb_add_poll (MYDVB_ENGINE *engine, int fd);

void mydvb_remove_poll (MYDVB_ENGINE *engine, int fd);

/* ------------------------------------------- */
/* Main loop */
/* ------------------------------------------- */

void mydvb_main_loop (MYDVB_ENGINE *engine);

void mydvb_end_main_loop (MYDVB_ENGINE *engine);

#endif
