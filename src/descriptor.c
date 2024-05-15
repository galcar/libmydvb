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

#include "descriptor.h"

#include "logger.h"

#include "util.h"

#include "mhp_desc.h"


static char *__mydvb_descriptor_type_table[MYDVB_DESCRIPTOR_TYPE_LEN] = {
		"Reserved",
		"Reserved",
		"video_stream_descriptor",
		"audio_stream_descriptor",
		"hierarchy_descriptor",
		"registration_descriptor",
		"data_stream_alignment_descriptor",
		"target_background_grid_descriptor",
		"Video_window_descriptor",
		"CA_descriptor",
		"ISO_639_language_descriptor",
		"System_clock_descriptor",
		"Multiplex_buffer_utilization_descriptor",
		"Copyright_descriptor",
		"Maximum_bitrate_descriptor",
		"Private_data_indicator_descriptor",
		"Smoothing_buffer_descriptor", // 16
		"STD_descriptor",
		"IBP_descriptor",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"Defined in ISO/IEC 13818-6",
		"MPEG-4_video_descriptor",
		"MPEG-4_audio_descriptor",
		"IOD_descriptor",
		"SL_descriptor",
		"FMC_descriptor",
		"External_ES_ID_descriptor", // 32
		"MuxCode_descriptor",
		"FmxBufferSize_descriptor",
		"MultiplexBuffer_descriptor",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ", // 40
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ", // 48
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved ",
		"network_name_descriptor", // 64
		"service_list_descriptor",
		"stuffing_descriptor",
		"satellite_delivery_system_descriptor",
		"cable_delivery_system_descriptor",
		"reserved",
		"reserved",
		"bouquet_name_descriptor",
		"service_descriptor",
		"country_availability_descriptor",
		"linkage_descriptor",
		"NVOD_reference_descriptor",
		"time_shifted_service_descriptor",
		"short_event_descriptor",
		"extended_event_descriptor",
		"time_shifted_event_descriptor",
		"component_descriptor", // 80
		"mosaic_descriptor",
		"stream_identifier_descriptor",
		"CA_identifier_descriptor",
		"content_descriptor",
		"parental_rating_descriptor",
		"teletext_descriptor",
		"telephone_descriptor",
		"local_time_offset_descriptor",
		"subtitling_descriptor",
		"terrestrial_delivery_system_descriptor",
		"multilingual_network_name_descriptor",
		"multilingual_bouquet_name_descriptor",
		"multilingual_service_name_descriptor",
		"multilingual_component_descriptor",
		"private_data_specifier_descriptor",
		"service_move_descriptor", // 96
		"short_smoothing_buffer_descriptor",
		"frequency_list_descriptor",
		"partial_transport_stream_descriptor",
		"data_broadcast_descriptor",
		"CA_system_descriptor",
		"data_broadcast_id_descriptor",
		"transport_stream_descriptor (TSDT)", //103
		"DSNG_descriptor (TSDT)",
		"PDC_descriptor",
		"AC-3_descriptor",
		"ancillary_data_descriptor",
		"cell_list_descriptor",
		"cell_frequency_link_descriptor",
		"announcement_support_descriptor",
		"application_signalling_descriptor",
		"adaptation_field_data_descriptor", // 112
		"service_identifier_descriptor",
		"service_availability_descriptor ",
		"default_authority_descriptor",
		"related_content_descriptor",
		"TVA_id_descriptor",
		"content_identifier_descriptor",
		"time_slice_fec_identifier_descriptor",
		"ECM_repetition_rate_descriptor",
		"S2_satellite_delivery_system_descriptor",
		"enhanced_AC-3_descriptor",
		"DTS_descriptor",
		"AAC_descriptor",
		"XAIT_location_descriptor",
		"FTA_content_management_descriptor", // 126
		"extension_descriptor"
};

char **mydvb_descriptor_type_table() {
	return __mydvb_descriptor_type_table;
}

