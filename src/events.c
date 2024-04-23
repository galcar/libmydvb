
#include <stdlib.h>

#include <dynarray.h>

#include "events.h"

/* ------------------------------------------- */
/* ------------ listeners and events --------- */
/* ------------------------------------------- */



int __mydvb_listeners_id = 100;

/**
 * add a new listener
 */
int mydvb_register_listener (MYDVB *mydvb, int type, void (*callback) (MYDVB *, MYDVB_EVENT *, void *), void *data) {

	MYDVB_LISTENER *listener = NULL;

	if (mydvb==NULL || callback == NULL) {
		return -1;
	}

	listener = (MYDVB_LISTENER *) dyn_array_add_empty (mydvb->listeners);
	listener->id = __mydvb_listeners_id;
	listener->type = type;
	listener->callback = callback;
	listener->data = data;

	__mydvb_listeners_id ++;

	return listener->id;

}

/**
 * add a new external filedescriptor listener
 */
int mydvb_register_ext_listener (MYDVB *mydvb, int fd, void (*callback) (MYDVB *, MYDVB_EVENT *, void *), void *data) {

	MYDVB_LISTENER *listener = NULL;

	if (mydvb==NULL || callback == NULL) {
		return -1;
	}

	listener = (MYDVB_LISTENER *) dyn_array_add_empty (mydvb->listeners);
	listener->id = __mydvb_listeners_id;
	listener->type = MYDVB_EVENT_EXT_FD;
	listener->callback = callback;
	listener->data = data;
	listener->extra.fd = fd;

	__mydvb_add_poll (mydvb, fd);

	__mydvb_listeners_id ++;

	return listener->id;

}

void mydvb_remove_listener (MYDVB *mydvb, int id) {
	int j;
	int len;
	MYDVB_LISTENER *aux;

	if (mydvb==NULL) {
		return;
	}

	len = dyn_array_get_size (mydvb->listeners);

	for (j=0; j < len; j++) {

		aux = (MYDVB_LISTENER *) dyn_array_get_data (mydvb->listeners, j);

		if (aux->id == id) {

			if (aux->type == MYDVB_EVENT_EXT_FD) {

				__mydvb_remove_poll (mydvb, aux->extra.fd);

			}

			dyn_array_remove (mydvb->listeners, j);

			mydvb->event_propagation = 0;

			break;
		}
	}

}

/**
 * stop the propagation of the event across the listeners list
 */
void mydvb_event_stop_propagation (MYDVB *mydvb) {

	if (mydvb == NULL) {
		return;
	}

	mydvb->event_propagation = 0;
}

void mydvb_notify_event (MYDVB *mydvb, MYDVB_EVENT *event) {

	int i;
	MYDVB_LISTENER *listener;

	int match;

	if (mydvb == NULL || event == NULL) {
		return;
	}

	mydvb->event_propagation = 1;

	i = 0;

	while (i < dyn_array_get_size (mydvb->listeners)) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (mydvb->listeners, i);

		match = 0;

		if (listener->type == event->type) {

			if (event->type == MYDVB_EVENT_EXT_FD) {
				if (event->external.fd == listener->extra.fd) {
					match = 1;
				}

			} else {
				match = 1;
			}
		}

		if (match) {

			listener->callback (mydvb, event, listener->data);

			if (mydvb->event_propagation == 0) {
				break;
			}

		}

		i++;
	}

}
