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
#ifndef __MYDVB_TUNER__
#define __MYDVB_TUNER__

#include <stdint.h>

#include <linux/dvb/frontend.h>

typedef struct _mydvb_tuner MYDVB_TUNE;

/**
 * status of the tuner device
 */
typedef enum {
	TUNER_STATUS_NOOP   = 0, /* no operation, initial status */
	TUNER_STATUS_TRYING = 1, /* tuner request, trying to lock... */
	TUNER_STATUS_LOCK   = 2, /* after a tuner request and tuner locks ok */
	TUNER_STATUS_FAILED = 3  /* after a tuner request and tuner fails to lock */
} mydvb_tuner_status_t;

typedef enum {
	DVB_TYPE_NONE = 0,
	DVB_TYPE_DVBS,
	DVB_TYPE_DVBT,
	DVB_TYPE_DVBC

} mydvb_tuner_type_t;

typedef struct _mydvb_tuner_scan_ctrl MYDVB_TUNER_SCAN_CTRL;
typedef struct _mydvb_tuner_scan_parameter MYDVB_TUNER_SCAN_PARAMETER;

#include <mydvb.h>


typedef enum {

	MYDVB_TUNER_SEARCH_FULL,
	MYDVB_TUNER_SEARCH_PARTIAL

} mydvb_tuner_search_mode_t;

typedef struct {
	char *name;
	int value;
	int can;
} mydvb_tuner_param;


static const mydvb_tuner_param bw_list [] = {
	{ "BANDWIDTH_AUTO",  		BANDWIDTH_AUTO,  		FE_CAN_BANDWIDTH_AUTO},
	{ "BANDWIDTH_8_MHZ", 		BANDWIDTH_8_MHZ, 		0xFFFFFFFF },
	{ "BANDWIDTH_7_MHZ", 		BANDWIDTH_7_MHZ, 		0xFFFFFFFF },
	{ "BANDWIDTH_6_MHZ", 		BANDWIDTH_6_MHZ, 		0xFFFFFFFF },
	{ "BANDWIDTH_1_712_MHZ", 	BANDWIDTH_1_712_MHZ, 	0xFFFFFFFF},
	{ "BANDWIDTH_5_MHZ", 		BANDWIDTH_5_MHZ, 		0xFFFFFFFF},
	{ "BANDWIDTH_10_MHZ", 		BANDWIDTH_10_MHZ, 		0xFFFFFFFF},
	{ NULL, 0, 0}
};


static const mydvb_tuner_param guard_list [] = {
	{"GUARD_INTERVAL_AUTO",		GUARD_INTERVAL_AUTO, 	FE_CAN_GUARD_INTERVAL_AUTO},
	{"GUARD_INTERVAL_1_4", 		GUARD_INTERVAL_1_4,		0xFFFFFFFF},
	{"GUARD_INTERVAL_1_8", 		GUARD_INTERVAL_1_8,		0xFFFFFFFF},
	{"GUARD_INTERVAL_1_16", 	GUARD_INTERVAL_1_16,	0xFFFFFFFF},
	{"GUARD_INTERVAL_1_32", 	GUARD_INTERVAL_1_32,	0xFFFFFFFF},
	{"GUARD_INTERVAL_1_128", 	GUARD_INTERVAL_1_128, 	0xFFFFFFFF},
	{"GUARD_INTERVAL_19_128",	GUARD_INTERVAL_19_128, 	0xFFFFFFFF},
	{"GUARD_INTERVAL_19_256", 	GUARD_INTERVAL_19_256, 	0xFFFFFFFF},
	{"GUARD_INTERVAL_PN420", 	GUARD_INTERVAL_PN420, 	0xFFFFFFFF},
	{"GUARD_INTERVAL_PN595", 	GUARD_INTERVAL_PN595, 	0xFFFFFFFF},
	{"GUARD_INTERVAL_PN945",	GUARD_INTERVAL_PN945, 	0xFFFFFFFF},
	{ NULL, 0, 0}
};

