#ifndef _MYDVB_MHP_DESC_H_
#define _MYDVB_MHP_DESC_H_

#include <dynarray.h>

typedef struct {

	unsigned int lang;
	unsigned char *name;

} mydvb_iso_name;

/**
 Indicador de Aplicacion elemental: contiene el tipo de aplicaion (1 para DVB-J) y
 la version de la tabla en la que se encuentra descrito.
 */
typedef struct {

	unsigned short 		type;
	unsigned char  		AIT_version;

} mydvb_app_sig;

/**
 El descriptor de indicacion de aplicacion contiene una lista de indicadores elementales
 */
typedef struct {
	unsigned char 			type;

	DYN_ARRAY 		*		apps;
	
} mydvb_application_signalling_descriptor;


#define TP_OBJECT_CAROUSEL	0x0001
#define TP_IP_VIA_DVB		0x0002
#define TP_HTTP				0x0003

/**
 informacion para protocolo de transporte object carousel
 */
typedef struct {
	unsigned char 		remote_flag;	/* 1 indica servicio remoto */
	
	unsigned short	 	original_nid;	/* original network id */
	unsigned short 	transport_sid; /* transport stream id */
	unsigned short 	service_id;		/* service id */
	
	unsigned char		component_tag;

} mydvb_tp_oc;

/**
 El descriptor de protocolo de transporte
 */
typedef struct {
	unsigned char					type;		/* shall be 0x02 */
	
	unsigned short					id;		/* identificador de protocolo, 1 para Carousel */
	unsigned char					label;
	
	union {
		mydvb_tp_oc	tp_oc;
	}									selector;
	
} mydvb_transport_protocol_descriptor;



typedef struct {
	unsigned short					application_profile;
	unsigned char 					major;
	unsigned char 					minor;
	unsigned char 					micro;		
	
} mydvb_app_profile;

/** 
 * application descriptor
 */
typedef struct {
	unsigned char 					type;						/* shall be 0x00 */
	
	DYN_ARRAY			*			profiles;
	
	unsigned char					service_bound_flag;
	
	unsigned char					visibility;
	
	unsigned char					priority;
	
	unsigned char					labels_len;
	unsigned char	*				labels;

} mydvb_application_descriptor; 
 

/** 
 * Application name descriptor
 */
typedef struct {
	unsigned char					type;			/* shall be 0x01 */
	
	DYN_ARRAY			*			iso_names;

} mydvb_application_name_descriptor;



/**
 DVB-J Param 
*/
typedef struct {
	unsigned char		len;
	unsigned char	*	value;
	
} dvb_j_param;

/**
 DVB-J Application Descriptor
*/
typedef struct {
	unsigned char					type;			/* shall be 0x03 */
	
	DYN_ARRAY			*			params;

} mydvb_dvb_j_application_descriptor;

/**
 * DVB-J Application location Descriptor
 */
typedef struct {
	unsigned char					type;			/* shall be 0x04 */
	
	unsigned char 		*			base_path;
	unsigned char		*			classpath;
	unsigned char		*			class;
	
} mydvb_dvb_j_application_loc_descriptor;


/**
 * Data broadcast id descriptor
 */
typedef struct {
	unsigned char			type;	/* shall be 0x66 */
	
	unsigned short			data_broadcast_id;
	
	unsigned char			apps_len;
	unsigned short	*		apps;		
	
} mydvb_mhp_data_broadcast_id_descriptor;
								

#endif