mydvb_descriptor *mydvb_descriptor_find (DYN_ARRAY *da, unsigned char tag) {
	int i, len;
	mydvb_descriptor *descriptor = NULL;

	if (da==NULL) {
		return NULL;
	}

	len = dyn_array_get_size (da);
	for (i=0; i < len; i++) {
		descriptor = dyn_array_get_data (da, i);
		if (descriptor->info.type==tag) {
			return descriptor;
		}
	}

	/* not found */

	return NULL;

}




void __parse_network_name_descriptor (const unsigned char *buf, mydvb_network_name_descriptor *nnd) {
	int len;

	len = buf[1];
	buf += 2;

	nnd->name = mydvb_text_create (buf, len);

}

void __free_network_name_descriptor (mydvb_network_name_descriptor *nnd) {

	// mydvb_log (MYDVB_LOG_LEVEL_DEBUG,"Free network name descriptor %d-%d %s\n", nnd->table, nnd->type, nnd->name);

	mydvb_text_free (nnd->name);
	nnd->name = NULL;	
}

void __parse_service_descriptor (const unsigned char *buf, mydvb_service_descriptor *s) {
	unsigned char len;

	s->service_type = buf[2];

	buf += 3;
	len = *buf;
	buf++;

	s->provider_name = mydvb_text_create (buf, len);


	buf += len;
	len = *buf;
	buf++;

	s->service_name = mydvb_text_create (buf, len);

}

void __free_service_descriptor (mydvb_service_descriptor *s) {
	
	mydvb_text_free (s->provider_name);
	s->provider_name = NULL;

	mydvb_text_free (s->service_name);
	s->service_name = NULL;
	
}

void __parse_short_event_descriptor (const unsigned char *buf, mydvb_short_event_descriptor *s) {
	unsigned short len;

	strncpy (s->lang, buf+2, 3);
	s->lang[3] = '\0';

	buf += 5;
	len = *buf;
	buf++;

	s->event_name = mydvb_text_create (buf, len);

	buf += len;
	len = *buf;
	buf++;

	s->text = mydvb_text_create (buf, len);

}

void __free_short_event_descriptor (mydvb_short_event_descriptor *s) {
	
	mydvb_text_free (s->event_name);
	s->event_name = NULL;

	mydvb_text_free (s->text);
	s->text = NULL;

}

void __free_ext_event_item (void *p) {

	mydvb_ext_event_item *item = (mydvb_ext_event_item *) p;

	if (item == NULL) {
		return;
	}

	mydvb_text_free (item->item_description);
	item->item_description = NULL;

	mydvb_text_free (item->item_value);
	item->item_value = NULL;
}

void __parse_extended_event_descriptor (const unsigned char *buf, mydvb_extended_event_descriptor *d) {

	unsigned short len = buf[1];
	int n = 0;
	int m = 0;
	mydvb_ext_event_item *item;

	buf +=2;

	d->id 		= getbits (buf, 0, 4);
	d->last_id 	= getbits (buf, 4, 4);

	buf ++;

	strncpy (d->lang, buf, 3);
	d->lang[3] = '\0';

	buf += 3;

	/* items */
	d->items = dyn_array_new (sizeof (mydvb_ext_event_item));
	dyn_array_set_free_callback (d->items, __free_ext_event_item);
	n = *buf; /* length of items */

	buf++;
	while (n > 0) {
		item = (mydvb_ext_event_item *) dyn_array_add_empty (d->items);

		m = *buf; /* item description length */
		buf ++;
		n--;
		item->item_description = mydvb_text_create (buf, m);
		buf += m;
		n -= m;

		m = *buf; /* item length */
		buf ++;
		n--;
		item->item_value = mydvb_text_create (buf, m);
		buf += m;
		n -= m;

	}


	/* text */
	m = *buf; /* text length */
	buf ++;
	d->text = mydvb_text_create (buf, m);

}

void __free_extended_event_descriptor (mydvb_extended_event_descriptor *d) {

	dyn_array_free (d->items);
	d->items = NULL;

	mydvb_text_free (d->text);
	d->text = NULL;

}

