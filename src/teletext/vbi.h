#ifndef __VBI_H__
#define __VBI_H__

#define MYDVB_BUFFY	(188*100)

typedef struct _txt_buffer {
	unsigned char data[MYDVB_BUFFY];	/* datos */
	unsigned char *rp;					/* puntero de lectura */
	unsigned char *bufptr;				/* puntero de datos */


	int estado;
	int n;

	int data_id;
	int plen;
	
	void (*txt_deco_callback) (struct _txt_buffer *txt_buffer, void *data);
	void *txt_deco_data;
	

} TXT_BUFFER;

#endif