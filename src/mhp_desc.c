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
#include <stdio.h>

#include <mydvb.h>

#include "mhp_desc.h"

#include "dynarray.h"

#include "util.h"

/** 
 * Application Signalling Descriptor parser
 */ 
void __parse_application_signalling_descriptor (const unsigned char *buffer,
								mydvb_application_signalling_descriptor *asd) {
								
	unsigned char len = buffer[1];
	int i, total;	
	mydvb_app_sig *ref;
	
	buffer += 2;
		
	total = len / 3;
	
	if (total > 0) {
		asd->apps = dyn_array_new (sizeof (mydvb_app_sig));		
	
		for (i=0; i < total; i++) {
			ref = (mydvb_app_sig *) dyn_array_add_empty (asd->apps);
			
			ref->type 			= getbits (buffer, 0, 16);
			ref->AIT_version 	= getbits (buffer, 19, 5);
	
			buffer += 3;
		}	
	} else {
		asd->apps = NULL;
	}
	
}

void __free_application_signalling_descriptor (mydvb_application_signalling_descriptor *asd) {
	if (asd->apps == NULL) {
		return;
	}

	dyn_array_free (asd->apps);
	asd->apps = NULL;	
}


/** 
 * Transport Protocol Descriptor parser
 */
void __parse_transport_protocol_descriptor (const unsigned char *buffer,
								mydvb_transport_protocol_descriptor *tpd) {
								
	/* unsigned char len = buffer[1]; */
	
	buffer += 2;
	
	tpd->id 		= getbits (buffer, 0, 16);
	tpd->label 	= getbits (buffer, 16, 8);
	
	buffer += 3;
	
	switch (tpd->id) {
		case TP_OBJECT_CAROUSEL:
			tpd->selector.tp_oc.remote_flag = getbits (buffer, 0, 1);
			if (tpd->selector.tp_oc.remote_flag==1) {
				tpd->selector.tp_oc.original_nid 	= getbits (buffer, 8, 16);
				tpd->selector.tp_oc.transport_sid 	= getbits (buffer, 16, 16);
				tpd->selector.tp_oc.service_id 		= getbits (buffer, 32, 16);
				buffer += 7;				
			} else {
				tpd->selector.tp_oc.original_nid 	= 0;
				tpd->selector.tp_oc.transport_sid 	= 0;
				tpd->selector.tp_oc.service_id 		= 0;											
				buffer += 1;
			}
			tpd->selector.tp_oc.component_tag = getbits (buffer, 0, 8);			
			break;
		case TP_IP_VIA_DVB:
			/** not yet implemented */
			break;
		case TP_HTTP:
			/** not yet implemented */
			break;
	}
	
}


/** 
 * Application Descriptor parser
 */
void __parse_application_descriptor (const unsigned char *buffer,
								mydvb_application_descriptor *ad) {
								
	unsigned char len = buffer[1];
	int i, n, total;	
	mydvb_app_profile *ref;
	
	buffer += 2;
			
	n = getbits (buffer, 0, 8);
	buffer ++;
	
	total = n / 5;
	if (total > 0) {
		ad->profiles = dyn_array_new (sizeof(mydvb_app_profile));

		for (i=0; i < total; i++) {
			ref = (mydvb_app_profile *) dyn_array_add_empty (ad->profiles);
			
			ref->application_profile = getbits (buffer, 0, 16);
			ref->major = getbits (buffer, 16, 8);
			ref->minor = getbits (buffer, 24, 8);
			ref->micro = getbits (buffer, 32, 8);				
		
			buffer += 5;
		}
	} else {
		ad->profiles = NULL;
	}
	
	ad->service_bound_flag 	= getbits (buffer, 0, 1);
	ad->visibility 			= getbits (buffer, 1, 2);
	ad->priority 				= getbits (buffer, 8, 8);
	
	buffer += 2;
	
	ad->labels_len = len - 2 - 1 - 5 * total;
	if (ad->labels_len > 0) {
		ad->labels = (unsigned char *) malloc (sizeof(ad->labels_len));
		for (i=0; i < ad->labels_len; i++) {
			ad->labels[i] = buffer[i];
		}
	} else {
		ad->labels = NULL;
	}
}

void __free_application_descriptor (mydvb_application_descriptor *ad) {

	dyn_array_free (ad->profiles);
	ad->profiles = NULL;	
	
	if (ad->labels != NULL) {
		free (ad->labels);
		ad->labels = NULL;
		ad->labels_len = 0;
	}
}


/** 
 * Application Name Descriptor parser
 */

void __free_application_name_descriptor (mydvb_application_name_descriptor *and) {

	dyn_array_free (and->iso_names);
	and->iso_names = NULL;
	
} 


void __free_mydvb_iso_name (void *p) {
	mydvb_iso_name *n = (mydvb_iso_name *) p;

	if (n==NULL) {
		return;
	}
	
	if (n->name!=NULL) {
		free (n->name);
		n->name = NULL;
	}
}
 
