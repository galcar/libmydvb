#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>

#include "channels.h"

#define ERROR(x...)                                                     \
        do {                                                            \
                fprintf(stderr, "ERROR: ");                             \
                fprintf(stderr, x);                                     \
                fprintf (stderr, "\n");                                 \
        } while (0)

#define PERROR(x...)                                                    \
        do {                                                            \
                fprintf(stderr, "ERROR: ");                             \
                fprintf(stderr, x);                                     \
                fprintf (stderr, " (%s)\n", strerror(errno));		\
        } while (0)


typedef struct {
	char *name;
	int value;
} Param;

static const Param inversion_list [] = {
	{ "INVERSION_OFF", INVERSION_OFF },
	{ "INVERSION_ON", INVERSION_ON },
	{ "INVERSION_AUTO", INVERSION_AUTO }
};

static const Param bw_list [] = {
	{ "BANDWIDTH_6_MHZ", BANDWIDTH_6_MHZ },
	{ "BANDWIDTH_7_MHZ", BANDWIDTH_7_MHZ },
	{ "BANDWIDTH_8_MHZ", BANDWIDTH_8_MHZ }
};

static const Param fec_list [] = {
	{ "FEC_1_2", FEC_1_2 },
	{ "FEC_2_3", FEC_2_3 },
	{ "FEC_3_4", FEC_3_4 },
	{ "FEC_4_5", FEC_4_5 },
	{ "FEC_5_6", FEC_5_6 },
	{ "FEC_6_7", FEC_6_7 },
	{ "FEC_7_8", FEC_7_8 },
	{ "FEC_8_9", FEC_8_9 },
	{ "FEC_AUTO", FEC_AUTO },
	{ "FEC_NONE", FEC_NONE }
};

static const Param guard_list [] = {
	{"GUARD_INTERVAL_1_16", GUARD_INTERVAL_1_16},
	{"GUARD_INTERVAL_1_32", GUARD_INTERVAL_1_32},
	{"GUARD_INTERVAL_1_4", GUARD_INTERVAL_1_4},
	{"GUARD_INTERVAL_1_8", GUARD_INTERVAL_1_8}
};

static const Param hierarchy_list [] = {
	{ "HIERARCHY_1", HIERARCHY_1 },
	{ "HIERARCHY_2", HIERARCHY_2 },
	{ "HIERARCHY_4", HIERARCHY_4 },
	{ "HIERARCHY_NONE", HIERARCHY_NONE }
};

static const Param constellation_list [] = {
	{ "QPSK", QPSK },
	{ "QAM_128", QAM_128 },
	{ "QAM_16", QAM_16 },
	{ "QAM_256", QAM_256 },
	{ "QAM_32", QAM_32 },
	{ "QAM_64", QAM_64 }
};

static const Param transmissionmode_list [] = {
	{ "TRANSMISSION_MODE_2K", TRANSMISSION_MODE_2K },
	{ "TRANSMISSION_MODE_8K", TRANSMISSION_MODE_8K },
};

#define LIST_SIZE(x) sizeof(x)/sizeof(Param)


static
int parse_param (int fd, const Param * plist, int list_size, int *param)
{
	char c;
	int character = 0;
	int index = 0;

	while (1) {
		if (read(fd, &c, 1) < 1)
			return -1;	/*  EOF? */

		if ((c == ':' || c == '\n')
		    && plist->name[character] == '\0')
			break;

		while (toupper(c) != plist->name[character]) {
			index++;
			plist++;
			if (index >= list_size)	 /*  parse error, no valid */
				return -2;	 /*  parameter name found  */
		}

		character++;
	}

	*param = plist->value;

	return 0;
}


static
int parse_int(int fd, int *val)
{
	char number[11];	/* 2^32 needs 10 digits... */
	int character = 0;

	while (1) {
		if (read(fd, &number[character], 1) < 1)
			return -1;	/*  EOF? */

		if (number[character] == ':' || number[character] == '\n') {
			number[character] = '\0';
			break;
		}

		if (!isdigit(number[character]))
			return -2;	/*  parse error, not a digit... */

		character++;

		if (character > 10)	/*  overflow, number too big */
			return -3;	/*  to fit in 32 bit */
	};

	*val = strtol(number, NULL, 10);

	return 0;
}


static
int find_channel(int fd, const char *channel)
{
	int character = 0;

	while (1) {
		char c;

		if (read(fd, &c, 1) < 1)
			return -1;	/*  EOF! */

		if (c == ':' && channel[character] == '\0')
			break;

		if (toupper(c) == toupper(channel[character]))
			character++;
		else
			character = 0;
	};

	return 0;
}

static
int find_channel_number(int fd, int channel_number)
{
	int character = 0;
	char channel[10];
	
	sprintf (channel, "%d", channel_number);

	while (1) {
		char c;

		if (read(fd, &c, 1) < 1)
			return -1;	/*  EOF! */

		if (c == ':' && channel[character] == '\0')
			break;

		if (toupper(c) == toupper(channel[character]))
			character++;
		else
			character = 0;
	};

	return 0;
}

static
int try_parse_int(int fd, int *val, const char *pname)
{
	int err;

	err = parse_int(fd, val);

	if (err)
		ERROR("error while parsing %s (%s)", pname,
		      err == -1 ? "end of file" :
		      err == -2 ? "not a number" : "number too big");

	return err;
}


static
int try_parse_param(int fd, const Param * plist, int list_size, int *param,
		    const char *pname)
{
	int err;

	err = parse_param(fd, plist, list_size, param);

	if (err)
		ERROR("error while parsing %s (%s)", pname,
		      err == -1 ? "end of file" : "syntax error");

	return err;
}


