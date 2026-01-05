#include "mul.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(a) (unsigned int)(sizeof(a)/sizeof(*a))

struct mulnodestackel {
	struct mulnode *n;
	int l;
};

/* A parser function takes a pointer res to the resulting struct mulnode to
   allocate, a pointer buf to the buffer to parse and its size, buflen.

   -1 is returned in case of error. 0 is returned and res is not set if no new
   node is found. The number of characters which contain the mulnode res is set
   to is returned in case a new node is found. */
typedef long (*parser)(struct mulnode **res, char *buf, size_t buflen);

static long parseheader(struct mulnode **res, char *buf, size_t buflen);

static parser parsers[1] = {
	parseheader,
};

long
parseheader(struct mulnode **res, char *buf, size_t buflen)
{
	size_t l, ul1, ul2, ul3;
	char *ustart;

	/* Set l to the length of the possibly underlined content. It must be
	   in a single line. */
	for (l = 0; buf[l] != '\n' && l < buflen; l++);

	if (!l)
		return 0;

	/* Set ustart to the start character of the possible underline. It
	   skips the newline character present between the content and the
	   underline. */
	ustart = &buf[l + 1];

	/* Set ul1 to the length of the possible "=" underline. */
	for (ul1 = 0; l + ul1 < buflen && ustart[ul1] == '='; ul1++);

	/* Set ul2 to the length of the possible "-" underline. */
	for (ul2 = 0; l + ul2 < buflen && ustart[ul2] == '-'; ul2++);

	/* Set ul3 to the length of the possible "." underline. */
	for (ul3 = 0; l + ul3 < buflen && ustart[ul3] == '.'; ul3++);

	if (l != ul1 && l != ul2 && l != ul3)
		return 0;

	if (!(*res = malloc(sizeof(struct mulnode)))) {
		perror("malloc");
		return -1;
	}
	memset(*res, 0, sizeof(struct mulnode));

	(*res)->closed = 1;
	(*res)->content = buf;
	(*res)->contentsize = l;
	(*res)->content[(*res)->contentsize] = '\0';

	if (l < ul1)
		(*res)->type = MUL_NODE_HEADER_1;
	else if (l < ul2)
		(*res)->type = MUL_NODE_HEADER_2;
	else if (l < ul3)
		(*res)->type = MUL_NODE_HEADER_3;
	else
		return 0; /* Unreachable. */

	/* 1 is added to l to take the newline characters into account. */
	return 2 * (l + 1);
}

struct mulnode *
muldocument(void)
{
	struct mulnode *d;

	if (!(d = malloc(sizeof(struct mulnode)))) {
		perror("malloc");
		return NULL;
	}

	memset(d, 0, sizeof(struct mulnode));
	d->type = MUL_NODE_DOCUMENT;

	return d;
}

int
mulparse(struct mulnode *document, char *buf, size_t buflen)
{
	size_t off;
	struct mulnode *last = document;

	off = 0;
	while (off < buflen) {
		unsigned int i;
		long l;
		struct mulnode *new;

		/* Set l to the number of affected characters by any of the
		   parsers. It follows the order of the parsers array. */
		for (i = 0; i < LEN(parsers); i++) {
			if ((l = parsers[i](&new,
			                    &buf[off],
			                    buflen - off)) == -1)
				return -1;
			if (l)
				break;
		}

		/* Add the number of affected characters to the offset. */
		off += l;

		if (!l) {
			/* If no new node has been created, just copy the
			   character to the content of the last open node. */
			if (off < buflen) {
				last->content = realloc(last->content,
				                        last->contentsize + 1);
				if (!last->content) {
					perror("realloc");
					return -1;
				}
				last->content[last->contentsize] = buf[off];
				last->contentsize++;
				last->content[last->contentsize] = '\0';
				off++;
			}
		} else {
			/* Make the new node a child of the last open node. */
			if (!last->children) {
				last->children = new;
			} else {
				struct mulnode *tail;
				for (tail = last->children;
				     tail->sibling;
				     tail = tail->sibling);

				tail->sibling = new;
			}

			new->parent = last;

			/* If still open, make new the last open node. */
			if (!new->closed)
				last = new;
		}

		/* In case of a double newline, close the last node and set
		   its parent as the last open node. This does not happen
		   when the last node is open or is the document. */
		if (last != document && !last->closed
		                     && off < buflen - 1
		                     && buf[off] == '\n'
		                     && buf[off + 1] == '\n') {
			last->closed = 1;
			last = last->parent;
		}
	}

	return 0;
}

int
multree(struct mulnode *document)
{
	struct mulnodestackel *stack;
	size_t stackcap, stacksize;

	if (!document)
		return 0;

	stackcap = 1;

	if (!(stack = malloc(stackcap * sizeof(struct mulnodestackel)))) {
		perror("malloc");
		return -1;
	}

	stacksize = 0;

	stack[stacksize].n = document;
	stack[stacksize].l = 0;

	stacksize++;

	while (stacksize > 0) {
		struct mulnode *cur, *child;
		int l, i;

		stacksize--;

		cur = stack[stacksize].n;
		l = stack[stacksize].l;

		for (i = 0; i < l; i++)
			printf("\t");

		printf("[");

		switch (cur->type) {
		case MUL_NODE_DOCUMENT:
			printf("Document");
			break;
		case MUL_NODE_ORDERED_LIST_ITEM:
			printf("Ordered list item");
			break;
		case MUL_NODE_BLOCK_QUOTE:
			printf("Block quote");
			break;
		case MUL_NODE_UNORDERED_LIST_ITEM:
			printf("Unordered list item");
			break;
		case MUL_NODE_CODE_BLOCK:
			printf("Code block");
			break;
		case MUL_NODE_HEADER_1:
			printf("Header 1");
			break;
		case MUL_NODE_HEADER_2:
			printf("Header 2");
			break;
		case MUL_NODE_PARAGRAPH:
			printf("Paragraph");
			break;
		case MUL_NODE_THEMATIC_BREAK:
			printf("Thematic break");
			break;
		default:
			break;
		}

		printf("] (%ld)\n", cur->contentsize);

		if (cur->content) {
			for (i = 0; i < l; i++)
				printf("\t");

			printf("%s\n", cur->content);
		}

		for (child = cur->children; child; child = child->sibling) {
			if (stacksize >= stackcap) {
				size_t newsz;

				/* Everytime new memory is needed, the capacity
				   of the stack is doubled. This makes the
				   program call realloc less, reducing the
				   overhead. */
				stackcap *= 2;
				newsz = stackcap
				        * sizeof(struct mulnodestackel);

				if (!(stack = realloc(stack, newsz))) {
					perror("realloc");
					return -1;
				}
			}

			stack[stacksize].n = child;
			stack[stacksize].l = l + 1;

			stacksize++;
		}
	}

	free(stack);

	return 0;
}
