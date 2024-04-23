
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include "util.h"

unsigned int getbits(const unsigned char *buffer, unsigned int bitpos, unsigned int bitcount) {
    unsigned int i;
    unsigned int val = 0;

    for (i = bitpos; i < bitcount + bitpos; i++) {
      val = val << 1;
      val = val + ((buffer[i >> 3] & (0x80 >> (i & 7))) ? 1 : 0);
    }
    return val;
}


char *tokenizer (char *p, const char *delim) {
	static char *base_pointer;
	char *aux = NULL;
    char *r_value = NULL;

    if (p!=NULL) {
		base_pointer = p;
	}

	if (base_pointer==NULL) {
		return NULL;
	}

	r_value = base_pointer;
	aux = strstr (base_pointer, delim);
	if (aux==NULL) {
		base_pointer = NULL;
	} else {
		base_pointer = aux + strlen (delim);
		*aux=0;
    }

	return r_value;
}

/**
 * return current time in ms
 */
long long current_timestamp_ms () {
    static struct timeval tv;

    gettimeofday(&tv, NULL); // get current time

    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000; // calculate milliseconds

    return ms;
}
