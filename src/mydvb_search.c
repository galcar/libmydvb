#include <stdlib.h>

#include <string.h>

#include "mydvb.h"
#include "mydvb_tune.h"

#include "info_dvb.h"

#include "events.h"

#include "descriptor.h"

#include "dynarray.h"

/* ----------------------------------- */

void __mydvb_search_finish (MYDVB *mydvb, TUNE_SCAN_INFO *scan_info, mydvb_search_status_t status) {

	int i;
	int n;
	MYDVB_LISTENER *listener = NULL;
	MYDVB_LISTENER *aux;

	MYDVB_EVENT event;

	/* remove all search listeners */
	n = dyn_array_get_size (mydvb->listeners);
	i = 0;
	while (i < n) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (mydvb->listeners, i);

		if (listener->type == MYDVB_EVENT_EXT_FD) {

			i++;

		} else {

			dyn_array_remove (mydvb->listeners, i);

			n --;
		}
	}

	/* restore everything !!!  */
	n = dyn_array_get_size (scan_info->listeners);
	for (i = 0; i < n ; i++) {
		listener = (MYDVB_LISTENER *) dyn_array_get_data (scan_info->listeners, i);

		aux = dyn_array_add_empty (mydvb->listeners);

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

	mydvb_notify_event (mydvb, &event);

	/* stop event propagation */
	mydvb->event_propagation = 0;

	/* free all, except de dvb_info */

	scan_info->dvb_info = NULL;

	dyn_array_free (scan_info->listeners);
	scan_info->listeners = NULL;


	free (scan_info->scan);
	scan_info->scan = NULL;

	free (scan_info);
}

/* scan callback manager */

void __mydvb_search_ready_callback (MYDVB *mydvb, MYDVB_EVENT *event, void *data) {

	TUNE_SCAN_INFO *scan_info = (TUNE_SCAN_INFO *) data;

	INFO_DVB *dvb_info = scan_info->dvb_info;

	MYDVB_TUNE_SCAN_CTRL *scan = scan_info->scan;

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
	ichannel->n = mydvb_tune_get_channel (mydvb->mytune.p.frequency);

	ichannel->p.frequency = mydvb->mytune.p.frequency;
	ichannel->p.inversion = mydvb->mytune.p.inversion;
	ichannel->p.u.ofdm.bandwidth = mydvb->mytune.p.u.ofdm.bandwidth;
	ichannel->p.u.ofdm.code_rate_HP = mydvb->mytune.p.u.ofdm.code_rate_HP;
	ichannel->p.u.ofdm.code_rate_LP = mydvb->mytune.p.u.ofdm.code_rate_LP;
	ichannel->p.u.ofdm.constellation = mydvb->mytune.p.u.ofdm.constellation;
	ichannel->p.u.ofdm.transmission_mode = mydvb->mytune.p.u.ofdm.transmission_mode;
	ichannel->p.u.ofdm.guard_interval = mydvb->mytune.p.u.ofdm.guard_interval;
	ichannel->p.u.ofdm.hierarchy_information = mydvb->mytune.p.u.ofdm.hierarchy_information;

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
	mydvb_end_decoder (mydvb);

	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: end decoder");


	/* check search cancellation */
	if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
		__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_CANCELLED);
		return;
	}

	/* check search stopped */
	if (scan_info->status == MYDVB_SEARCH_STOPPED) {
		__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_STOPPED);
		return;
	}

    /* and startup next tuning */

	if (mydvb_tune_scan_next (&mydvb->mytune, scan)) {

		mydvb_tune_scan (&mydvb->mytune, scan);

	} else {

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan completed");

		/* no more scanning: finish */
		__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_COMPLETED);

	}


}

