// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#include "per.h"

_Bool
symbolicp(char *str) {
	char chars[] = "rwx";
	char chars_spec[] = "sst";
	char chars_spec_upp[] = "SST";

	if (strlen(str) != 9) return (FALSE);

	for (unsigned int i = 0; i <= strlen(str)-1; i++) {
		if (str[i] != chars[i % 3] && str[i] != '-') {
			if (((i + 1) % 3 != 0 || !specialp) ||
			(str[i] != chars_spec[i / 3] && str[i] != chars_spec_upp[i / 3]))
				return (FALSE);
		}
	}

	return (TRUE);
} /* End of symbolicp() */
