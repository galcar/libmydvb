#ifndef __MYDVB_TUNE__
#define __MYDVB_TUNE__

#include <linux/dvb/frontend.h>

typedef enum {
	DVB_TYPE_DVBS,
	DVB_TYPE_DVBT,
	DVB_TYPE_DVBC

} mydvb_tune_type_t;

typedef enum {

	MYDVB_TUNE_SEARCH_FULL,
	MYDVB_TUNE_SEARCH_PARTIAL

} mydvb_tune_search_mode_t;

typedef struct {
	char *name;
	int value;
	int can;
} mydvb_tune_param;


static const mydvb_tune_param bw_list [] = {
	{ "BANDWIDTH_AUTO",  BANDWIDTH_AUTO,  FE_CAN_BANDWIDTH_AUTO},
	{ "BANDWIDTH_8_MHZ", BANDWIDTH_8_MHZ, 0xFFFFFFFF },
	{ "BANDWIDTH_7_MHZ", BANDWIDTH_7_MHZ, 0xFFFFFFFF },
	{ "BANDWIDTH_6_MHZ", BANDWIDTH_6_MHZ, 0xFFFFFFFF },
	{ NULL, 0, 0}
};


static const mydvb_tune_param guard_list [] = {
	{"GUARD_INTERVAL_AUTO", GUARD_INTERVAL_AUTO, FE_CAN_GUARD_INTERVAL_AUTO},
	{"GUARD_INTERVAL_1_4", GUARD_INTERVAL_1_4, 0xFFFFFFFF},
	{"GUARD_INTERVAL_1_8", GUARD_INTERVAL_1_8, 0xFFFFFFFF},
	{"GUARD_INTERVAL_1_16", GUARD_INTERVAL_1_16, 0xFFFFFFFF},
	{"GUARD_INTERVAL_1_32", GUARD_INTERVAL_1_32, 0xFFFFFFFF},
	{ NULL, 0, 0}
};

