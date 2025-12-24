#ifndef _STRUCTURE_H
#define _STRUCTURE_H

enum blocktype {
	CONTAINER_BLOCK_DOCUMENT,
	CONTAINER_BLOCK_LIST_ITEM,
	CONTAINER_BLOCK_ORDERED_LIST,
	CONTAINER_BLOCK_QUOTE,
	CONTAINER_BLOCK_UNORDERED_LIST,

	LEAF_BLOCK_CODE_BLOCK,
	LEAF_BLOCK_HEADER_1,
	LEAF_BLOCK_HEADER_2,
	LEAF_BLOCK_PARAGRAPH,
	LEAF_BLOCK_THEMATIC_BREAK,
};

struct block {
	enum blocktype type;

	/* The text content of the block. */
	char *content;

	/* The pointer to the next sibling. */
	struct block *sibling;

	/* The pointer to the first child. */
	struct block *children;
};

#endif
