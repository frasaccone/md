#include "mul.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

/* It returns the last open child of node b. If b has no open children, NULL
   is returned instead. */
static struct mulnode *lastopenchild(struct mulnode *n);

struct mulnode *
lastopenchild(struct mulnode *n)
{
	struct mulnode *c, *lastopen = NULL;

	for (c = n->children; c && c->sibling; c = c->sibling) {
		if (!c->closed)
			lastopen = c;
	}

	return lastopen;
}

struct mulnode *
muldocument(void)
{
	struct mulnode *d;

	if (!(d = malloc(sizeof(struct mulnode)))) {
		perror("malloc");
		return NULL;
	}

	d->type = MUL_NODE_DOCUMENT;

	return d;
}

void
parsebuffer(struct mulnode *document, char *buf, size_t buflen)
{
	struct mulnode *c, *lastopen;

	/* Make lastopen the deepest last open child of document, or make it
	   document if it has no open children. */
	lastopen = document;
	while ((c = lastopenchild(lastopen)))
		lastopen = c;

	if (!(lastopen->content = addtobuffer(lastopen->content,
	                                      lastopen->content
	                                      ? strlen(lastopen->content)
	                                      : 0,
	                                      buf,
	                                      strnlen(buf, buflen))))
		exit(EXIT_FAILURE);
}