static const mydvb_tune_param hierarchy_list [] = {
	{ "HIERARCHY_AUTO", HIERARCHY_AUTO, FE_CAN_HIERARCHY_AUTO },
	{ "HIERARCHY_1", HIERARCHY_1, 0xFFFFFFFF },
	{ "HIERARCHY_2", HIERARCHY_2, 0xFFFFFFFF },
	{ "HIERARCHY_4", HIERARCHY_4, 0xFFFFFFFF },
	{ "HIERARCHY_NONE", HIERARCHY_NONE, 0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tune_param constellation_list [] = {
	{ "QPSK", QPSK, FE_CAN_QPSK },
	{ "QAM_AUTO", QAM_AUTO, FE_CAN_QAM_AUTO},
	{ "QAM_16", QAM_16, FE_CAN_QAM_16 },
	{ "QAM_32", QAM_32, FE_CAN_QAM_32 },
	{ "QAM_64", QAM_64, FE_CAN_QAM_64 },
	{ "QAM_128", QAM_128, FE_CAN_QAM_128 },
	{ "QAM_256", QAM_256, FE_CAN_QAM_256 },
	{ NULL, 0, 0}
};

static const mydvb_tune_param transmissionmode_list [] = {
	{ "TRANSMISSION_MODE_AUTO", TRANSMISSION_MODE_AUTO, FE_CAN_TRANSMISSION_MODE_AUTO },
	{ "TRANSMISSION_MODE_2K", TRANSMISSION_MODE_2K, 0xFFFFFFFF },
	{ "TRANSMISSION_MODE_8K", TRANSMISSION_MODE_8K, 0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tune_param fec_list [] = {
	{ "FEC_AUTO", FEC_AUTO,	FE_CAN_FEC_AUTO },
	{ "FEC_1_2", FEC_1_2,	FE_CAN_FEC_1_2 },
	{ "FEC_2_3", FEC_2_3,	FE_CAN_FEC_2_3 },
	{ "FEC_3_4", FEC_3_4,	FE_CAN_FEC_3_4 },
	{ "FEC_4_5", FEC_4_5,	FE_CAN_FEC_4_5 },
	{ "FEC_5_6", FEC_5_6,	FE_CAN_FEC_5_6 },
	{ "FEC_6_7", FEC_6_7,	FE_CAN_FEC_6_7 },
	{ "FEC_7_8", FEC_7_8,	FE_CAN_FEC_7_8 },
	{ "FEC_8_9", FEC_8_9,	FE_CAN_FEC_8_9 },
	{ "FEC_NONE", FEC_NONE,	0xFFFFFFFF },
	{ NULL, 0, 0}
};

static const mydvb_tune_param inversion_list [] = {
	{ "INVERSION_AUTO", INVERSION_AUTO, FE_CAN_INVERSION_AUTO },
	{ "INVERSION_OFF", INVERSION_OFF,   0xFFFFFFFF },
	{ "INVERSION_ON", INVERSION_ON, 0xFFFFFFFF },
	{ NULL, 0, 0}
};

/**
 * status of the tune device
 */
typedef enum tune_status {
	TUNE_STATUS_NOOP   = 0,	/* no operation, initial status */
	TUNE_STATUS_TRYING = 1, /* tune request, trying to lock... */
	TUNE_STATUS_LOCK   = 2, /* after a tune request and tune locks ok */
	TUNE_STATUS_FAILED = 3  /* after a tune request and tune fails to lock */
} TUNE_STATUS_T;


typedef struct {

	unsigned short 	signal;		/* signal strength */
	unsigned int 	ber;		/* ber */
	unsigned short 	snr;		/* signal/noise relation */

} MYDVB_TUNE_INFO;

typedef struct {
    int 				adapter;		/* adapter number */
	int 				device;			/* device frontend for this tuner */
	int					demuxer;		/* device demuxer for this tuner */
	
	char       			name[128];		/* adapter name */
	mydvb_tune_type_t 	type;			/* type */
    fe_caps_t 			caps;			/* capabilities */
    int					freq_min;		/* minimal frequency of tuning */
    int					freq_max;		/* max frequency of tuning */
    int					freq_step_min;	/* min step of frequency tuning */

	struct dvb_frontend_parameters p;	// current synth information

	int fd;								// file descriptor for tunning device
	int fd_dvr;							// file descriptor of the dvr device

	MYDVB_TUNE_INFO info;				// info about tunning

	TUNE_STATUS_T status;				/* status of the tunning */

	long long start_ms;					/* time in ms of a tunning trying */

} MYDVB_TUNE;


/* ------------------------------------------------------------- */
/* tune scanning data and structures */
/* ------------------------------------------------------------- */

typedef struct {
	unsigned int f_start;
	unsigned int f_end;
	unsigned int f_step;

} mydvb_tune_scan_param_t;

typedef struct {
	unsigned int f_start;
	unsigned int f_end;
	unsigned int f_step;
	int u;

} mydvb_tune_scan_param_s;

typedef struct {
	int u;
} mydvb_tune_scan_param_c;

typedef union {
	mydvb_tune_scan_param_t t;
	mydvb_tune_scan_param_s s;
	mydvb_tune_scan_param_c c;
} mydvb_tune_scan_param;

typedef struct {

	mydvb_tune_search_mode_t search_mode;

	mydvb_tune_scan_param param;

} MYDVB_TUNE_SCAN_PARAMETER;


struct __mydvb_tune_t_scan_ctrl {

	mydvb_tune_scan_param_t	scan_param;

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

struct __mydvb_tune_s_scan_ctrl {

	mydvb_tune_scan_param_s scan_param;

	unsigned int f; /* frequency */

	int i; 	/* inversion */

	int sr;	/* symbol rate */
	int fi; /* fec inner */

};

typedef union {

	struct __mydvb_tune_t_scan_ctrl t;
	struct __mydvb_tune_s_scan_ctrl s;

} __mydvb_tune_scan_ctrl;

typedef struct {

	mydvb_tune_search_mode_t 	search_mode;

	__mydvb_tune_scan_ctrl 		scan;

} MYDVB_TUNE_SCAN_CTRL;

int mydvb_tune_init (MYDVB_TUNE *mytune, int adapter, int device);
void mydvb_tune_end (MYDVB_TUNE *mytune);
int mydvb_tune_get_info (MYDVB_TUNE *mytune);
int mydvb_tune_is_tuned (MYDVB_TUNE *mytune);

int mydvb_tune_frontend (MYDVB_TUNE *mytune);

int mydvb_tune_is_dvbs (MYDVB_TUNE *mytune);
int mydvb_tune_is_dvbc (MYDVB_TUNE *mytune);
int mydvb_tune_is_dvbt (MYDVB_TUNE *mytune);

int mydvb_tune_get_channel (int frequency);


/* -------------------------------------------------- */
/* tune scan functions
/* -------------------------------------------------- */

void mydvb_tune_scan (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan);
void mydvb_tune_scan_init (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan, MYDVB_TUNE_SCAN_PARAMETER *param);
int mydvb_tune_scan_next (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan);

#endif