int parse_favourite_channel(const char *fname, const char *channel,
	  struct dvb_frontend_parameters *frontend, int *vpid, int *apid) {
	int fd;
	int err;

	if ((fd = open(fname, O_RDONLY | O_NONBLOCK)) < 0) {
		PERROR ("could not open file '%s'", fname);
		perror ("");
		return -1;
	}

	if (find_channel(fd, channel) < 0) {
		ERROR("could not find channel '%s' in channel list", channel);
		return -2;
	}

	if ((err = try_parse_int(fd, &frontend->frequency, "frequency")))
		return -3;

	if ((err = try_parse_param(fd,
				   inversion_list, LIST_SIZE(inversion_list),
				   (int *) &frontend->inversion,
				   "inversion")))
		return -4;

	if ((err = try_parse_param(fd, bw_list, LIST_SIZE(bw_list),
				   (int *) &frontend->u.ofdm.bandwidth,
				   "bandwidth")))
		return -5;

	if ((err = try_parse_param(fd, fec_list, LIST_SIZE(fec_list),
				   (int *) &frontend->u.ofdm.code_rate_HP,
				   "code_rate_HP")))
		return -6;

	if ((err = try_parse_param(fd, fec_list, LIST_SIZE(fec_list),
				   (int *) &frontend->u.ofdm.code_rate_LP,
				   "code_rate_LP")))
		return -7;

	if ((err = try_parse_param(fd, constellation_list,
				   LIST_SIZE(constellation_list),
				   (int *) &frontend->u.ofdm.constellation,
				   "constellation")))
		return -8;

	if ((err = try_parse_param(fd, transmissionmode_list,
				   LIST_SIZE(transmissionmode_list),
				   (int *) &frontend->u.ofdm.
				   transmission_mode,
				   "transmission_mode")))
		return -9;

	if ((err = try_parse_param(fd, guard_list, LIST_SIZE(guard_list),
				   (int *) &frontend->u.ofdm.
				   guard_interval, "guard_interval")))
		return -10;

	if ((err = try_parse_param(fd, hierarchy_list,
				   LIST_SIZE(hierarchy_list),
				   (int *) &frontend->u.ofdm.
				   hierarchy_information,
				   "hierarchy_information")))
		return -11;

	if ((err = try_parse_int(fd, vpid, "Video PID")))
		return -12;

	if ((err = try_parse_int(fd, apid, "Audio PID")))
		return -13;

	close(fd);

	return 0;
}

int parse_channel(const char *fname, int channel_number,
	  struct dvb_frontend_parameters *frontend) {
	int fd;
	int err;

	if ((fd = open(fname, O_RDONLY | O_NONBLOCK)) < 0) {
		PERROR ("could not open file '%s'", fname);
		perror ("");
		return -1;
	}

	if (find_channel_number(fd, channel_number) < 0) {
		ERROR("could not find channel '%d' in channel list", channel_number);
		return -2;
	}

	if ((err = try_parse_int(fd, &frontend->frequency, "frequency")))
		return -3;

	if ((err = try_parse_param(fd,
				   inversion_list, LIST_SIZE(inversion_list),
				   (int *) &frontend->inversion,
				   "inversion")))
		return -4;

	if ((err = try_parse_param(fd, bw_list, LIST_SIZE(bw_list),
				   (int *) &frontend->u.ofdm.bandwidth,
				   "bandwidth")))
		return -5;

	if ((err = try_parse_param(fd, fec_list, LIST_SIZE(fec_list),
				   (int *) &frontend->u.ofdm.code_rate_HP,
				   "code_rate_HP")))
		return -6;

	if ((err = try_parse_param(fd, fec_list, LIST_SIZE(fec_list),
				   (int *) &frontend->u.ofdm.code_rate_LP,
				   "code_rate_LP")))
		return -7;

	if ((err = try_parse_param(fd, constellation_list,
				   LIST_SIZE(constellation_list),
				   (int *) &frontend->u.ofdm.constellation,
				   "constellation")))
		return -8;

	if ((err = try_parse_param(fd, transmissionmode_list,
				   LIST_SIZE(transmissionmode_list),
				   (int *) &frontend->u.ofdm.
				   transmission_mode,
				   "transmission_mode")))
		return -9;

	if ((err = try_parse_param(fd, guard_list, LIST_SIZE(guard_list),
				   (int *) &frontend->u.ofdm.
				   guard_interval, "guard_interval")))
		return -10;

	if ((err = try_parse_param(fd, hierarchy_list,
				   LIST_SIZE(hierarchy_list),
				   (int *) &frontend->u.ofdm.
				   hierarchy_information,
				   "hierarchy_information")))
		return -11;

	close(fd);

	return 0;
}

int parse_list_channel(const char *fname, void (*list_callback) (char *c, void *d), void *data) {
	int fd;
	int err;
	char s[1024];
	int nl = 1, k;

	if ((fd = open(fname, O_RDONLY | O_NONBLOCK)) < 0) {
		PERROR ("could not open file '%s'", fname);
		perror ("");
		return -1;
	}

	while (1) {
		char c;

		if (read(fd, &c, 1) < 1)
			return -1;	/*  EOF! */

		if (nl==1) {
			if (c == ':') {
				s[k] = '\0';
				k = 0;
				nl = 0;
				// method call
				list_callback (s, data);
			} else if (c=='\n') {
				nl = 1;
				k  = 0;
			} else {
				s[k] = c;
				k++;
			}
		} else if (c=='\n') {
			nl = 1;
			k  = 0;
		} 
	};

	close(fd);

	return 0;
}

