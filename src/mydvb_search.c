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

#include <string.h>

#include "mydvb.h"
#include "mydvb_tune.h"

#include "info_dvb.h"

#include "events.h"

#include "descriptor.h"

#include "dynarray.h"

/* ----------------------------------- */

void __mydvb_search_finish (MYDVB_ENGINE *engine, TUNER_SCAN_INFO *scan_info, mydvb_search_status_t status) {

	int i;
	int n;
	MYDVB_LISTENER *listener = NULL;
	MYDVB_LISTENER *aux;

	MYDVB_TUNE *tune = scan_info->tune;

	MYDVB_EVENT event;


	/* remove all search listeners */
	n = dyn_array_get_size (engine->listeners);
	i = 0;
	while (i < n) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (engine->listeners, i);

		if (listener->type == MYDVB_EVENT_EXT_FD || listener->type == MYDVB_SEARCH_EVENT) {

			i++;

		} else {

			dyn_array_remove (engine->listeners, i);

			n --;
		}
	}

	/* restore everything !!!  */
	n = dyn_array_get_size (scan_info->listeners);
	for (i = 0; i < n ; i++) {
		listener = (MYDVB_LISTENER *) dyn_array_get_data (scan_info->listeners, i);

		aux = dyn_array_add_empty (engine->listeners);

		aux->id = listener->id;
		aux->type = listener->type;
		aux->callback = listener->callback;
		aux->data = listener->data;
	}

	scan_info->status = status;

	/* and fire a scanning complete event */

	event.type = MYDVB_SEARCH_EVENT;
	event.search.status = status;
	event.search.info_dvb = scan_info->dvb_info;

	mydvb_notify_event (engine, &event);

	/* stop event propagation */
	mydvb_event_stop_propagation (engine);

	mydvb_tuner_release (engine, tune);

	scan_info->tune = NULL;

	/* free all, except de dvb_info */

	scan_info->dvb_info = NULL;

	dyn_array_free (scan_info->listeners);
	scan_info->listeners = NULL;


	free (scan_info->scan);
	scan_info->scan = NULL;

	free (scan_info);
}

/* scan callback manager */

