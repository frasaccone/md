#ifndef _MUL_H
#define _MUL_H

#include <stddef.h>

enum mulnodetype {
	NODE_DOCUMENT,

	NODE_ORDERED_LIST_ITEM,
	NODE_BLOCK_QUOTE,
	NODE_UNORDERED_LIST_ITEM,

	NODE_CODE_BLOCK,
	NODE_HEADER_1,
	NODE_HEADER_2,
	NODE_PARAGRAPH,
	NODE_THEMATIC_BREAK,
};

struct mulnode {
	enum mulnodetype type;

	/* It is 0 if the node is open, or any non-0 integer if it is
	   closed. */
	unsigned int closed;

	/* The text content of the node. */
	char *content;

	/* The pointer to the next sibling. */
	struct mulnode *sibling;

	/* The pointer to the first child. */
	struct mulnode *children;
};

/* It updates the document node after parsing the given buf buffer of length
   buflen. The length of the buffer must not consider the NULL character of the
   buffer. */
void parsebuffer(struct mulnode *document, char *buf, size_t buflen);

#endif
