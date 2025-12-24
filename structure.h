#ifndef _STRUCTURE_H
#define _STRUCTURE_H

enum blocktype {
	BLOCK_CONTAINER_DOCUMENT,

	BLOCK_CONTAINER_ORDERED_LIST_ITEM,
	BLOCK_CONTAINER_QUOTE,
	BLOCK_CONTAINER_UNORDERED_LIST_ITEM,

	BLOCK_LEAF_CODE_BLOCK,
	BLOCK_LEAF_HEADER_1,
	BLOCK_LEAF_HEADER_2,
	BLOCK_LEAF_PARAGRAPH,
	BLOCK_LEAF_THEMATIC_BREAK,
};

struct block {
	enum blocktype type;

	/* It is 0 if the block is open, or any non-0 integer if it is
	   closed. */
	unsigned int closed;

	/* The text content of the block. */
	char *content;

	/* The pointer to the next sibling. */
	struct block *sibling;

	/* The pointer to the first child. */
	struct block *children;
};

#endif
