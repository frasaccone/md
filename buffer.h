#ifndef _BUFFER_H
#define _BUFFER_H

#include <stddef.h>

/* It reallocates the buf buffer of length buflen to concatenate the new string
   of length newlen at its end. Lengths are to be calculated without the NULL
   character. If buf is NULL, additional memory is allocated and the new string
   is copied to it.

   In case of error, perror is called and NULL is returned; otherwise, the
   reallocated buffer is returned. */
char *addtobuffer(char *buf, size_t buflen, char *new, size_t newlen);

#endif
