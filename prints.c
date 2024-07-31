// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */
#include <stdio.h>

#include "per.h"

char* regular_pnames[] = {"read", "write", "execute"};
char* special_pnames[] = {"suid", "sgid", "sticky"};

/* Printing Functions */
void
print_verbose(Perm *perm) {
	printf("user: %s\n", numeric_to_verbose((perm->numeric & 0700) >> 6, regular_pnames));
	printf("group: %s\n", numeric_to_verbose((perm->numeric & 0070) >> 3, regular_pnames));
	printf("other: %s\n", numeric_to_verbose((perm->numeric & 0007), regular_pnames));
	
	if (specialp) {
		printf("special: %s\n", numeric_to_verbose((perm->numeric & 07000) >> 9, special_pnames));
	}
	
} /* End of print_verbose */


void
usage() {
	fprintf(stderr, "usage: per [-nSsv] [PATH | SYMBOLIC | NUMERIC | -]\n");
} /* End of usage() */


/* End of Printing Functions */
