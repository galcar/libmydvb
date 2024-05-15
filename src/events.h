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
#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <mydvb.h>

typedef enum {
	MYDVB_EVENT_SCOPE_USER,
	MYDVB_EVENT_SCOPE_ENGINE
} mydvb_event_scope_t;

typedef struct {
	int						id;			/* unique listener identifier */

	mydvb_event_type_t 		type;

	mydvb_event_scope_t		scope; /* scope of the listener */

	void					(*callback) (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data);

	void					*data;

	union {
							int fd; /* file descriptor of MYDVB_EVENT_EXT_FD */
	} extra;

} MYDVB_LISTENER;

int mydvb_register_internal_listener (MYDVB_ENGINE *engine, int type, void (*callback) (MYDVB_ENGINE *, MYDVB_EVENT *, void *), void *data);

void mydvb_notify_event (MYDVB_ENGINE *engine, MYDVB_EVENT *event);


#endif
