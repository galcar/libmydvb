/*
 * test02.c
 *
 *  Created on: 28 abr 2024
 *  Author: g.alcaraz
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "mydvb.h"

#include "mydvb_tune.h"
#include "mydvb_descriptors.h"

#include "dynarray.h"

typedef struct {

	INFO_DVB 		*info_dvb;

	INFO_PROGRAM 	*selection;

} APP;

void stdin_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	APP *app = (APP *) data;

	INFO_DVB *info_dvb = app->info_dvb;

	char cmd[256];

	if (event->external.fd != 0) {
		return;
	}

	int n = read (0, cmd, 255);
	cmd[n-1] = '\0';

	if (strcmp (cmd, "exit")==0) {

		mydvb_end_main_loop (engine);

	} else if (strcmp (cmd, "epg")==0) {

		for (int i = 0; i < dyn_array_get_size (engine->channels); i++) {
			MYDVB *mydvb = *((MYDVB **) dyn_array_get_data (engine->channels,i));

			for (int j = 0; j < dyn_array_get_size(mydvb->pat->programs); j++) {
				MYDVB_PROGRAM *program = (MYDVB_PROGRAM *) dyn_array_get_data (mydvb->pat->programs, j);


				mydvb_text *t1 = mydvb_get_provider_name (mydvb, j);
				mydvb_text *t2 = mydvb_get_service_name (mydvb, j);

				char *s1 = mydvb_text_to_utf8 (t1);
				char *s2 = mydvb_text_to_utf8 (t2);

				for (int k = 0; k < 16 ; k++) {
					 MYDVB_EIT *eit = program->eit_sched[k];

					 if (eit == NULL) {
						 continue;
					 }

					 for (int n = 0; n < dyn_array_get_size (eit->entries); n++) {
						 MYDVB_EIT_ENTRY *eit_entry = (MYDVB_EIT_ENTRY *) dyn_array_get_data (eit->entries, n);

						 mydvb_short_event_descriptor *sed = (mydvb_short_event_descriptor * )
						 					mydvb_descriptor_find (eit_entry->descriptors, 0x4d);
						 if (sed != NULL) {
							char *text = mydvb_text_to_utf8 (sed->event_name);

							printf ("Event %s-%s: %s\n", s1, s2, text);
							free (text);
						 }
					 }
				}

				if (s1) free (s1);
				if (s2) free (s2);
			}
		}

	} else if (strncmp (cmd, "tune in", 7)==0) {

		MYDVB_TUNE *tuner = mydvb_get_dvbt (engine);

		if (tuner == NULL) {

			printf ("No tuners\n");

		} else {

			if (tuner->status == TUNER_STATUS_NOOP) {

				int chn, prog;

				sscanf (cmd, "tune in %d/%d", &chn, &prog);

				printf ("Selected %d - %d \n", chn, prog);

				mydvb_log_set_level (MYDVB_LOG_LEVEL_DEBUG);

				app->selection = info_dvb_find (info_dvb, tuner->type, chn,prog);

				//mydvb_tuner_channel_by_number (engine, info_dvb, chn);
				mydvb_tuner_channel (engine, app->selection->channel);

			} else {
				mydvb_stop_decoder (engine, tuner);

				mydvb_tuner_release (engine, tuner);

				printf ("Tuner closed\n");
			}

		}

	}
}

void stream_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "Receiving streams %d", event->stream.length);

	//mydvb_end_main_loop (engine);
	//mydvb_stop_decoder (engine);

	//mydvb_tuner_channel_by_number (engine, info_dvb, 22);
}

void program_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "Program ready, starting services");

	MYDVB_PROGRAM *program = event->program.program;

	// we want all streams, so start all of them
	int len = dyn_array_get_size (program->streams);
	for (int i=0; i < len; i++) {
		MYDVB_STREAM  *stream = dyn_array_get_data (program->streams, i);
		mydvb_start_stream (event->program.tuner, stream);
	}

	mydvb_log_set_level (MYDVB_LOG_LEVEL_NONE);
}

void ready_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	APP *app = (APP *) data;

	MYDVB_PROGRAM *program;

	mydvb_log (MYDVB_LOG_LEVEL_INFO, "Channel ready, starting program");

	program = mydvb_get_program_in_channel (event->ready.tuner->mydvb, app->selection->n);
	mydvb_start_program (engine, event->ready.tuner, program);
}

void tune_callback (MYDVB_ENGINE *engine, MYDVB_EVENT *event, void *data) {

	mydvb_tuner_status_t t_status = event->tuner.status;
	mydvb_tuner_type_t s;

	if (t_status == TUNER_STATUS_LOCK) {

		printf ("Locked\n");

		mydvb_start_decoder (engine, event->tuner.tuner);

	} else if (t_status == TUNER_STATUS_FAILED) {
		printf ("Failed to lock\n");

	} else {
		printf ("Unknown status %d\n", t_status);
	}

}

int main (int argc, char **argsv) {

	APP app;

	MYDVB_ENGINE engine;

	mydvb_log_open (MYDVB_LOG_LEVEL_DEBUG, NULL);

	app.info_dvb = info_dvb_new ();

	info_dvb_load ("test.cfg", app.info_dvb); // test.cfg generated with test01


	mydvb_engine_init (&engine);

	//mydvb_tuner_channel_by_number (&engine, info_dvb, 22);

	mydvb_register_listener (&engine, MYDVB_TUNER_EVENT, tune_callback, &app);
	mydvb_register_listener (&engine, MYDVB_EVENT_READY, ready_callback, &app);
	mydvb_register_listener (&engine, MYDVB_EVENT_PROGRAM, program_callback, &app);
	mydvb_register_listener (&engine, MYDVB_EVENT_STREAM, stream_callback, &app);

	mydvb_register_ext_listener (&engine, 0, stdin_callback, &app);


	printf ("entering main loop\n");

	mydvb_main_loop (&engine);

	printf ("end main loop\n");

	mydvb_engine_end (&engine);

	printf ("engine closed\n");

	info_dvb_free (app.info_dvb);

	mydvb_log_close ();

	return 0;
}


