// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "per.h"
_Bool specialp = FALSE;

/* Function Definitions */

int
main(int argc, char **argv) {
	if (argc == 1) {
		usage();
		exit(1);
	}

	/* If only one arg is supplied respawn with `-vns' */
	if (argc == 2) {
		if (strcmp(argv[1], "-h"))
		  execl(argv[0], argv[0], "-vns", argv[1], NULL);
		else { usage(); exit(0); }
	}

	/* The same as above but runs when only `-S' is passed */
	if (argc == 3 && (strcmp(argv[1], "-S")) == 0)
		execl(argv[0], argv[0], "-Svns", argv[2], NULL);

	Perm *perm = NULL;

	/* Cleaner syntax */
	char *target = argv[argc-1];

	/* If target is a `-', read target from stdin */
	if (strcmp(target, "-") == 0) {
		fscanf(stdin, "%s", target);
	}

	/* Switch options */
	int option;
	while ((option = getopt(argc, argv, "Snsvh")) != -1) {
		switch (option) {
			case 'S':
				specialp = TRUE;
				break;
			case 'n':
				if (!perm) perm = new_perm_from_value(target);
				printf("%0*o\n", specialp ? 4 : 3, perm->numeric);
				break;
			case 's':
				if (!perm) perm = new_perm_from_value(target);
				puts(perm->symbolic);
				break;
			case 'v':
				if (!perm) perm = new_perm_from_value(target);
				print_verbose(perm->numeric);
				break;
			case 'h':
				usage();
				exit(0);
			default:
				usage();
				exit(1);
		}
	}

	return (0);
} /* End of main() */

Perm *
new_perm_from_value(char *target) {
	Perm *perm = calloc(1, sizeof(Perm));
	char *endptr;
	long numeric = strtol(target, &endptr, 8);
	struct stat statbuf;

	/* Exit if ``NUMERIC'' is negative or is longer than ``BITN'' bits	*/
	if (numeric >> (specialp ? 12 : 9)) {
		usage();
		ERR(1, EINVAL, "Incorrect numeric notation");
	}

	/* Is ``TARGET'' a number? */
	if (*endptr == '\0') {
		perm->numeric  = numeric;
		perm->symbolic = numeric_to_symbolic(numeric);
	}
	/* Is ``TARGET'' a valid path? */
	else if (stat(target, &statbuf) != -1) {
		numeric = statbuf.st_mode & (0777 + (specialp ? 07000 : 0));
		perm->numeric  = numeric;
		perm->symbolic = numeric_to_symbolic(numeric);
	}
	/* Assuming ``TARGET'' is symbolic notation */
	else {
		perm->numeric = symbolic_to_numeric(target);
		perm->symbolic = target;
	}

	return perm;
} /* End of new_perm_from_value() */

