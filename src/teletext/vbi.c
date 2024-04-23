
#include "vbi.h"

static void __mydvb_txt_reader (TXT_BUFFER *t, int fd, void (*cb)(TXT_BUFFER *t, void *data), void *data) {
	int leidos = 0;
	int faltan_datos = 0;
	int hlen;

	while ((leidos = read (fd, t->rp, 500)) != -1) {
		t->n += leidos;
		t->rp += leidos;
		faltan_datos = 0;
		while (t->n > 0 && !faltan_datos) {
			switch (t->estado) {
				case 0: 
					if (t->n >= 1) {
						if (t->bufptr[0]==0) {
							t->estado = 1;
						} 
						t->bufptr ++;
						t->n--;
					} else {
						faltan_datos = 1;
					}
					break;
				case 1: 
					if (t->n >= 1) {
						if (t->bufptr[0]==0) {
							t->estado = 2;
						} else {
							t->estado = 0;
						}
						t->bufptr++; 
						t->n--;
					} else {
						faltan_datos = 1;
					}
					break;
				case 2: 
					if (t->n >= 1) {	
						if (t->bufptr[0]==1) {
							t->estado = 3;
						} else {
							t->estado = 0;
						}
						t->bufptr++; 
						t->n--;
					} else {
						faltan_datos = 1;
					}
					break;
				case 3: 
					if (t->n >= 1) {
						if (t->bufptr[0]==0xbd) { /* private stream */
							t->estado = 4;
						} else {
							t->estado = 0;
						}
						t->bufptr++; 
						t->n--;
					} else {
						faltan_datos = 1;
					}
					break;
				case 4: 
					if (t->n >= 5) {
						/* PES packet len */
				        t->plen = ((t->bufptr[0] << 8) | t->bufptr[1]) & 0xffff;
				        /* PES header data length */
				        hlen = t->bufptr[4];
						if (hlen != 0x24) { /* error */
							t->rp = t->data;
							t->bufptr = t->data;
							t->estado = 0;
						} else {
							t->bufptr += 5;
							t->estado = 5;
						}
						t->n -= 5;
					} else {
						/* no hay datos suficientes, leer mas */						
						faltan_datos = 1;
					}		
					break;
				case 5:
					if (t->n >= 36) {
				        /* skip rest of PES header */
						t->bufptr+=36;
						t->n -= 36;
						t->estado = 6;
					} else {
						/* no hay datos suficientes, leer mas */
						faltan_datos = 1;
					}
					break;
				case 6:
					if (t->n >= 1) {
						/* read data id */
						t->data_id = t->bufptr[0];
						t->bufptr++;
						t->n --;
						t->estado = 7;
					} else {
						faltan_datos = 1;
					}
					break;
				case 7:
					if (t->n >= t->plen - 40) {
						/* ok, listo */
						/* decode the lines */

						/* and reset the state machine */
						t->rp = t->data;
						t->estado = 8;
						t->n -= t->plen - 40;
						if (cb!=NULL) {
							cb(t, data);
							return;
						}
					} else {
						faltan_datos = 1;
					}
					break;
				case 8:
					t->bufptr = t->data;
					t->estado = 0;
					break;
			}	
		}
	}
	if (leidos==-1) {
		/*perror ("TXT Err");*/
	}

}

/* the teletext thread */
void *mydvb_txt_decoder (void *data) {
	MYDVB *mydvb = (MYDVB *) data;

	/* pthread_detach(pthread_self()); */

	while (mydvb->txt_decoding) {
		__mydvb_txt_reader (&mydvb->txt_buffer,
			mydvb->fd_txt,
			mydvb->txt_deco_callback,
			mydvb->txt_deco_data);
	}

	pthread_exit(NULL);
	return NULL;

}