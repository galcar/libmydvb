#ifndef __CHANNELS_C__
#define __CHANNELS_C__

#include <linux/dvb/frontend.h>

int parse_favourite_channel(const char *fname, const char *channel,
	  struct dvb_frontend_parameters *frontend, int *vpid, int *apid);

int parse_channel(const char *fname, int channel_number,
	  struct dvb_frontend_parameters *frontend);

#endif
