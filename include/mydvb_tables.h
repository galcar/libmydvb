#ifndef __MYDVB_TABLES_H__
#define __MYDVB_TABLES_H__

#include <time.h>

#include <dynarray.h>

#include <mydvb_date.h>

typedef enum table_type {
			PAT,
			CAT,
			PMT,
			SDT,
			NIT,
			EIT,
			AIT
} TABLE_TYPE_T;

typedef struct {
	
	unsigned short 				number; 			/* program/service pid */
	unsigned char 				running_status;
	unsigned char 				scram; 				/* 1 scrambled, 0 not scram */
	unsigned char 				eit;				/* 1 tiene eit, 0 no tiene */
	unsigned char				eit_pw;				/* 1 tiene eit present/following, 0 no tiene */
	
	DYN_ARRAY 			*		descriptors;

} mydvb_service_description;

typedef struct {
	unsigned short ts_id;
	unsigned short original_net_id;
	
	DYN_ARRAY *descriptors;

} mydvb_transport_description;

/* ------------------ CMT ------------------------- */
/* -- CA Message Table -- */
typedef struct {
	unsigned char table_id; /* table identification */
	
	unsigned char data_len; /* len of private data */
	unsigned char *data;	/* private data */
} MYDVB_CMT;

/* ------------------ EIT ------------------------- */
/* EIT entry */
typedef struct {
	unsigned short 				event_id;
	mydvb_date		*			start_time; 		/* start date */
	int 						duration; 			/* in seconds */
	unsigned char 				running_status;
	unsigned char 				scram; 				/* 1 scrambled, 0 not scram */
	
	DYN_ARRAY 		*			descriptors;

} MYDVB_EIT_ENTRY;

typedef struct {
	unsigned short number; // program pid o service_id
	unsigned char version; 
	unsigned char initial_section_number;

	DYN_ARRAY *entries;

	int incomplete;	/* indica cuando la EIT esta completa. Inicialmente vale 2, cuando
						se encuentra la section_number = last_section_number, 
						se decrementa en 1. Por lo tanto, la segunda vez que detecta
						que se ha encontrado con la last_section_number la EIT
						se habra completado */
} MYDVB_EIT;

/* --------------- TIME AND DATE TABLE ------------- */
typedef struct {
	time_t t;
} MYDVB_TDT;

/* ------------------- STREAMS --------------------- */
typedef struct {
	unsigned char type;
	unsigned short pid;
	
	DYN_ARRAY *descriptors;

} MYDVB_STREAM;

typedef struct {
	unsigned short number;
	unsigned short pmt_pid;

	unsigned char version;

	short section_n;
	short section_n_l;

	unsigned short pcr_pid;

	DYN_ARRAY *descriptors;

	DYN_ARRAY *streams;

	MYDVB_EIT *eit;				/* programa actual y siguiente */
	MYDVB_EIT *eit_sched[16];	/* planificacion de la programacion, hasta 16 dias? */

	int ready;

} MYDVB_PROGRAM;

/* ----------- SDT -------------------- */
typedef struct {
	unsigned short ts_id;		/* transport stream id */
	unsigned char version;

	unsigned short original_net_id;	/* original network id */

	DYN_ARRAY *service_description;
	
} MYDVB_SDT;

/* ---------- Conditional Access Table --------------- */
typedef struct {
	unsigned char version;
 	unsigned short section_n;
	unsigned short section_n_l;

	DYN_ARRAY *descriptors;

} MYDVB_CAT;

/* ---------- Network Information Table -------------- */
typedef struct {
	unsigned short network_id;	/* network identification */
	unsigned char version;
	short section_n;
	short section_n_l;
	
	DYN_ARRAY *descriptors;

	DYN_ARRAY *transport_desc;

	unsigned short pid;

} MYDVB_NIT;

/**
 * Esta es la tabla PAT, existe una por canal. Puede contener un cierto
 * numero de programas y, opcionalmente, una NIT
*/
typedef struct {
	unsigned short ts_id;		/* transport stream id */
	unsigned char version;
	short section_n;
	short section_n_l;
	
	MYDVB_NIT *nit;

	DYN_ARRAY *programs;

} MYDVB_PAT;


#endif
