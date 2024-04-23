#ifndef __UTIL_H__
#define __UTIL_H__

unsigned int getbits(const unsigned char *buffer, unsigned int bitpos, unsigned int bitcount);

char *tokenizer (char *p, const char *delim);

long long current_timestamp_ms ();

#endif