void __parse_application_name_descriptor (const unsigned char *buffer,
								mydvb_application_name_descriptor *and) {
								
	unsigned char len = buffer[1];
	int i, n, count;	
	mydvb_iso_name *ref;
		
	buffer += 2;
	
	if (len==0) {
		and->iso_names = NULL;
		return;
	}
	
	and->iso_names = dyn_array_new (sizeof (mydvb_iso_name));
	dyn_array_set_free_callback (and->iso_names, __free_mydvb_iso_name);
	count = 0;
	while (count < len) {
		ref = dyn_array_add_empty (and->iso_names);

		ref->lang = getbits (buffer, 0, 24);
		buffer += 3;
	
		n = getbits (buffer, 0, 8);
		buffer ++;
		if (n > 0) {
			ref->name = (unsigned char *) malloc (n+1);
			for (i=0; i < n; i++) {
				ref->name[i] = buffer[i];
			}	
			ref->name[i] = 0;
						
			buffer += n;
			printf ("APP NAME: %s\n", ref->name);
		} else {
			ref->name = NULL;
		}	
		
		count += 4+n;
	}
}


/**
 * ----------------------------------- DVB-J ----------------------------- 
 */
 
void __dvb_j_param_free (void *p) {
	dvb_j_param *ref = (dvb_j_param *) p;
	
	if (ref==NULL) {
		return;
	}
	
	if (ref->value != NULL) {
		free (ref->value);
		ref->value = NULL;
		ref->len = 0;
	}
} 

void __free_dvb_j_application_descriptor (mydvb_dvb_j_application_descriptor *jd) {
	if (jd==NULL) {
		return;
	}

	dyn_array_free (jd->params);

	jd->params = NULL;
}
 
/**
 * DVB-J Application Descriptor
 */
void __parse_dvb_j_application_descriptor (const unsigned char *buffer,
								mydvb_dvb_j_application_descriptor *jd) {
								
	unsigned char len = buffer[1];
	int i, count;	
	dvb_j_param *ref;
		
	buffer += 2;
	
	if (len==0) {
		jd->params = NULL;
		return;
	}

	jd->params = dyn_array_new (sizeof (dvb_j_param));
	dyn_array_set_free_callback (jd->params, __dvb_j_param_free);
	
	count = 0;
	while (count < len) {

		ref = (dvb_j_param *) dyn_array_add_empty (jd->params);		
			
		ref->len = getbits (buffer, 0, 8);
		buffer ++;
		count ++;
		if (ref->len > 0) {
			ref->value = (unsigned char *) malloc (ref->len + 1);
			for (i=0; i < ref->len; i++) {
				ref->value[i] = buffer[i];
			}	
			buffer += ref->len;
			ref->value[i] = 0;
			printf ("APP J PARAM VALUE: %s\n", ref->value);
		} else {
			printf ("APP J PARAM NULL\n");
			ref->value = NULL;
		}
		
		count += ref->len;
	}
}


/**
 *	DVB-J Application Location Descriptor
 */
 
void __free_dvb_j_application_loc_descriptor (mydvb_dvb_j_application_loc_descriptor *jld) {
	if (jld==NULL) {
		return;
	}
	
	if (jld->base_path!=NULL) {
		free (jld->base_path);
		jld->base_path = NULL;
	}
	
	if (jld->classpath!=NULL) {
		free (jld->classpath);
		jld->classpath = NULL;
	}

	if (jld->class!=NULL) {
		free (jld->class);
		jld->class = NULL;
	}
	
} 
 
void __parse_dvb_j_application_loc_descriptor (const unsigned char *buffer,
								mydvb_dvb_j_application_loc_descriptor *jld) {
								
	unsigned char len = buffer[1];
	int i, n, cont;	
		
	buffer += 2;
	
	if (len==0) {
		jld->base_path = NULL;
		jld->classpath = NULL;
		jld->class		= NULL;
		return;
	}
	
	cont = 0;	
	
	/* base path */
	n = getbits (buffer, 0, 8);
	buffer ++; cont++;
	if (n>0) {
		jld->base_path = (unsigned char *) malloc (n + 1);
		for (i=0; i < n; i++) {
			jld->base_path[i] = buffer[i];
		}
		jld->base_path[i] = 0;
		buffer +=n;
		printf ("DVB-J BASE PATH: %s\n", jld->base_path);
	} else {
		jld->base_path = NULL;
	}
	cont += n;	
	
	/* classpath */
	n = getbits (buffer, 0, 8);
	buffer ++; cont ++;
	if (n>0) {
		jld->classpath = (unsigned char *) malloc (n + 1);
		for (i=0; i < n; i++) {
			jld->classpath[i] = buffer[i];
		}
		jld->classpath[i] = 0;
		buffer += n;
		printf ("DVB-J CLASSPATH: %s\n", jld->classpath);
	} else {
		jld->classpath = NULL;
	}
	cont += n;
	
	/* initial class */
	n = len - cont;
	if (n>0) {
		jld->class = (unsigned char *) malloc (n + 1);
		for (i=0; i < n; i++) {
			jld->class[i] = buffer[i];
		}
		jld->class[i] = 0;
		buffer +=n;
		printf ("DVB-J CLASS: %s\n", jld->class);
	} else {
		jld->class = NULL;
	}	
}