void __mydvb_search_ready_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	TUNER_SCAN_INFO *scan_info = (TUNER_SCAN_INFO *) data;

	INFO_DVB *dvb_info = scan_info->dvb_info;

	MYDVB_TUNER_SCAN_CTRL *scan = scan_info->scan;

	MYDVB_TUNE *tune = scan_info->tune;

	MYDVB *mydvb = tune->mydvb;

	INFO_CHANNEL *ichannel = NULL;
	INFO_PROGRAM *iprogram = NULL;
	INFO_STREAM *istream = NULL;


	MYDVB_PROGRAM *program;
	MYDVB_STREAM *stream = NULL;
	mydvb_service_description *sd = NULL;
	mydvb_descriptor *descriptor = NULL;

	int i, j;
	int len, len2;

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: Program is ready");

	// ok nuevo canal
	ichannel = info_channel_new();
	ichannel->type = tune->type;
	ichannel->n = tune->channel;

	ichannel->p.frequency = tune->p.frequency;
	ichannel->p.inversion = tune->p.inversion;
	ichannel->p.u.ofdm.bandwidth = tune->p.u.ofdm.bandwidth;
	ichannel->p.u.ofdm.code_rate_HP = tune->p.u.ofdm.code_rate_HP;
	ichannel->p.u.ofdm.code_rate_LP = tune->p.u.ofdm.code_rate_LP;
	ichannel->p.u.ofdm.constellation = tune->p.u.ofdm.constellation;
	ichannel->p.u.ofdm.transmission_mode = tune->p.u.ofdm.transmission_mode;
	ichannel->p.u.ofdm.guard_interval = tune->p.u.ofdm.guard_interval;
	ichannel->p.u.ofdm.hierarchy_information = tune->p.u.ofdm.hierarchy_information;

	len = dyn_array_get_size (mydvb->pat->programs);

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: num programs found: %d", len);

	for (i=0; i < len; i++) {
		program = dyn_array_get_data (mydvb->pat->programs, i);

		iprogram = info_program_new ();
		iprogram->n = i;
		iprogram->number = program->number;
		iprogram->pid = program->pmt_pid;

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: program %d: number %d, PMT pid %d", i, program->number, program->pmt_pid);


		/* find provider name and service name from the SDT */
		len2 = dyn_array_get_size (mydvb->sdt->service_description);
		for (j=0; j < len2; j++) {

			sd = dyn_array_get_data (mydvb->sdt->service_description, j);

			if (sd == NULL) {
				continue;
			}

			iprogram->scrambled = sd->scram;

			if (sd->number == program->number) {
				descriptor = mydvb_descriptor_find (sd->descriptors, 72);
				if (descriptor != NULL) {

					mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: descriptor 72 found");

					if (descriptor->service.provider_name != NULL) {
						iprogram->provider = mydvb_text_dup (descriptor->service.provider_name);
					} else {
						iprogram->provider = NULL;
					}
					if (descriptor->service.service_name != NULL) {
						iprogram->service = mydvb_text_dup (descriptor->service.service_name);
					} else {
						iprogram->service = NULL;
					}
					iprogram->type = descriptor->service.service_type;


					char *utf8_p = mydvb_text_to_utf8 (iprogram->provider);
					char *utf8_s = mydvb_text_to_utf8 (iprogram->service);

					mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: found provider %s, service %s"
							, utf8_p==NULL ? '\0' : utf8_p
							, utf8_s==NULL ? '\0' : utf8_s
							);

					if (utf8_p) free (utf8_p);
					if (utf8_s) free (utf8_s);


				}
				break;
			}
		}

		/* find streams */
		len2 = dyn_array_get_size (program->streams);

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: num of streams found: %d", len2);


		for (j=0; j < len2; j++) {

			stream = dyn_array_get_data (program->streams, j);

			if (stream==NULL) {
				continue;
			}

			istream = info_stream_new ();
			istream->n = j;
			istream->type = stream->type;
			istream->pid = stream->pid;

			info_program_add (iprogram, istream);

			mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: stream added %d: typer %d, pid %d", j, stream->type, stream->pid);

		}

		iprogram->user_number = dvb_info->user_number++;
		info_channel_add (ichannel, iprogram);
	}

	info_dvb_add (dvb_info, ichannel);

	/* free decoder resources */
	mydvb_stop_decoder (engine, tune);

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: end decoder");


	/* check search cancellation */
	if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
		__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_CANCELLED);
		return;
	}

	/* check search stopped */
	if (scan_info->status == MYDVB_SEARCH_STOPPED) {
		__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_STOPPED);
		return;
	}

    /* and startup next tuning */

	if (mydvb_tune_scan_next (tune, scan)) {

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: next scan %d", scan->scan.t.f);

		mydvb_tune_scan (tune, scan);

	} else {

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: completed");

		/* no more scanning: finish */
		__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_COMPLETED);

	}


}

void __mydvb_search_tune_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	TUNER_SCAN_INFO *scan_info = (TUNER_SCAN_INFO *) data;

	MYDVB_TUNE *tune = scan_info->tune;

	MYDVB_EVENT_TUNER *tune_event = &event->tuner;

	/* check search cancellation */
	if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
		__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_CANCELLED);
		return;
	}

	/* check search stopped */
	if (scan_info->status == MYDVB_SEARCH_STOPPED) {
		__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_STOPPED);
		return;
	}

	if (tune_event->status == TUNER_STATUS_LOCK) {

		/* and start decoding */

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan locked at %d: Starting decoding channel %d", tune->p.frequency, tune->channel);

		mydvb_start_decoder (engine, tune);


    } else if (tune_event->status == TUNER_STATUS_TRYING) {

    	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan trying at %d", tune->p.frequency);

	} else if (tune_event->status == TUNER_STATUS_FAILED) {

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan failed");

		if (mydvb_tune_scan_next (tune, scan_info->scan)) {

			mydvb_tune_scan (tune, scan_info->scan);

		} else {
			/* no more scanning: finish */
			__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_COMPLETED);
		}
	}
}

