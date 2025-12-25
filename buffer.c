#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t
addtobuffer(char **buf, size_t buflen, char *new, size_t newlen)
{
	size_t origsize, newsize;

	origsize = *buf ? strnlen(*buf, buflen) : 0;
	newsize = origsize + strnlen(new, newlen) + 1;

	/* Reallocate the buf buffer with the new extended size. If it is NULL,
	   realloc just works like a normal malloc. */
	if (!(*buf = realloc(*buf, newsize))) {
		perror("realloc");
		return 0;
	}

	/* Copy new to the new bytes of the reallocated buf buffer. */
	strncpy((char *)((size_t)(*buf) + origsize), new,
	        strnlen(new, newlen));

	(*buf)[newsize] = '\0';

	return newsize;
}
