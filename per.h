/* 
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#ifndef PER_H
#define PER_H

/* errc on *BSD, error on GNU/Linux */
#if defined(__linux__)
  #include <error.h>
  #define ERR(eval, fmt, ...) error(eval, fmt, __VA_ARGS__)
#elif defined(__unix__) || defined(__APPLE__)
  #include <err.h>
  #define ERR(eval, fmt, ...) errc(eval, fmt, __VA_ARGS__)
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

/* Permision from the arg is converted into this struct */
typedef struct __perm {
  uint16_t    numeric;
  char       *symbolic; 
  _Bool       initialized;
} Perm;

/* Should we treat notations as special? */
extern _Bool specialp;

/* Function Prototypes */
/* per.c */
Perm       *new_perm_from_value    (char *target);

/* misc.c */
int         expt                   (int x, unsigned int exp);
_Bool       symbolicp              (char *str);

/* prints.c */
void        print_numeric          (Perm *perm);
void        print_symbolic         (Perm *perm);
void        print_verbose          (Perm *perm);
void        usage                  ();

/* conv.c */
char       *numeric_to_symbolic    (uint16_t num);
uint16_t    symbolic_to_numeric    (char* str);
char       *numeric_to_verbose     (unsigned int octal, char* read_str, 
                                    char* write_str,    char* exec_str);
/* You can jump into function definitions by regex ^function_name */

/* End of Function Prototypes */


#endif
