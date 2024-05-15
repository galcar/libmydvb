/*
 * test01.c
 *
 *  Created on: 28 abr 2024
 *  Author: g.alcaraz
 */

#include "mydvb.h"

#include "mydvb_tune.h"

#include "dynarray.h"


void search_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	INFO_DVB *dvb = event->search.info_dvb;
	char *provider=NULL, *service = NULL;

	if (dvb == NULL) {
		return;
	}

	info_dvb_save ("test.cfg", dvb);

	if (event->search.status == MYDVB_SEARCH_COMPLETED ||
		event->search.status == MYDVB_SEARCH_STOPPED) {

		for (int i = 0; i < dvb->channels_len; i++) {

			for (int j = 0; j <dvb->channels[i]->programs_len; j++) {

				INFO_PROGRAM *program = dvb->channels[i]->programs[j];

				provider = mydvb_text_to_utf8 (program->provider);
				service = mydvb_text_to_utf8 (program->service);

				printf ("Channel %d; %d-%d-%d: %s-%s\n"
						,dvb->channels[i]->n
						,program->n, program->number
						,program->user_number
						,provider
						,service);

				if (provider) {
					free (provider);
					provider = NULL;
				}
				if (service) {
					free (service);
					service = NULL;
				}
			}

		}

		for (int i = 0; i < dyn_array_get_size(engine->channels); i++) {
			MYDVB *mydvb = *((MYDVB **) dyn_array_get_data(engine->channels,i));
			printf ("MYDVB: %d-%d, %d\n"
					,mydvb->type
					,mydvb->channel
					,mydvb->ready
					);
			mydvb_show_pat (mydvb->pat);
			mydvb_show_sdt (mydvb->sdt);
		}
	}

	info_dvb_free (dvb);

	mydvb_end_main_loop (engine);

}

int main (int argc, char **argsv) {

	TUNER_SCAN_INFO *scan_info;

	MYDVB_ENGINE engine;

	MYDVB_TUNER_SCAN_PARAMETER scan;

	scan.search_mode = MYDVB_TUNER_SEARCH_FULL;
	scan.type = DVB_TYPE_DVBT;

	// uhf channels 21-48
	scan.param.t.f_step  = 8000000; // 8MHz bandwidth
	scan.param.t.f_start = 474000000; // + 8000000*45; // 474000000;
	scan.param.t.f_end   = 690000000; //scan.param.t.f_start + 3*scan.param.t.f_step; // 690000000; 858000000;

	mydvb_log_open (MYDVB_LOG_LEVEL_ERROR, NULL);

	mydvb_engine_init (&engine);

	// list devices found
	for (int i = 0; i < dyn_array_get_size (engine.tuners); i++) {
		MYDVB_TUNE *tune = *((MYDVB_TUNE **) dyn_array_get_data (engine.tuners, i));

		printf ("Engine tuner %d, %s at (%d-%d-%d) of type %d. Status %d\n"
				, i
				, tune->name
				, tune->device, tune->adapter, tune->demuxer
				, tune->type
				, tune->status
				);
	}

	printf ("Starting scan\n");

	mydvb_register_listener (&engine, MYDVB_SEARCH_EVENT, search_callback, NULL);

	scan_info = mydvb_tuner_search (&engine, &scan);
	if (scan_info==NULL) {
		printf ("No tuner search\n");
		return 0;
	}

	printf ("Scanning\n");

	printf ("entering main loop\n");

	mydvb_main_loop (&engine);

	printf ("end main loop\n");

	mydvb_engine_end (&engine);

	printf ("engine closed\n");

	mydvb_log_close ();

	return 0;
}


