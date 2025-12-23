#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE   700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "structure.h"

void usage(void);

static struct block document = {
	.type = CONTAINER_BLOCK_DOCUMENT,
};

void
usage(void)
{
	fprintf(stderr, "Usage: %s [-h | -t] [file]\n", argv0);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	int hflag = 0,
	    tflag = 0;
	FILE *file = stdin;

	ARGBEGIN {
	case 'h':
		hflag = 1;
		break;
	case 't':
		tflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (hflag + tflag > 1)
		usage();

	if (argc > 1) {
		usage();
	} else if (argc == 1 && strcmp(argv[0], "-")) {
		file = fopen(argv[0], "rb");

		if (!file) {
			fprintf(stderr, "Unable to open %s.", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (file != stdin)
		fclose(file);

	return EXIT_SUCCESS;
}
