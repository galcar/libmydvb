
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

#include "tune.h"

#include "logger.h"

#include "util.h"


/* terrestrial tune */
static int __tune_frontend_t (MYDVB_TUNE *mytune) {

	struct dvb_frontend_parameters *p = &mytune->p;

	mytune->status = TUNE_STATUS_TRYING;

	/*
	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tunning at %d with inversion=%d bandwidth=%d code_rate_hp=%d code_rate_lp=%d constellation=%d transmission=%d guard=%d hierarchy=%d",
			p->frequency,
			p->inversion,
			p->u.ofdm.bandwidth,
			p->u.ofdm.code_rate_HP,
			p->u.ofdm.code_rate_LP,
			p->u.ofdm.constellation,
			p->u.ofdm.transmission_mode,
			p->u.ofdm.guard_interval,
			p->u.ofdm.hierarchy_information
			);
	*/

	mytune->start_ms = current_timestamp_ms ();

	/* try tunning */
	if (ioctl (mytune->fd, FE_SET_FRONTEND, p)==-1) {
		return -1; // error general, abortar
	}



	return 0;
}

/* sat tune */
static int __tune_frontend_s (MYDVB_TUNE *mytune) {

	struct dvb_frontend_parameters *p = &mytune->p;

	mytune->status = TUNE_STATUS_TRYING;

	mytune->start_ms = current_timestamp_ms ();

	/* try tunning */
	if (ioctl (mytune->fd, FE_SET_FRONTEND, p)==-1) {
		return -1; // error general, abortar
	}

	return 0;
}



static int __next_tune_param (const mydvb_tune_param *list, int current) {

	int n = current + 1;

	if (list[n].name == NULL) {
		n = -1;
	}

	return n;

}

static int __mydvb_find_next_valid_bandwidth (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (bw_list, current);

	while (n != -1) {

		if (tune->caps & bw_list[n].can) {
			return n;
		}

		n = __next_tune_param (bw_list, n);

	}

	return -1;
}

static int __mydvb_find_next_valid_constellation (MYDVB_TUNE *tune, int current) {

	if (current == 1) { /* if current is qam auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (constellation_list, current);

	while (n != -1) {

		if (tune->caps & constellation_list[n].can) {
			return n;
		}

		n = __next_tune_param (constellation_list, n);

	}

	return -1;
}

static int __mydvb_find_next_valid_transmission_mode (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (transmissionmode_list, current);

	while (n != -1) {

		if (tune->caps & transmissionmode_list[n].can) {
			return n;
		}

		n = __next_tune_param (transmissionmode_list, n);

	}

	return -1;
}

static int __mydvb_find_next_valid_guard (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (guard_list, current);

	while (n != -1) {

		if (tune->caps & guard_list[n].can) {
			return n;
		}

		n = __next_tune_param (guard_list, n);

	}

	return -1;
}

static int __mydvb_find_next_valid_hierarchy (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (hierarchy_list, current);

	while (n != -1) {

		if (tune->caps & hierarchy_list[n].can) {
			return n;
		}

		n = __next_tune_param (hierarchy_list, n);

	}

	return -1;
}



static int __mydvb_find_next_valid_fec (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (fec_list, current);

	while (n != -1) {

		if (tune->caps & fec_list[n].can) {
			return n;
		}

		n = __next_tune_param (fec_list, n);

	}

	return -1;
}

static int __mydvb_find_next_valid_inversion (MYDVB_TUNE *tune, int current) {

	if (current == 0) { /* if current is auto, no more options available */
		return -1;
	}

	int n = __next_tune_param (inversion_list, current);

	while (n != -1) {

		if (tune->caps & inversion_list[n].can) {
			return n;
		}

		n = __next_tune_param (inversion_list, n);

	}

	return -1;
}

/**
 *
 */
static void __mydvb_tune_s_scan_init (MYDVB_TUNE *tune, struct __mydvb_tune_s_scan_ctrl *scan, mydvb_tune_scan_param_s *param) {

	if (scan == NULL || param == NULL) {
		return;
	}

	scan->scan_param.f_start 	= param->f_start;
	scan->scan_param.f_end  	= param->f_end;
	scan->scan_param.f_step  	= param->f_step;
	scan->scan_param.u			= param->u;

	scan->fi 	= __mydvb_find_next_valid_fec 				(tune, -1);
	scan->i 	= __mydvb_find_next_valid_inversion			(tune, -1);
	scan->sr = scan->scan_param.u;

	scan->f = scan->scan_param.f_start;

}

