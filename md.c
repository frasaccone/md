#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "parse.h"
#include "structure.h"

void usage(void);

static struct block document = { 0 };

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
	char line[LINE_MAX];

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
			fprintf(stderr, "Unable to open %s.\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	document.type = BLOCK_CONTAINER_DOCUMENT;

	while (fgets(line, sizeof(line), file))
		parseline(&document, line);

	if (file != stdin)
		fclose(file);

	return EXIT_SUCCESS;
}
