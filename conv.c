// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#include "per.h"
#include <ctype.h>

// Format access mode into a rwxrwxrwx string
char *
numeric_to_symbolic(uint16_t num) {
	char *buf = calloc(10, sizeof(char));

	// Revised Toybox lib/lib.c mode_to_string()
	for (int i = 0; i < 9; i++) {
		int bit = num & (1 << i), rwx = i % 3;
		if (!rwx && (num & (1 << ((i / 3) + 9)))) {
			buf[8-i] = "tss"[i/3];
			if (!bit) buf[8-i] &= ~0x20;
		} else buf[8-i] = bit ? "xwr"[rwx] : '-';
	}
	return buf;
} /* End of numeric_to_symbolic() */

uint16_t
symbolic_to_numeric(char *str) {
	uint16_t numeric = 0;

	if (strlen(str) != 9) {
		usage();
		ERR(1, EINVAL, "%s", str);
	}

	for (unsigned int bit = 0; bit < 9; bit++) {
		char chars[] = "rwxsst";
		if (str[bit] != chars[bit % 3] && str[bit] != '-') {
			if ((bit + 1) % 3 != 0 || !specialp || tolower(str[bit]) != chars[3 + (bit / 3)]) {
				usage();
				ERR(1, EINVAL, "%s", str);
			}
		}

		if (str[bit] != '-') {
			if (strchr("stST", str[bit])) {
				numeric += 01000 * (1 << (2 - (bit/3)));
			}
			if (str[bit] != 'S' && str[bit] != 'T') {
				numeric += (1 << (3*(2 - (bit/3)))) * (1 << (2 - (bit%3)));
			}
		}
	}
	
	return numeric;
} /* End of symbolic_to_numeric() */

/* Converts 3 bits into symbolic */
char*
numeric_to_verbose(unsigned int octal, char* strs[]) {
	char* buff_str = calloc(22, sizeof(char));

	for (int i = 04; i; i >>= 1) {
		if (octal & i) {
			if (*buff_str) strcat(buff_str, ", ");
			buff_str = strcat(buff_str, strs[2-(i>>1)]);
		}
	}

	return buff_str;
} /* End of numeric_to_verbose */

/* End of Function Definitions */