void __free_parental_rating (void *p) {

	mydvb_country_parental_rating *cpr = (mydvb_country_parental_rating *) p;

	/* nothing, for now */
}

void __parse_parental_rating_descriptor (const unsigned char *buf, mydvb_parental_rating_descriptor *d) {

	mydvb_country_parental_rating *cpr = NULL;

	unsigned short len = buf[1];

	buf ++;

	d->ratings = dyn_array_new (sizeof (mydvb_country_parental_rating));
	dyn_array_set_free_callback (d->ratings, __free_parental_rating);

	while (len > 0) {
		cpr = dyn_array_add_empty (d->ratings);

		strncpy (cpr->country, buf, 3);
		cpr->country[3] = '\0';
		buf += 3;

		cpr->other = *buf;
		if (cpr->other >= 0x01 && cpr->other <= 0x0f) {
			cpr->min_years = cpr->other + 3;
		} else {
			cpr->min_years = 0;
		}

		// mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "--Parental rating descriptor %s %d %d", cpr->country, cpr->other, cpr->min_years);

		buf++;

		len -= 4;
	}


}

void __free_parental_rating_descriptor (mydvb_parental_rating_descriptor *d) {

	dyn_array_free (d->ratings);
	d->ratings = NULL;
}

void __parse_content_descriptor (const unsigned char *buf, mydvb_content_descriptor *cd) {
	int i;
	unsigned short len = buf[1];


	buf += 2;

	cd->num_contents = len/2;
	cd->content = (int *) malloc (sizeof(int) * cd->num_contents);

	cd->num_user = len/2;
	cd->user = (int *) malloc (sizeof(int) * cd->num_user);


	for (i=0; i < len / 2; i++) {
		cd->content[i]	= (int) buf[2*i];
		cd->user[i]		= (int) buf[2*i+1];
	}	

}

void __free_content_descriptor (mydvb_content_descriptor *cd) {
	if (cd->content != NULL) {
		free (cd->content);
		cd->content = NULL;
	}
	
	if (cd->user != NULL) {
		free (cd->user);
		cd->user = NULL;
	}

}

void __parse_component_descriptor (const unsigned char *buf, mydvb_component_descriptor *cd) {
	unsigned short len = buf[1];

	buf += 2;

	cd->stream_content_ext = getbits (buf, 0, 4);
	cd->stream_content	= getbits (buf, 4, 4);
	cd->component_type 	= getbits (buf, 8, 8);

	buf += 2;
	cd->component_tag 	= *buf;
	
	buf ++;
	strncpy (cd->lang, buf, 3);
	cd->lang[3] = '\0';

	len -= 6;
	buf += 3;

	cd->text = mydvb_text_create (buf, len);

}

void __free_component_descriptor (mydvb_component_descriptor *cd) {

	mydvb_text_free (cd->text);
	cd->text = NULL;	
}

void __parse_iso639_lang_descriptor (const unsigned char *buf, mydvb_iso639_lang_descriptor *d) {
	unsigned short len = buf[1];

	buf += 2;

	d->num_langs = len / 4;
	if (d->num_langs > 0) {
		d->langs = (iso639_lang *) malloc (sizeof (iso639_lang) * d->num_langs);

		for (int i = 0; i < d->num_langs; i++) {
			strncpy (d->langs[i].lang, buf, 3);
			d->langs[i].lang[3] = '\0';
			buf += 3;
			d->langs[i].audio_type = *buf;

			buf ++;
		}

	} else {
		d->langs = NULL;
	}

}

void __free_iso639_lang_descriptor (mydvb_iso639_lang_descriptor *d) {
	if (d->langs) {
		free (d->langs);
		d->langs = NULL;
	}
}


