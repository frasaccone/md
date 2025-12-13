#include <stdio.h>
#include <stdlib.h>

#include "arg.h"

void usage(void);

void
usage(void)
{
	fprintf(stderr, "Usage: %s [file]\n", argv0);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND

	return 0;
}
