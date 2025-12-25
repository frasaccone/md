#ifndef _BUFFER_H
#define _BUFFER_H

#include <stddef.h>

/* It reallocates the buf buffer of length buflen to concatenate the new string
   of length newlen at its end. Lengths are to be calculated without the NULL
   character.

   In case of error, 0 is returned; otherwise, the size of the reallocated
   buffer is returned. */
size_t addtobuffer(char **buf, size_t buflen, char *new, size_t newlen);

#endif