/* parse the ca descriptor */
void __parse_ca_descriptor (const unsigned char *buffer, mydvb_ca_descriptor *ca) {
	unsigned short len = buffer[1];

	buffer += 2;
	
	ca->ca_system_id = getbits (buffer, 0, 16);
	ca->ca_pid 		 = getbits (buffer, 19, 13);
	
	// mydvb_log (MYDVB_LOG_LEVEL_DEBUG, "CA Info: ca_system_id: %d, pid: %d\n", ca->ca_system_id, ca->ca_pid);

	buffer += 4;
	len    -= 4;
	if (len == 0) {
		ca->private = NULL;
		ca->private_len = 0;
		return;
	}
	ca->private_len = len;
	ca->private = (unsigned char *) malloc (ca->private_len);
	while (len > 0) {
		ca->private[ca->private_len - len] = *buffer;
		buffer ++;
		len --;	
	}

}

void __free_ca_descriptor (mydvb_ca_descriptor *ca) {
	if (ca->private == NULL) {
		return;
	}

	free (ca->private);
	ca->private = NULL;	
}

/**
 * Data broadcast descriptor
 */


 
void __parse_data_broadcast_descriptor (const unsigned char *buffer,
		mydvb_data_broadcast_descriptor *dbd) {
		
	/* unsigned char len = buffer[1];*/
	unsigned int lang;
	int i, n;

	char c_lang[4];

	buffer += 2;

	dbd->data_broadcast_id = getbits (buffer, 0, 16);
	dbd->component_tag	   = getbits (buffer, 16, 8);

	buffer += 3;	
	
	dbd->selectors_len 	  = getbits (buffer, 0, 8);

	buffer ++;
	if (dbd->selectors_len > 0) {
		dbd->selectors = (unsigned char *) malloc (dbd->selectors_len);
		for (i=0; i < dbd->selectors_len; i++) {
			dbd->selectors[i] = buffer[i];
		}
		buffer += dbd->selectors_len;
	} else {
		dbd->selectors = NULL;
	}
	
	/* iso text */
	lang = getbits (buffer, 0, 24);

	strncpy (dbd->lang, buffer, 3);
	dbd->lang[3] = '\0';

	buffer += 3;
	
	n = getbits (buffer, 0, 8);
	buffer++;

	dbd->text = mydvb_text_create (buffer, n);

}

void __free_data_broadcast_descriptor (mydvb_data_broadcast_descriptor *dbd) {
	if (dbd==NULL) {
		return;
	}
	
	if (dbd->selectors != NULL) {
		free (dbd->selectors);
		dbd->selectors = NULL;
		dbd->selectors_len = 0;
	}

	mydvb_text_free (dbd->text);
	dbd->text = NULL;

}

void __parse_data_broadcast_id_descriptor (const unsigned char *buffer,
		mydvb_data_broadcast_id_descriptor *dbd) {
		
	unsigned char len = buffer[1];
	int i;

	buffer += 2;

	dbd->data_broadcast_id = getbits (buffer, 0, 16);
	buffer += 2;	
	
	dbd->selectors_len 	  = len - 2;
	if (dbd->selectors_len > 0) {
		dbd->selectors = (unsigned char *) malloc (dbd->selectors_len);
		for (i=0; i < dbd->selectors_len; i++) {
			dbd->selectors[i] = buffer[i];
		}
		buffer += dbd->selectors_len;
	} else {
		dbd->selectors = NULL;
	}
}

void __free_data_broadcast_id_descriptor (mydvb_data_broadcast_id_descriptor *dbd) {
	if (dbd==NULL) {
		return;
	}

	if (dbd->selectors != NULL) {
		free (dbd->selectors);
		dbd->selectors = NULL;
		dbd->selectors_len = 0;
	}

}

