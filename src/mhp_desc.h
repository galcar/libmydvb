#ifndef _MHP_DESC_H_
#define _MHP_DESC_H_

#include <mydvb_mhp_desc.h>

void __parse_application_signalling_descriptor (const unsigned char *buffer,
								mydvb_application_signalling_descriptor *asd);
void __free_application_signalling_descriptor (mydvb_application_signalling_descriptor *asd);

void __parse_transport_protocol_descriptor (const unsigned char *buffer,
								mydvb_transport_protocol_descriptor *tpd);

void __parse_application_descriptor (const unsigned char *buffer,
								mydvb_application_descriptor *ad);
void __free_application_descriptor (mydvb_application_descriptor *ad);

void __free_application_name_descriptor (mydvb_application_name_descriptor *and);
void __parse_application_name_descriptor (const unsigned char *buffer,
								mydvb_application_name_descriptor *and);
								

/**
 * DVB-J
 */
 
void __free_dvb_j_application_descriptor (mydvb_dvb_j_application_descriptor *jd);
void __parse_dvb_j_application_descriptor (const unsigned char *buffer,
								mydvb_dvb_j_application_descriptor *jd);

void __free_dvb_j_application_loc_descriptor (mydvb_dvb_j_application_loc_descriptor *jld);
void __parse_dvb_j_application_loc_descriptor (const unsigned char *buffer,
								mydvb_dvb_j_application_loc_descriptor *jld);
								

#endif
