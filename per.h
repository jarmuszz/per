// vim: ts=2 shiftwidth=2 noexpandtab

/*
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#ifndef PER_H
#define PER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#define TRUE 1
#define FALSE 0

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define ERR(eval, fmt, ...) do { errno = fmt; err(eval, __VA_ARGS__); } while (0)

/* Permision from the arg is converted into this struct */
typedef struct __perm {
	uint16_t		numeric;
	char			*symbolic;
	_Bool			initialized;
} Perm;

/* Should we treat notations as special? */
extern _Bool specialp;

/* Function Prototypes */
/* per.c */
Perm			 *new_perm_from_value		 (char *target);

/* misc.c */
_Bool				symbolicp							 (char *str);

/* prints.c */
void				print_numeric					 (Perm *perm);
void				print_symbolic				 (Perm *perm);
void				print_verbose					 (Perm *perm);
void				usage									 ();

/* conv.c */
char			 *numeric_to_symbolic		 (uint16_t num);
uint16_t		symbolic_to_numeric		 (char* str);
char			 *numeric_to_verbose		 (unsigned int octal, char* strs[]);
/* You can jump into function definitions by regex ^function_name */

/* End of Function Prototypes */

#endif