int parse_descriptor (TABLE_TYPE_T t, const unsigned char *buffer, mydvb_descriptor *des) {
	unsigned char  descriptor_tag = buffer[0];
	int descriptor_len = buffer[1] + 2;

	des->info.table = t;
	des->info.type = descriptor_tag;

	if (descriptor_len == 2) {
		//warning("descriptor_tag == 0x%02x, len is 0\n", descriptor_tag);
		return 2;
	}
	
	switch (descriptor_tag) {
		/* iso tags */
		case 0x00:
			if (t==AIT) {
				__parse_application_descriptor (buffer, &des->app);		
			}
			break;
		case 0x01:
			if (t==AIT) {
				__parse_application_name_descriptor (buffer, &des->app_name);
			}
			break;
		case 0x02:
			if (t==AIT) {
				__parse_transport_protocol_descriptor (buffer, &des->app_tp);
			}
			break;
		case 0x03:
			if (t==AIT) {
				__parse_dvb_j_application_descriptor (buffer, &des->app_dvbj);
			}
			break;
		case 0x04:
			if (t==AIT) {
				__parse_dvb_j_application_loc_descriptor (buffer, &des->app_dvbj_loc);
			}
			break;
		case 0x09: 
			if (t==CAT || t == PMT) {
				__parse_ca_descriptor (buffer, &des->ca);
			}
			break;
		case 0x0a:
			//if (t == PMT)
			__parse_iso639_lang_descriptor (buffer, &des->iso639_lang);
			break;
		case 0x13:
			printf ("CAROUSEL id descriptor %d\n", t);
			break;
		/* del 0x40 en adelante son de dvb */
		case 0x40:
			if (t == NIT) {
				__parse_network_name_descriptor (buffer, &des->network_name);
			}
            break;
		/*
        case 0x43:
			//if (t == NIT)
			//	parse_satellite_delivery_system_descriptor (buffer, data);
			break;

        case 0x44:
			//if (t == NIT)
			//	parse_cable_delivery_system_descriptor (buffer, data);
			break;
		*/
		case 0x48:
			if (t == SDT)
				__parse_service_descriptor (buffer, &des->service);
			break;
		case 0x4d:
			/* short event descriptor */
			if (t == EIT) {
				__parse_short_event_descriptor (buffer, &des->short_event);
				//printf ("EIT short event desc: %s %s\n", des->short_event.event_name, des->short_event.text);
			}
			break;
		case 0x4e:
			/* extended event descriptor */
			if (t == EIT) {
				__parse_extended_event_descriptor (buffer, &des->extended_event);
			}
			break;
		case 0x50: /* component descriptor */
			if (t == EIT) {
				__parse_component_descriptor (buffer, &des->component);
			}
			break;
		case 0x52: // stream identifier descriptor
			if (t==PMT) {
				des->stream_id.component_tag = buffer[2];
			}
			break;
		/*
		case 0x53:
			//if (t == SDT)
			//	parse_ca_identifier_descriptor (buffer, data);
			break;
		*/
		case 0x54:
			if (t==EIT) {
				__parse_content_descriptor (buffer, &des->content);
			}
			break;
		case 0x55:
			if (t==EIT) {
				__parse_parental_rating_descriptor (buffer, &des->parental_rating);
			}
			break;
		/*
		case 0x5a:
			//if (t == NIT)
			//	parse_terrestrial_delivery_system_descriptor (buffer, data);
			break;

		case 0x62:
			//if (t == NIT)
			//	parse_frequency_list_descriptor (buffer, data);
			break;
		*/
		case 0x64:
			/* data broadcast descriptor */
			if (t == SDT || t == EIT) {
				__parse_data_broadcast_descriptor (buffer, &des->data_broadcast);
			}

			break;
		case 0x66:
			/* data broadcast id descriptor */
			if (t == PMT) {
				__parse_data_broadcast_id_descriptor (buffer, &des->data_broadcast_id);
			}
			break;
		case 0x67 ... 0x6e: /* reserved for future use */
			break;
		case 0x6f: 
			/* MHP: application signalling descriptor */
			if (t == PMT) {
				__parse_application_signalling_descriptor (buffer, &des->app_signal);
			}
			break;
		case 0x70 ... 0x7f: /* reserved for future use */
			break;
		case 0x80 ... 0xfe: /* user defined */
			break;
		case 0xff: /* forbidden */
			break;
		default:
			//verbosedebug("skip descriptor 0x%02x\n", descriptor_tag);
			//printf ("Unknown tag descriptor: 0x%02x, tabla %d\n", descriptor_tag, t);
			;
	}

    return descriptor_len;	
	
}

