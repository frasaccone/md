#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "mul.h"
#include "structure.h"

void usage(void);

static struct mulnode document = { 0 };

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
	size_t filesize;
	char *filecontent;

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

	document.type = NODE_DOCUMENT;

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (!(filecontent = malloc(filesize + 1))) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	if (fread(filecontent, filesize, 1, file) < 1) {
		perror("fread");
		exit(EXIT_FAILURE);
	}

	if (file != stdin)
		fclose(file);

	parsebuffer(&document, filecontent, filesize);

	free(filecontent);

	return EXIT_SUCCESS;
}
