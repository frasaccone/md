#include "mul.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

struct mulnodestackel {
	struct mulnode *n;
	int l;
};

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

int
mulparse(struct mulnode *document, char *buf, size_t buflen)
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
		return -1;

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

		printf("]\n");

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
