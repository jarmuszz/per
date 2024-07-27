// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#include "per.h"

char *
numeric_to_symbolic(uint16_t num) {
	char *symbolic = calloc(10, sizeof(char));
	const char chars[] = "rwxrwxrwx";
	const char chars_spec[] = "sstSST";
	uint8_t numeric_len = 9;

	/*
	 * Checks num bit by bit from left. If bit nth is 1 then fill the nth place
	 * in `symbolic' with nth char from `chars[]', otherwise fill it with a '-'.
	 */
	for (unsigned int i = 0; i < numeric_len; i++) {
		symbolic[i] = (num & (1 << (8-i))) ? chars[i] : '-';
	}

	if (specialp) {
		/* Similar to above, but checks 3 bits from the left */
		for (unsigned int i = 0; i < 3; i++) {
			/* Check if nth bit frm the beginning is 1 */
			if (num & (1 << (11 - i))) {
				/*
				 * Check if corresponding bit from non-special notation is empty
				 * and apply uppercase or lowercase char appropriately.
				 */
				if (num & (1 << (8 - ((i + 1) * 3) + 1)))
					symbolic[((i + 1) * 3) - 1] = chars_spec[i];
				else
					symbolic[((i + 1) * 3) - 1] = chars_spec[i + 3];
			}
		}
	}
	
	symbolic[9] = '\0';

	return (symbolic);
} /* End of numeric_to_symbolic() */

uint16_t
symbolic_to_numeric(char *str) {
	char special[] = "stST";

	uint16_t numeric = 0;
	for (unsigned int field = 0; field < 3; field++) {
		for (unsigned int bit = 0; bit < 3; bit++) {
			if (str[field * 3 + bit] != '-') {
				if (specialp) {
					for (unsigned int i = 0; i < LEN(special); i++) {
						if (str[field * 3 + bit] == special[i]) {
							numeric += 01000 * (1 << (2 - field));
							break;
						}
					}
					if (str[field * 3 + bit] != 'S' &&
							str[field * 3 + bit] != 'T') {
						numeric += expt(8, 2 - field) * (1 << (2 - bit));
					}
				}
				else numeric += expt(8, 2 - field) * (1 << (2 - bit));
			}
		}
	}
	
	return (numeric);
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