/**
 *
 */
static void __mydvb_tune_t_scan_init (MYDVB_TUNE *tune, struct __mydvb_tune_t_scan_ctrl *scan, mydvb_tune_scan_param_t *param) {

	if (scan == NULL || param == NULL) {
		return;
	}

	scan->scan_param.f_start 	= param->f_start;
	scan->scan_param.f_end  	= param->f_end;
	scan->scan_param.f_step  	= param->f_step;

	scan->h 	= __mydvb_find_next_valid_hierarchy 		(tune, -1);
	scan->gi 	= __mydvb_find_next_valid_guard 			(tune, -1);
	scan->tm 	= __mydvb_find_next_valid_transmission_mode (tune, -1);
	scan->c 	= __mydvb_find_next_valid_constellation 	(tune, -1);
	scan->bw 	= __mydvb_find_next_valid_bandwidth 		(tune, -1);

	scan->cr_hp = __mydvb_find_next_valid_fec 				(tune, -1);
	scan->cr_lp = __mydvb_find_next_valid_fec 				(tune, -1);

	scan->i 	= __mydvb_find_next_valid_inversion			(tune, -1);

	scan->f = scan->scan_param.f_start;

}

/**
 *
 */
static int __mydvb_tune_s_scan_next (MYDVB_TUNE *tune, struct __mydvb_tune_s_scan_ctrl *scan) {

	if (scan == NULL) {
		return -1;
	}

	if (tune->status == TUNE_STATUS_LOCK) {
		scan->fi 	= __mydvb_find_next_valid_fec 				(tune, -1);
		scan->i 	= __mydvb_find_next_valid_inversion			(tune, -1);
		scan->sr 	= scan->scan_param.u;

		/* frequency selector */
		scan->f += scan->scan_param.f_step;
		if (scan->f > scan->scan_param.f_end) {
			/* fin del escaneo */
			return 0;
		}

		return 1;
	}


	/* fec inner selector */
	scan->fi = __mydvb_find_next_valid_fec (tune, scan->fi);

	if (scan->fi == -1) {

		scan->fi = __mydvb_find_next_valid_fec (tune, -1);

		/* inversion selector */
		scan->i = __mydvb_find_next_valid_inversion (tune, scan->i);

		if (scan->i == -1) {

			scan->i = __mydvb_find_next_valid_inversion (tune, -1);

			/* frequency selector */
			scan->f += scan->scan_param.f_step;
			if (scan->f > scan->scan_param.f_end) {
				/* fin del escaneo */
				return 0;
			}
		}
	}


	return 1;
}

/**
 *
 */