void free_descriptor (void *obj) {

	mydvb_descriptor *des = (mydvb_descriptor *) obj;
	
	switch (des->info.type) {
		/* iso tags */
		case 0x00:
			if (des->info.table == AIT) {
				__free_application_descriptor (&des->app);
			}
			break;
		case 0x01:
			if (des->info.table == AIT) {
				__free_application_name_descriptor (&des->app_name);
			}
			break;
		case 0x02:			
			break;
		case 0x03:
			if (des->info.table == AIT) {
				__free_dvb_j_application_descriptor (&des->app_dvbj);
			}
			break;
		case 0x04:
			if (des->info.table == AIT) {
				__free_dvb_j_application_loc_descriptor (&des->app_dvbj_loc);
			}
			break;
		case 0x09: 
			if (des->info.table==CAT || des->info.table == PMT) {
				__free_ca_descriptor (&des->ca);
			}
			break;
		case 0x0a:
			__free_iso639_lang_descriptor (&des->iso639_lang);
			break;

		/* del 0x40 en adelante son de dvb */
		case 0x40:
			if (des->info.table == NIT) {
				__free_network_name_descriptor (&des->network_name);
			}
         break;
		/*
        case 0x43:
			//	__free_satellite_delivery_system_descriptor (data);
			break;

        case 0x44:
			//	__free_cable_delivery_system_descriptor (data);
			break;
		*/
		case 0x48:
			if (des->info.table==SDT) {
				__free_service_descriptor (&des->service);
			}
			break;
		case 0x4d:
			if (des->info.table==EIT) {
				__free_short_event_descriptor (&des->short_event);
			}
			break;
		case 0x4e:
			if (des->info.table==EIT) {
				__free_extended_event_descriptor (&des->extended_event);
			}
			break;
		case 0x50: /* component descriptor */
			if (des->info.table==EIT) {
				__free_component_descriptor (&des->component);
			}
			break;
		case 0x52: // stream identifier descriptor
			break;
		/*
		case 0x53:
			//	__free_ca_identifier_descriptor (data);
			break;
		*/
		case 0x54:
			if (des->info.table==EIT) {
				__free_content_descriptor (&des->content);
			}
			break;
		case 0x55:
			if (des->info.table==EIT) {
				__free_parental_rating_descriptor (&des->parental_rating);
			}
			break;
		/*
		case 0x5a:
			//	__free_terrestrial_delivery_system_descriptor (data);
			break;

		case 0x62:
			//	__free_frequency_list_descriptor (data);
			break;
		*/
		case 0x64:
			if (des->info.table == SDT || des->info.table == EIT) {
				__free_data_broadcast_descriptor (&des->data_broadcast);
			}
			break;
		case 0x66:
			if (des->info.table == PMT) {
				__free_data_broadcast_id_descriptor (&des->data_broadcast_id);
			}
			break;			
		case 0x67 ... 0x6e: /* reserved for future use */
			break;
		case 0x6f: 
			if (des->info.table == PMT) {
				__free_application_signalling_descriptor (&des->app_signal);
			}
			break;
		case 0x70 ... 0x7f: /* reserved for future use */
			break;
		case 0x80 ... 0xfe: /* user defined */
			break;
		case 0xff: /* forbidden */
			break;
		default:
			;	
	}
}

void _save_descriptor (FILE *f, mydvb_descriptor *des) {
	if (des == NULL) {
		return;
	}


}

void save_descriptors (FILE *f, DYN_ARRAY *descriptors) {
	int i = 0;

	if (descriptors == NULL) {
		return;
	}

	for (i = 0; i < dyn_array_get_size (descriptors); i++) {
		_save_descriptor (f, dyn_array_get_data (descriptors, i));
	}
}


