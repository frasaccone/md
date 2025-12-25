#ifndef _MUL_H
#define _MUL_H

#include <stddef.h>

enum mulnodetype {
	MUL_NODE_DOCUMENT,

	MUL_NODE_ORDERED_LIST_ITEM,
	MUL_NODE_BLOCK_QUOTE,
	MUL_NODE_UNORDERED_LIST_ITEM,

	MUL_NODE_CODE_BLOCK,
	MUL_NODE_HEADER_1,
	MUL_NODE_HEADER_2,
	MUL_NODE_PARAGRAPH,
	MUL_NODE_THEMATIC_BREAK,
};

struct mulnode {
	enum mulnodetype type;

	/* It is 0 if the node is open, or any non-0 integer if it is
	   closed. */
	unsigned int closed;

	/* The text content of the node. */
	char *content;
	size_t contentsize;

	/* The pointer to the next sibling. */
	struct mulnode *sibling;

	/* The pointer to the first child. */
	struct mulnode *children;
};

/* It allocates and returns a document node. In case of error, NULL is returned
   instead. */
struct mulnode *muldocument(void);

/* It updates the document node after parsing the given buf buffer of length
   buflen. The length of the buffer must not consider the NULL character of the
   buffer.

   In case of error, -1 is returned; otherwise, 0 is returned. */
int mulparse(struct mulnode *document, char *buf, size_t buflen);

/* It prints the document tree structure to stdout.

   In case of error, -1 is returned; otherwise, 0 is returned. */
int multree(struct mulnode *document);

#endif
