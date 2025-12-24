#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* It returns the last open child of block b. If b has no open children, NULL
   is returned instead. */
struct block *lastopenchild(struct block *b);

struct block *
lastopenchild(struct block *b)
{
	struct block *c, *lastopen = NULL;

	for (c = b->children; c && c->sibling; c = c->sibling) {
		if (!c->closed)
			lastopen = c;
	}

	return lastopen;
}

void
parseline(struct block *document, char *line)
{
	struct block *c, *lastopen;
	size_t origsize, newsize;

	/* Make lastopen the deepest last open child of document, or make it
	   document if it has no open children. */
	lastopen = document;
	while ((c = lastopenchild(lastopen)))
		lastopen = c;

	/* Calculate the new extended size, i.e. the original content buffer
	   size plus the length of the line being parsed plus \0. */
	origsize = lastopen->content ? strlen(lastopen->content) : 0;
	newsize = origsize + strlen(line) + 1;

	/* Reallocate the content buffer with the new extended size. If the
	   content is NULL, realloc just works like a normal malloc. */
	if (!(lastopen->content = realloc(lastopen->content, newsize))) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}

	/* Copy line to the new bytes of the reallocated content buffer. */
	strncpy((char *)((size_t)lastopen->content + origsize), line,
	        strlen(line));
}
