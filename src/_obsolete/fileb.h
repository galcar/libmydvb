#ifndef __FILE_B_H__
#define __FILE_B_H__

#include <sys/types.h>
#include <unistd.h>

#define FB_MODE_NORMAL	1
#define FB_MODE_TAIL	2

typedef struct _file_buffer {
	const char *fname;
	int fd_read;
	int fd_write;
	
	int mode;

	off_t capacity;	// capacidad en modo TAIL
	off_t wp;	// puntero de escritura
	off_t rp; 	// puntero de lectura

	int cflag;	// flag de vuelta completada por puntero:
			// la escritura lo pone a 1, la lectura lo pone a 0 

} file_buffer;

int fileb_open (file_buffer *fb, const char *name);
void fileb_close (file_buffer *fb);

void fileb_set_mode (file_buffer *fb, int mode);
int fileb_get_mode (file_buffer *fb);

ssize_t fileb_write (file_buffer *fb, const unsigned char *buffer, size_t len);
ssize_t fileb_read (file_buffer *fb, unsigned char *buffer, size_t len);

#endif