void __mydvb_search_timeout_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	TUNER_SCAN_INFO *scan_info = (TUNER_SCAN_INFO *) data;

	MYDVB_TUNE *tune = scan_info->tune;

	MYDVB_TUNER_SCAN_CTRL *scan = scan_info->scan;

	if (tune->status == TUNER_STATUS_LOCK && event->timeout.count - tune->start_ms >= 5000) { // sometimes there is a tune lock without dvb data

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: end decoder, more than 5 seconds waiting data");

		/* free decoder resources */
		mydvb_stop_decoder (engine, tune);

		/* check search cancellation */
		if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
			__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_CANCELLED);
			return;
		}

		/* check search stopped */
		if (scan_info->status == MYDVB_SEARCH_STOPPED) {
			__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_STOPPED);
			return;
		}

		/* and startup next tuning */

		if (mydvb_tune_scan_next (tune, scan)) {

			mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: next scan %d", scan->scan.t.f);

			mydvb_tune_scan (tune, scan);

		} else {

			mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: completed");

			/* no more scanning: finish */
			__mydvb_search_finish (engine, scan_info, MYDVB_SEARCH_COMPLETED);
		}

	}

}

/**
 *
 */
TUNER_SCAN_INFO *mydvb_tuner_search (MYDVB_ENGINE *engine, MYDVB_TUNER_SCAN_PARAMETER *tuner_param) {

	int i;
	int n;

	MYDVB_LISTENER *listener = NULL;
	MYDVB_LISTENER *aux = NULL;

	TUNER_SCAN_INFO *scan_info;

	MYDVB_TUNE *tune;

	MYDVB_EVENT event;

	/* check if there is an available tuner */
	tune = mydvb_get_available_tuner (engine, tuner_param->type);
	if (tune == NULL) {
		return NULL;
	} else {
		tune->references ++;
	}

	scan_info = (TUNER_SCAN_INFO *) malloc (sizeof (TUNER_SCAN_INFO));

	scan_info->status 	= MYDVB_SEARCH_SEARCHING;
	scan_info->scan 	= (MYDVB_TUNER_SCAN_CTRL *) malloc (sizeof(MYDVB_TUNER_SCAN_CTRL));
	scan_info->dvb_info = info_dvb_new ();
	scan_info->tune 	= tune;


	/* save current listeners */
	scan_info->listeners = dyn_array_new (sizeof (MYDVB_LISTENER));

	n = dyn_array_get_size (engine->listeners);
	i = 0;
	while (i < n) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (engine->listeners, i);

		if (listener->type == MYDVB_EVENT_EXT_FD || listener->type == MYDVB_SEARCH_EVENT) {

			i++;

		} else {

			aux = (MYDVB_LISTENER *) dyn_array_add_empty (scan_info->listeners);
			aux->id = listener->id;
			aux->type = listener->type;
			aux->callback = listener->callback;
			aux->data = listener->data;

			dyn_array_remove (engine->listeners, i);

			n --;
		}
	}

	/* register a specific tune listener */
	mydvb_register_internal_listener (engine,
			MYDVB_TUNER_EVENT,
					__mydvb_search_tune_callback, scan_info);

	/* register a specific ready listener */
	mydvb_register_internal_listener (engine,
					MYDVB_EVENT_READY,
					__mydvb_search_ready_callback, scan_info);

	/* register a specific timeout listener */
	mydvb_register_internal_listener (engine,
			MYDVB_TIMEOUT_EVENT,
			__mydvb_search_timeout_callback, scan_info);

	mydvb_event_stop_propagation (engine);

	/* now init the tune parameters */
	mydvb_tune_scan_init (tune, scan_info->scan, tuner_param);

	mydvb_tune_open (tune);

	/* notify listeners about trying tuning */
	event.type = MYDVB_TUNER_EVENT;
	event.tuner.status = TUNER_STATUS_TRYING;

	mydvb_notify_event (engine, &event);


	/* add the tune filedescriptor to the poll system */
	mydvb_add_poll (engine, tune->fd);

	/* and start scan */
	mydvb_tune_scan (tune, scan_info->scan);

	return scan_info;

}