void __mydvb_search_tune_callback (MYDVB *mydvb, MYDVB_EVENT *event, void *data) {

	TUNE_SCAN_INFO *scan_info = (TUNE_SCAN_INFO *) data;

	MYDVB_EVENT_TUNE *tune_event = &event->tune;

	/* check search cancellation */
	if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
		__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_CANCELLED);
		return;
	}

	/* check search stopped */
	if (scan_info->status == MYDVB_SEARCH_STOPPED) {
		__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_STOPPED);
		return;
	}

	if (tune_event->status == TUNE_STATUS_LOCK) {

		/* and start decoding */

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: Starting decoding");

		mydvb_start_decoder (mydvb);


    } else if (tune_event->status == TUNE_STATUS_TRYING) {

    	mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: trying");

	} else if (tune_event->status == TUNE_STATUS_FAILED) {

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: failed");

		if (mydvb_tune_scan_next (&mydvb->mytune, scan_info->scan)) {

			mydvb_tune_scan (&mydvb->mytune, scan_info->scan);

		} else {
			/* no more scanning: finish */
			__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_COMPLETED);
		}
	}
}

void __mydvb_search_timeout_callback (MYDVB *mydvb, MYDVB_EVENT *event, void *data) {

	TUNE_SCAN_INFO *scan_info = (TUNE_SCAN_INFO *) data;

	MYDVB_TUNE_SCAN_CTRL *scan = scan_info->scan;

	if (event->timeout.count >= 3) { // sometimes there is a tune lock without dvb data

		mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "Tune scan: end decoder, more than 3 seconds waiting data");

		/* free decoder resources */
		mydvb_end_decoder (mydvb);

		/* check search cancellation */
		if (scan_info->status == MYDVB_SEARCH_CANCELLED) {
			__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_CANCELLED);
			return;
		}

		/* check search stopped */
		if (scan_info->status == MYDVB_SEARCH_STOPPED) {
			__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_STOPPED);
			return;
		}

		/* and startup next tuning */

		if (mydvb_tune_scan_next (&mydvb->mytune, scan)) {

			mydvb_tune_scan (&mydvb->mytune, scan);

		} else {

			/* no more scanning: finish */
			__mydvb_search_finish (mydvb, scan_info, MYDVB_SEARCH_COMPLETED);
		}

	}

}

/**
 *
 */
TUNE_SCAN_INFO *mydvb_tune_search (MYDVB *mydvb, MYDVB_TUNE_SCAN_PARAMETER *tune_param) {

	int i;
	int n;

	MYDVB_LISTENER *listener = NULL;
	MYDVB_LISTENER *aux = NULL;

	TUNE_SCAN_INFO *scan_info;

	scan_info = (TUNE_SCAN_INFO *) malloc (sizeof (TUNE_SCAN_INFO));

	scan_info->status = MYDVB_SEARCH_SEARCHING;
	scan_info->scan = (MYDVB_TUNE_SCAN_CTRL *) malloc (sizeof(MYDVB_TUNE_SCAN_CTRL));
	scan_info->dvb_info = info_dvb_new ();


	/* save current listeners */
	scan_info->listeners = dyn_array_new (sizeof (MYDVB_LISTENER));

	n = dyn_array_get_size (mydvb->listeners);
	i = 0;
	while (i < n) {

		listener = (MYDVB_LISTENER *) dyn_array_get_data (mydvb->listeners, i);

		if (listener->type == MYDVB_EVENT_EXT_FD) {

			i++;

		} else {

			aux = (MYDVB_LISTENER *) dyn_array_add_empty (scan_info->listeners);
			aux->id = listener->id;
			aux->type = listener->type;
			aux->callback = listener->callback;
			aux->data = listener->data;

			dyn_array_remove (mydvb->listeners, i);

			n --;
		}
	}

	/* register a specific tune listener */
	mydvb_register_listener (mydvb,
					MYDVB_TUNE_EVENT,
					__mydvb_search_tune_callback, scan_info);

	/* register a specific ready listener */
	mydvb_register_listener (mydvb,
					MYDVB_EVENT_READY,
					__mydvb_search_ready_callback, scan_info);

	/* register a specific timeout listener */
	mydvb_register_listener (mydvb,
			MYDVB_TIMEOUT_EVENT,
			__mydvb_search_timeout_callback, scan_info);

	mydvb->event_propagation = 0;

	/* now init the tune parameters */
	mydvb_tune_scan_init (&mydvb->mytune, scan_info->scan, tune_param);

	/* and start scan */
	mydvb_tune_scan (&mydvb->mytune, scan_info->scan);

	return scan_info;

}


