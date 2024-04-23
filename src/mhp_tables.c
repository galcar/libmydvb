
#include <stdlib.h>
#include <stdio.h>

#include <dynarray.h>

#include "mhp_tables.h"

#include "util.h"
#include "logger.h"

#include "descriptor.h"

void __free_ait_app (void *p) {
	MYDVB_AIT_APP *ait_app = (MYDVB_AIT_APP *) p;
	
	if (ait_app==NULL) {
		return;
	}
	
	dyn_array_free (ait_app->descriptors);
	ait_app->descriptors = NULL;
	
}

MYDVB_AIT *mydvb_new_ait () {
	MYDVB_AIT *ait = NULL;
	
	ait = (MYDVB_AIT *) malloc (sizeof (MYDVB_AIT));
	ait->version = 0xff;
	ait->test_app_flag = 0;
	ait->app_type = 0;
	ait->descriptors = NULL;
	ait->apps = NULL;
	
	ait->pid = 0;
	ait->fd  = -1;
	
	return ait;
}

MYDVB_AIT *mydvb_new_ait_if_changed (const unsigned char *buffer, size_t len, MYDVB_AIT *ait) {
	unsigned char new_version;
	const unsigned char *bufptr;
	int counter, counter2, dl;
	mydvb_descriptor *descriptor;
	MYDVB_AIT_APP *ait_app = NULL;

	mydvb_hex_log (1, buffer, len);

	if (ait==NULL) {
		ait = mydvb_new_ait ();
	} 

	new_version = getbits (buffer, 42, 5);
	if (ait->version==new_version) {
		return ait;
	}

	ait->version 				= new_version;
	ait->test_app_flag 		= getbits (buffer, 24, 1);
	ait->app_type 				= getbits (buffer, 25, 15);

	/* common descriptors */
	dyn_array_free (ait->descriptors);
	ait->descriptors = dyn_array_new (sizeof (mydvb_descriptor));
	dyn_array_set_free_callback (ait->descriptors, free_descriptor);
	
	counter  	= getbits (buffer, 68, 12);
	bufptr 		= buffer + 10;
	while (counter > 0) {

		descriptor = dyn_array_add_empty (ait->descriptors);
		dl = parse_descriptor (AIT, bufptr, descriptor);
		/* possible event callback here */

		bufptr 	+= dl;
		counter 	-= dl;
	}	
	/* end common descriptors */
	
	buffer = bufptr;

	/* application loop */	
	counter = getbits (buffer, 4, 12);
	buffer += 2;
	dyn_array_free (ait->apps);
	ait->apps = dyn_array_new (sizeof (MYDVB_AIT_APP));	
	dyn_array_set_free_callback (ait->apps, __free_ait_app);
	while (counter > 0) {
		ait_app = dyn_array_add_empty (ait->apps);
		ait_app->descriptors = NULL;
		
		/* app identifier */
		buffer  += 6;
		counter -= 6;
		
		ait_app->ctrl_code = getbits (buffer, 0, 8);
		buffer  ++;
		counter --;
		
		/* app descriptors loop */
		dyn_array_free (ait_app->descriptors);
		ait_app->descriptors = dyn_array_new (sizeof (mydvb_descriptor));		
		dyn_array_set_free_callback (ait_app->descriptors, free_descriptor);		
		
		counter2  	= getbits (buffer, 4, 12);
		counter 	  -= counter2;
		bufptr 		= buffer + 2;
		counter 	  -= 2;		
		while (counter2 > 0) {

			descriptor = dyn_array_add_empty (ait_app->descriptors);
			dl = parse_descriptor (AIT, bufptr, descriptor);
			/* possible event callback here */

			bufptr 	+= dl;
			counter2 -= dl;
		}			
		
		buffer = bufptr;
	}	
		
	return ait;	
}

void mydvb_delete_ait (MYDVB_AIT *ait) { 
	if (ait==NULL) {
		return;
	}
		
	dyn_array_free (ait->descriptors);
	
	dyn_array_free (ait->apps);
	
	free (ait);
}
