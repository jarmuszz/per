// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#include "per.h"

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
