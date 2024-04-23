#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <mydvb.h>

typedef struct {
	int			id;			/* unique listener identifier */

	int 		type;

	void		(*callback) (MYDVB *mydvb, MYDVB_EVENT *event, void *data);

	void		*data;

	union {
						int fd; /* file descriptor of MYDVB_EVENT_EXT_FD */
	} extra;

} MYDVB_LISTENER;

void mydvb_notify_event (MYDVB *mydvb, MYDVB_EVENT *event);


#endif
