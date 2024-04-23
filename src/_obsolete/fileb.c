
#include "fileb.h"

#include <sys/stat.h>
#include <fcntl.h>

int fileb_open (file_buffer *fb, const char *name) {

	fb->fd_write = open (name, O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK);
	if (fb->fd_write == -1) {
		return -1;
	}

	fb->fd_read = open (name, O_RDONLY | O_NONBLOCK);
	if (fb->fd_read == -1) {
		return -1;
	}

	fb->mode = FB_MODE_NORMAL;
	fb->fname = name;
	fb->capacity = 0;
	fb->rp = fb->wp = 0;
	fb->cflag = 0;

	return 0;
}

void fileb_close (file_buffer *fb) {
	close (fb->fd_read);
	close (fb->fd_write);
}

void fileb_set_mode (file_buffer *fb, int mode) {

	fdatasync (fb->fd_write);

	if (mode==FB_MODE_TAIL) {
		fb->capacity = lseek (fb->fd_read, 0, SEEK_END);
		fb->rp = fb->wp = 0;
		fb->cflag = 1;
		lseek (fb->fd_read, fb->rp, SEEK_SET);
		lseek (fb->fd_write, fb->wp, SEEK_SET);
	}

	fb->mode = mode;

}

int fileb_get_mode (file_buffer *fb) {
	return fb->mode;
}

ssize_t fileb_write (file_buffer *fb, const unsigned char *buffer, size_t len) {

	ssize_t ss = 0;

	if (fb->mode==FB_MODE_NORMAL) {
		ss = write (fb->fd_write, buffer, len);
	} else if (fb->mode==FB_MODE_TAIL) {
		ss = write (fb->fd_write, buffer, len);
		fb->wp += len;
		if (fb->wp > fb->capacity) {
			fb->wp = 0;
			fb->cflag = 1;
			lseek (fb->fd_write, fb->wp, SEEK_SET);
		}
	}

	return ss;
}


ssize_t fileb_read (file_buffer *fb, unsigned char *buffer, size_t len) {

	ssize_t ss = 0;

	if (fb->mode==FB_MODE_NORMAL) {

		ss = read (fb->fd_read, buffer, len);

	} else if (fb->mode==FB_MODE_TAIL) {

		ss = read (fb->fd_read, buffer, len);
		fb->rp += len;
		if (fb->rp > fb->capacity) {
			fb->rp = 0;
			fb->cflag = 0;
			lseek (fb->fd_read, fb->rp, SEEK_SET);
		}
	}	

	return ss;
}
