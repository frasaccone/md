#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* It reallocates the content buffer of block b to concatenate the content
   string at its end. If the block b has an unallocated content buffer, it is
   allocated and the content string is copied to it. */
void addcontent(struct block *b, char *content);

/* It returns the last open child of block b. If b has no open children, NULL
   is returned instead. */
struct block *lastopenchild(struct block *b);

void
addcontent(struct block *b, char *content)
{
	size_t origsize, newsize;

	/* Calculate the new extended size, i.e. the original content buffer
	   size plus the length of the passed content plus \0. */
	origsize = b->content ? strlen(b->content) : 0;
	newsize = origsize + strlen(content) + 1;

	/* Reallocate the content buffer with the new extended size. If the
	   content is NULL, realloc just works like a normal malloc. */
	if (!(b->content = realloc(b->content, newsize))) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}

	/* Copy content to the new bytes of the reallocated content buffer. */
	strncpy((char *)((size_t)b->content + origsize), content,
	        strlen(content));
}

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

	addcontent(lastopen, line);
}
