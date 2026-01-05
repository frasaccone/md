#include "mul.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mulnodestackel {
	struct mulnode *n;
	int l;
};

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
	if (!(document->content = realloc(document->content,
	                                  document->contentsize + buflen))) {
		perror("realloc");
		return -1;
	}

	memcpy((char *)((size_t)document->content + document->contentsize),
	       buf, buflen);

	document->contentsize += buflen;
	document->content[document->contentsize] = '\0';

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
