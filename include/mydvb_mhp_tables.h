#ifndef _MYDVB_MHP_TABLES_H_
#define _MYDVB_MHP_TABLES_H_

#include <dynarray.h>

typedef struct {


	unsigned char ctrl_code;

	DYN_ARRAY *descriptors;

} MYDVB_AIT_APP;

/**
 * Application Information Table
 */
typedef struct {
	unsigned char version; 
	unsigned char test_app_flag;
	unsigned short app_type;

	DYN_ARRAY *descriptors;				/* common descriptors */
	
	DYN_ARRAY *apps;						/* applications */
	
	int pid;
	int fd;

} MYDVB_AIT;

#endif