static const mydvb_tuner_param hierarchy_list [] = {
	{ "HIERARCHY_AUTO", HIERARCHY_AUTO, FE_CAN_HIERARCHY_AUTO },
	{ "HIERARCHY_1", 	HIERARCHY_1, 	0xFFFFFFFF },
	{ "HIERARCHY_2",	HIERARCHY_2,	0xFFFFFFFF },
	{ "HIERARCHY_4", 	HIERARCHY_4, 	0xFFFFFFFF },
	{ "HIERARCHY_NONE", HIERARCHY_NONE, 0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tuner_param constellation_list [] = {
	{ "QPSK", 		QPSK, 		FE_CAN_QPSK },
	{ "QAM_AUTO", 	QAM_AUTO, 	FE_CAN_QAM_AUTO},
	{ "QAM_16", 	QAM_16, 	FE_CAN_QAM_16 },
	{ "QAM_32", 	QAM_32, 	FE_CAN_QAM_32 },
	{ "QAM_64", 	QAM_64, 	FE_CAN_QAM_64 },
	{ "QAM_128", 	QAM_128, 	FE_CAN_QAM_128 },
	{ "QAM_256", 	QAM_256, 	FE_CAN_QAM_256 },
	{ "VSB_8", 		VSB_8, 		0xFFFFFFFF },
	{ "VSB_16", 	VSB_16, 	0xFFFFFFFF },
	{ "PSK_8", 		PSK_8, 		0xFFFFFFFF },
	{ "APSK_16", 	APSK_16, 	0xFFFFFFFF },
	{ "APSK_16", 	APSK_16, 	0xFFFFFFFF },
	{ "APSK_32", 	APSK_32, 	0xFFFFFFFF },
	{ "DQPSK", 		DQPSK, 		0xFFFFFFFF },
	{ "QAM_4_NR", 	QAM_4_NR, 	0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tuner_param transmissionmode_list [] = {
	{ "TRANSMISSION_MODE_AUTO", TRANSMISSION_MODE_AUTO, FE_CAN_TRANSMISSION_MODE_AUTO },
	{ "TRANSMISSION_MODE_2K", 	TRANSMISSION_MODE_2K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_8K", 	TRANSMISSION_MODE_8K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_1K", 	TRANSMISSION_MODE_1K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_4K", 	TRANSMISSION_MODE_4K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_16K",	TRANSMISSION_MODE_16K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_32K", 	TRANSMISSION_MODE_32K, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_C1", 	TRANSMISSION_MODE_C1, 	0xFFFFFFFF },
	{ "TRANSMISSION_MODE_C3780",TRANSMISSION_MODE_C3780,0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tuner_param fec_list [] = {
	{ "FEC_AUTO", FEC_AUTO,	FE_CAN_FEC_AUTO },
	{ "FEC_1_2", FEC_1_2,	FE_CAN_FEC_1_2 },
	{ "FEC_2_3", FEC_2_3,	FE_CAN_FEC_2_3 },
	{ "FEC_3_4", FEC_3_4,	FE_CAN_FEC_3_4 },
	{ "FEC_4_5", FEC_4_5,	FE_CAN_FEC_4_5 },
	{ "FEC_5_6", FEC_5_6,	FE_CAN_FEC_5_6 },
	{ "FEC_6_7", FEC_6_7,	FE_CAN_FEC_6_7 },
	{ "FEC_7_8", FEC_7_8,	FE_CAN_FEC_7_8 },
	{ "FEC_8_9", FEC_8_9,	FE_CAN_FEC_8_9 },
	{ "FEC_9_10", FEC_9_10,	0xFFFFFFFF },
	{ "FEC_2_5", FEC_2_5,	0xFFFFFFFF },
	{ "FEC_3_5", FEC_3_5,	0xFFFFFFFF },
	{ "FEC_NONE", FEC_NONE,	0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tuner_param inversion_list [] = {
	{ "INVERSION_AUTO", INVERSION_AUTO, FE_CAN_INVERSION_AUTO },
	{ "INVERSION_OFF", 	INVERSION_OFF,   0xFFFFFFFF },
	{ "INVERSION_ON", 	INVERSION_ON, 0xFFFFFFFF },
	{ NULL, 0, 0}
};




typedef struct {

	double 	signal;		/* signal strength: positive: % value, negative: dB value*/
	double 	ber;		/* ber */
	double 	snr;		/* signal/noise relation: positive: % value, negative: dB value */

} MYDVB_TUNER_INFO;

struct FILE_PID {
	unsigned short 	pid;

	int 			fd;

};

struct _mydvb_tuner {
	// information fields

    int 				adapter;		/* adapter number */
	int 				device;			/* device frontend for this tuner */
	
	char       			name[128];		/* adapter name */
	mydvb_tuner_type_t 	type;			/* type */
    fe_caps_t 			caps;			/* capabilities */
    int					freq_min;		/* minimal frequency of tuning */
    int					freq_max;		/* max frequency of tuning */
    int					freq_step_min;	/* min step of frequency tuning */


    // current state fields

    mydvb_tuner_status_t status;		/* status of the tunning */
    int					references;		/* number of uses of this tuner */

    int 				channel;		// channel number
	struct dvb_frontend_parameters p;	// current synth information
	MYDVB_TUNER_INFO 	info;			// info about tunning

	int 				fd;				// file descriptor for tunning device
	int 				fd_dvr;			// file descriptor of the dvr device

	int					sct_fd_len; 	/* demux sct descriptors len */
	struct FILE_PID		sct_fd[32]; 	/* demux sct descriptors table */

	int					pes_fd_len;		/* demux pes descriptors len */
	struct FILE_PID		pes_fd[32];		/* demux pes descriptors table */

	long long 			start_ms;		/* time in ms of a tunning trying */

	MYDVB				*mydvb;			/* the decoder object of the tunning when it is locked */

	void				*data;			/* user data */
};


/* ------------------------------------------------------------- */
/* tuner scanning data and structures */
/* ------------------------------------------------------------- */

typedef struct {
	unsigned int f_start;
	unsigned int f_end;
	unsigned int f_step;

} mydvb_tuner_scan_param_t;

typedef struct {
	unsigned int f_start;
	unsigned int f_end;
	unsigned int f_step;
	int u;

} mydvb_tuner_scan_param_s;

typedef struct {
	int u;
} mydvb_tuner_scan_param_c;

typedef union {
	mydvb_tuner_scan_param_t t;
	mydvb_tuner_scan_param_s s;
	mydvb_tuner_scan_param_c c;
} mydvb_tuner_scan_param;

struct _mydvb_tuner_scan_parameter {

	mydvb_tuner_search_mode_t search_mode;

	mydvb_tuner_type_t type;

	mydvb_tuner_scan_param param;

};


struct __mydvb_tuner_t_scan_ctrl {

	mydvb_tuner_scan_param_t	scan_param;

	unsigned int f;

	int i; 	/* inversion */

	int bw;
	int tm;
	int c;
	int h;
	int gi;
	int cr_hp;
	int cr_lp;

};

struct __mydvb_tuner_s_scan_ctrl {

	mydvb_tuner_scan_param_s scan_param;

	unsigned int f; /* frequency */

	int i; 	/* inversion */

	int sr;	/* symbol rate */
	int fi; /* fec inner */

};

struct _mydvb_tuner_scan_ctrl {

	mydvb_tuner_search_mode_t 		search_mode;

	union {

		struct __mydvb_tuner_t_scan_ctrl t;
		struct __mydvb_tuner_s_scan_ctrl s;

	} 								scan;

};

char **mydvb_tuner_type_table();
mydvb_tuner_type_t mydvb_tuner_parse_type (char *type);

int mydvb_tune_init (MYDVB_TUNE *mytune, int adapter, int device);

int mydvb_tune_open (MYDVB_TUNE *tune);
void mydvb_tune_close (MYDVB_TUNE *tune);

int mydvb_tune_get_info (MYDVB_TUNE *mytune);
int mydvb_tune_is_tuned (MYDVB_TUNE *mytune);

int mydvb_tune_frontend (MYDVB_TUNE *mytune);

int mydvb_tune_is_dvbs (MYDVB_TUNE *mytune);
int mydvb_tune_is_dvbc (MYDVB_TUNE *mytune);
int mydvb_tune_is_dvbt (MYDVB_TUNE *mytune);

/**
 * get the uhf channel number from frequency in Hz
 */
int mydvb_tune_get_uhf_channel (int frequency);

void mydvb_tuner_set_user_data (MYDVB_TUNE *tuner, void *p);


/* -------------------------------------------------- */
/* tune scan functions
/* -------------------------------------------------- */

void mydvb_tune_scan (MYDVB_TUNE *tune, MYDVB_TUNER_SCAN_CTRL *scan);
void mydvb_tune_scan_init (MYDVB_TUNE *tune, MYDVB_TUNER_SCAN_CTRL *scan, MYDVB_TUNER_SCAN_PARAMETER *param);
int mydvb_tune_scan_next (MYDVB_TUNE *tune, MYDVB_TUNER_SCAN_CTRL *scan);

#endif
