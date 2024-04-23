#ifndef __MYDVB_DESCRIPTORS_H__
#define __MYDVB_DESCRIPTORS_H__

#include <mydvb_mhp_desc.h>

#include <mydvb_text.h>

#include <mydvb.h>

#include <dynarray.h>

typedef struct {

	mydvb_text *item_description;
	mydvb_text *item_value;

} mydvb_ext_event_item;

typedef struct {

	unsigned char country[4];

	unsigned short min_years;

	unsigned short other;

} mydvb_country_parental_rating;


typedef struct {
	unsigned char			table;
	unsigned char 			type;

	mydvb_text  			*name; /* network name */
} mydvb_network_name_descriptor;

/* the ca descriptor */
typedef struct {
	unsigned char			table;
	unsigned char 			type;

	unsigned short 			ca_system_id;
	unsigned short 			ca_pid;

	int 					private_len;
	unsigned char 			*private;
} mydvb_ca_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char 			type;

	unsigned char 			service_type; /* ver desc_info.h mydvb_service_type_table */

	mydvb_text 				*provider_name;
	mydvb_text		 		*service_name;

} mydvb_service_descriptor;


typedef struct {
	unsigned char			table;
	unsigned char 			type;

	unsigned char			lang[4];

	mydvb_text				*event_name;
	mydvb_text				*text;

} mydvb_short_event_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char 			type;

	unsigned char			id;			/* descriptor number */
	unsigned char			last_id;	/* last descriptor number */

	unsigned char			lang[4];

	DYN_ARRAY				*items;

	mydvb_text				*text;

} mydvb_extended_event_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char			type;

	DYN_ARRAY				*ratings;

} mydvb_parental_rating_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char 			type;

	int 					num_contents;
	int *					content;		/* tabla de contenidos clasificados, ver desc_info.h */

	int 					num_user;
	int *					user;			/* tabla de contenidos definidos por el broadcaster */

} mydvb_content_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char 			type; 			/* stream identifier tag, shall be 0x52 */

	unsigned char 			component_tag; /* component tag */

} mydvb_stream_id_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char 			type; /* component descriptor tag */

	unsigned char			stream_content;
	unsigned char 			component_type;
	unsigned char 			component_tag;

	unsigned char			lang[4];
	mydvb_text			 	*text;

} mydvb_component_descriptor;

/**
 * Data broadcast descriptor
 */
typedef struct {
	unsigned char			table;
	unsigned char			type; /* shall be 0x64 */

	unsigned short			data_broadcast_id;
	unsigned char			component_tag;

	unsigned char			selectors_len;
	unsigned char	*		selectors;

	unsigned char			lang[4];
	mydvb_text				*text;

} mydvb_data_broadcast_descriptor;

typedef struct {
	unsigned char			table;
	unsigned char			type;	/* shall be 0x66 */

	unsigned short			data_broadcast_id;

	unsigned char			selectors_len;
	unsigned char	*		selectors;

} mydvb_data_broadcast_id_descriptor;


typedef union {
	struct {
		unsigned char		table;
		unsigned char 		type;
	}														info;
	mydvb_network_name_descriptor							network_name;
	mydvb_ca_descriptor										ca;
	mydvb_service_descriptor 								service;
	mydvb_stream_id_descriptor 								stream_id;
	mydvb_short_event_descriptor 							short_event;
	mydvb_extended_event_descriptor							extended_event;
	mydvb_parental_rating_descriptor						parental_rating;
	mydvb_content_descriptor 								content;
	mydvb_component_descriptor 								component;

	mydvb_data_broadcast_descriptor							data_broadcast;
	mydvb_data_broadcast_id_descriptor						data_broadcast_id;

	/* MHP descriptors */
	mydvb_application_signalling_descriptor					app_signal;
	mydvb_application_descriptor							app;
	mydvb_application_name_descriptor						app_name;
	mydvb_transport_protocol_descriptor						app_tp;
	mydvb_dvb_j_application_descriptor						app_dvbj;
	mydvb_dvb_j_application_loc_descriptor					app_dvbj_loc;
	mydvb_mhp_data_broadcast_id_descriptor					app_db_id;

} mydvb_descriptor;


mydvb_descriptor *	mydvb_descriptor_find (DYN_ARRAY *da, unsigned char tag);

#endif
