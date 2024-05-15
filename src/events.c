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
#include <stdlib.h>

#include <dynarray.h>

#include "events.h"

/* ------------------------------------------- */
/* ------------ listeners and events --------- */
/* ------------------------------------------- */


int __mydvb_register_listener (MYDVB_ENGINE *engine, mydvb_event_type_t type, mydvb_event_scope_t scope, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data) {

	MYDVB_LISTENER *listener = NULL;

	if (engine==NULL || callback == NULL) {
		return -1;
	}

	listener = (MYDVB_LISTENER *) dyn_array_add_empty (engine->listeners);
	listener->id = engine->listener_cnt ++;
	listener->type = type;
	listener->scope = scope;
	listener->callback = callback;
	listener->data = data;

	return listener->id;

}


/**
 * add a new listener
 */
int mydvb_register_listener (MYDVB_ENGINE *engine, int type, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data) {

	return __mydvb_register_listener (engine, type, MYDVB_EVENT_SCOPE_USER, callback, data);

}

int mydvb_register_internal_listener (MYDVB_ENGINE *engine, int type, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data) {

	return __mydvb_register_listener (engine, type, MYDVB_EVENT_SCOPE_ENGINE, callback, data);

}

/**
 * add a new external filedescriptor listener
 */
int mydvb_register_ext_listener (MYDVB_ENGINE *engine, int fd, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data) {

	MYDVB_LISTENER *listener = NULL;

	if (engine==NULL || callback == NULL) {
		return -1;
	}

	listener = (MYDVB_LISTENER *) dyn_array_add_empty (engine->listeners);
	listener->id = engine->listener_cnt ++;
	listener->type = MYDVB_EVENT_EXT_FD;
	listener->scope = MYDVB_EVENT_SCOPE_USER;
	listener->callback = callback;
	listener->data = data;
	listener->extra.fd = fd;

	mydvb_add_poll (engine, fd);

	return listener->id;

}

void mydvb_remove_listener (MYDVB_ENGINE *engine, int id) {
	int j;
	int len;
	MYDVB_LISTENER *aux;

	if (engine==NULL) {
		return;
	}

	len = dyn_array_get_size (engine->listeners);

	for (j=0; j < len; j++) {

		aux = (MYDVB_LISTENER *) dyn_array_get_data (engine->listeners, j);

		if (aux->id == id) {

			if (aux->type == MYDVB_EVENT_EXT_FD) {

				mydvb_remove_poll (engine, aux->extra.fd);

			}

			dyn_array_remove (engine->listeners, j);

			engine->event_propagation = 0;

			break;
		}
	}

}

/**
 * stop the propagation of the event across the listeners list
 */
void mydvb_event_stop_propagation (MYDVB_ENGINE *engine) {

	if (engine == NULL) {
		return;
	}

	engine->event_propagation = 0;
}

void mydvb_notify_event (MYDVB_ENGINE *engine, MYDVB_EVENT *event) {

	int i;
	MYDVB_LISTENER *listener;

	int match;

	if (engine == NULL || event == NULL) {
		return;
	}

	engine->event_propagation = 1;

	i = 0;

	while (i < dyn_array_get_size (engine->listeners)) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (engine->listeners, i);

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

			listener->callback (engine, event, listener->data);

			if (engine->event_propagation == 0) {
				break;
			}

		}

		i++;
	}

}