static int __mydvb_tune_t_scan_next (MYDVB_TUNE *tune, struct __mydvb_tune_t_scan_ctrl *scan) {

	if (scan == NULL) {
		return -1;
	}

	if (tune->status == TUNE_STATUS_LOCK) {
		scan->h 	= __mydvb_find_next_valid_hierarchy 		(tune, -1);
		scan->gi 	= __mydvb_find_next_valid_guard 			(tune, -1);
		scan->tm 	= __mydvb_find_next_valid_transmission_mode (tune, -1);
		scan->c 	= __mydvb_find_next_valid_constellation 	(tune, -1);
		scan->bw 	= __mydvb_find_next_valid_bandwidth 		(tune, -1);

		scan->cr_hp = __mydvb_find_next_valid_fec 				(tune, -1);
		scan->cr_lp = __mydvb_find_next_valid_fec 				(tune, -1);

		scan->i 	= __mydvb_find_next_valid_inversion			(tune, -1);

		/* frequency selector */
		scan->f += scan->scan_param.f_step;
		if (scan->f > scan->scan_param.f_end) {
			/* fin del escaneo */
			return 0;
		}

		return 1;
	}

	/* hierarchy selector */
	scan->h = __mydvb_find_next_valid_hierarchy (tune, scan->h);

	if (scan->h == -1) {
		scan->h = __mydvb_find_next_valid_hierarchy (tune, -1);

		/* guard selector */
		scan->gi = __mydvb_find_next_valid_guard (tune, scan->gi);

		if (scan->gi == -1) {
			scan->gi = __mydvb_find_next_valid_guard (tune, -1);

			/* transmission mode selector */
			scan->tm = __mydvb_find_next_valid_transmission_mode (tune, scan->tm);

			if (scan->tm == -1) {
				scan->tm = __mydvb_find_next_valid_transmission_mode (tune, -1);

				/* constellation (modulation) selector */
				scan->c = __mydvb_find_next_valid_constellation (tune, scan->c);

				if (scan->c == -1) { // loop complete

					scan->c = __mydvb_find_next_valid_constellation (tune, -1);

					/* bandwidth selector */
					scan->bw = __mydvb_find_next_valid_bandwidth (tune, scan->bw);

					if (scan->bw == -1) {

						scan->bw = __mydvb_find_next_valid_bandwidth (tune, -1);

						/* code rate hp selector */

						scan->cr_hp = __mydvb_find_next_valid_fec (tune, scan->cr_hp);

						if (scan->cr_hp == -1) {

							scan->cr_hp = __mydvb_find_next_valid_fec (tune, -1);

							/* code rate lp selector */
							scan->cr_lp = __mydvb_find_next_valid_fec (tune, scan->cr_lp);

							if (scan->cr_lp == -1) {

								scan->cr_lp = __mydvb_find_next_valid_fec (tune, -1);

								/* inversion selector */
								scan->i = __mydvb_find_next_valid_inversion (tune, scan->i);

								if (scan->i == -1) {

									scan->i = __mydvb_find_next_valid_inversion (tune, -1);

									/* frequency selector */
									scan->f += scan->scan_param.f_step;
									if (scan->f > scan->scan_param.f_end) {
										/* fin del escaneo */
										return 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}


int mydvb_tune_init (MYDVB_TUNE *mytune, int adapter, int device) {
	char s[64];
	struct dvb_frontend_info info;


	if (mytune==NULL) {
		return -1;
	}

	sprintf (s, "/dev/dvb/adapter%d/frontend%d", adapter, device);
	mytune->fd = open (s, O_RDWR);

	if (mytune->fd!=-1) {

		struct dtv_property p[] = {
			{ .cmd = DTV_DELIVERY_SYSTEM }
		};

		struct dtv_properties dtv_props = {
			.num = 1,
			.props = p
		};

		if (ioctl(mytune->fd, FE_GET_PROPERTY, &dtv_props)==0) {

			if (p[0].u.data == SYS_DVBT || p[0].u.data == SYS_DVBT2) {

				mytune->type = DVB_TYPE_DVBT;

			} else if (p[0].u.data == SYS_DVBS || p[0].u.data == SYS_DVBS2) {

				mytune->type = DVB_TYPE_DVBS;

			} else if (p[0].u.data == SYS_DVBC_ANNEX_A || p[0].u.data == SYS_DVBC_ANNEX_B) {

				mytune->type = DVB_TYPE_DVBC;
			}

			printf ("Delivery system: %d\n",
				p[0].u.data);
		}

		/* ok, search for capabilities */
		if (ioctl (mytune->fd, FE_GET_INFO, &info)==0) {
			sprintf (mytune->name, info.name);

			/* info.type is deprecated
			if (info.type == FE_QPSK) {

				mytune->type = DVB_TYPE_DVBS;

			} else if (info.type == FE_QAM) {

				mytune->type = DVB_TYPE_DVBC;

			} else if (info.type == FE_OFDM) {

				mytune->type = DVB_TYPE_DVBT;

			}
			*/

			mytune->caps = info.caps;

			mytune->freq_min = info.frequency_min;
			mytune->freq_max = info.frequency_max;
			mytune->freq_step_min = info.frequency_stepsize;
		}
		mytune->adapter = adapter;
		mytune->device  = device;
		mytune->status  = TUNE_STATUS_NOOP;

		//mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Found tuner %d-%d type %d, name %s", adapter, device, mytune->type, info.name);

		return 0;
	}
	return -1;
}

void mydvb_tune_end (MYDVB_TUNE *mytune) {

	if (mytune->fd != -1) {

		close (mytune->fd);

		mytune->fd = -1;

	}
}

int mydvb_tune_get_info (MYDVB_TUNE *mytune) {

	unsigned int ss  = 0;
	unsigned int ber = 0;
	unsigned int snr = 0;

	int aux;

	if (mytune->fd==-1) {
		return -1;
	}

	if (mytune->status != TUNE_STATUS_LOCK) {
		return -1;
	}

	if (ioctl (mytune->fd, FE_READ_SIGNAL_STRENGTH, &ss)==0) {

		mytune->info.signal = (unsigned short) (ss * 100 / 65535);

	} else {

		mytune->info.signal = 0;
	}

	if (ioctl (mytune->fd, FE_READ_BER, &ber)==0) {

		mytune->info.ber = (unsigned int) ber;

	} else {

		mytune->info.ber = 0;;

	}

	if (ioctl (mytune->fd, FE_READ_SNR, &snr)==0) {

		mytune->info.snr = (unsigned short) snr;

	} else {

		mytune->info.snr = 0;

	}

	return 0;
}

int mydvb_tune_is_tuned (MYDVB_TUNE *mytune) {
	return mytune->status == TUNE_STATUS_LOCK;
}



int mydvb_tune_frontend (MYDVB_TUNE *mytune) {

	int r;

	if (mydvb_tune_is_dvbt (mytune)) {

		r = __tune_frontend_t (mytune);

	} else if (mydvb_tune_is_dvbs (mytune)) {

		r = __tune_frontend_s (mytune);

	}

	return r;
}



int mydvb_tune_is_dvbs (MYDVB_TUNE *mytune) {
	if (mytune == NULL) {
		return 0;
	}

	if (mytune->type == DVB_TYPE_DVBS) {
		return 1;
	} else {
		return 0;
	}
}

int mydvb_tune_is_dvbc (MYDVB_TUNE *mytune) {
	if (mytune == NULL) {
		return 0;
	}

	if (mytune->type == DVB_TYPE_DVBC) {
		return 1;
	} else {
		return 0;
	}
}


int mydvb_tune_is_dvbt (MYDVB_TUNE *mytune) {
	if (mytune == NULL) {
		return 0;
	}

	if (mytune->type == DVB_TYPE_DVBT) {
		return 1;
	} else {
		return 0;
	}
}

int mydvb_tune_get_channel (int frequency) {

	int v = (frequency / 1000000 - 306)  / 8;

	return v;

}


char *mydvb_tune_param_search (const mydvb_tune_param *l, int v) {
	int n=0;

	if (l == NULL) {
		return NULL;
	}

	while (l[n].name!=NULL) {
		if (l[n].value==v) {
			return l[n].name;
		}
		n++;
	}

	return NULL;
}



void mydvb_tune_scan (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan) {


	if (mydvb_tune_is_dvbt (tune)) {

		tune->p.u.ofdm.hierarchy_information = hierarchy_list[scan->scan.t.h].value;
		tune->p.u.ofdm.guard_interval = guard_list[scan->scan.t.gi].value;
		tune->p.u.ofdm.transmission_mode = transmissionmode_list[scan->scan.t.tm].value;
		tune->p.u.ofdm.constellation = constellation_list[scan->scan.t.c].value;
		tune->p.u.ofdm.bandwidth = bw_list[scan->scan.t.bw].value;
		tune->p.u.ofdm.code_rate_HP = fec_list[scan->scan.t.cr_hp].value;
		tune->p.u.ofdm.code_rate_LP = fec_list[scan->scan.t.cr_lp].value;
		tune->p.inversion = inversion_list[scan->scan.t.i].value;
		tune->p.frequency = scan->scan.t.f;

		mydvb_tune_frontend (tune);

	} else if (mydvb_tune_is_dvbs (tune)) {

		tune->p.u.qpsk.symbol_rate = scan->scan.s.sr;
		tune->p.u.qpsk.fec_inner = fec_list[scan->scan.s.fi].value;;

		tune->p.inversion = inversion_list[scan->scan.s.i].value;
		tune->p.frequency = scan->scan.s.f;

		mydvb_tune_frontend (tune);
	}
}

void mydvb_tune_scan_init (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan, MYDVB_TUNE_SCAN_PARAMETER *param) {

	if (tune == NULL || scan == NULL || param == NULL) {
		return;
	}

	scan->search_mode = param->search_mode;

	if (mydvb_tune_is_dvbt (tune)) {

		__mydvb_tune_t_scan_init (tune, &scan->scan.t, &param->param.t);

	} else if (mydvb_tune_is_dvbs (tune)) {

		__mydvb_tune_s_scan_init (tune, &scan->scan.s, &param->param.s);

	}
}

int mydvb_tune_scan_next (MYDVB_TUNE *tune, MYDVB_TUNE_SCAN_CTRL *scan) {

	if (tune == NULL || scan == NULL) {
		return 0;
	}

	if (mydvb_tune_is_dvbt (tune)) {

		return __mydvb_tune_t_scan_next (tune, &scan->scan.t);

	} else if (mydvb_tune_is_dvbs (tune)) {

		return __mydvb_tune_s_scan_next (tune, &scan->scan.s);
	}

	return 0;
}
