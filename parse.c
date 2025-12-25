#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

/* It returns the last open child of block b. If b has no open children, NULL
   is returned instead. */
static struct block *lastopenchild(struct block *b);

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

	/* Make lastopen the deepest last open child of document, or make it
	   document if it has no open children. */
	lastopen = document;
	while ((c = lastopenchild(lastopen)))
		lastopen = c;

	lastopen->content = addtobuffer(lastopen->content,
	                                lastopen->content
	                                ? strlen(lastopen->content)
	                                : 0,
	                                line,
	                                strlen(line));
}
