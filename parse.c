#include "parse.h"

#include <stdlib.h>

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
	(void)document;
	(void)line;
}
