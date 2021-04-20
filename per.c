/* 
 * per - Simple unix permission viewer and converter
 *
 * This program is licensed under GPL version 3.
 * Copyright is a theft.
 * For more info see the LICENSE file or reffer to ``https://www.gnu.org/licenses/gpl-3.0.html''.
 */

#if defined(__linux__)
#include <error.h>
#define ERR(eval, fmt, ...) error(eval, fmt, __VA_ARGS__)
#elif defined(__unix__)
#include <err.h>
#define ERR(eval, fmt, ...) errc(eval, fmt, __VA_ARGS__)
#endif

#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

/* Permision from the arg is
 * converted into this struct */
typedef struct __perm {
  uint16_t    numeric;
  char       *symbolic; 
  _Bool       initialized;
} Perm;

/* Should we treat notations as special? */
_Bool specialp = FALSE;

/* Function Prototypes */
void        usage                  ();
int         expt                   (int x, unsigned int exp);
_Bool       symbolicp              (char *str);

void        print_numeric          (Perm *perm);
void        print_symbolic         (Perm *perm);
void        print_verbose          (Perm *perm);

Perm       *new_perm_from_value    (char *target);
char       *numeric_to_symbolic    (uint16_t num);
uint16_t    symbolic_to_numeric    (char* str);
char       *numeric_to_verbose     (unsigned int octal, char* read_str, char* write_str, char* exec_str);
/* You can jump into function definitions by regex ^function_name */

/* End of Function Prototypes */


/* Function Definitions */

int
main(int argc, char **argv) {

  /* Exit if no args */
  if (argc == 1) {
    usage();
    exit(1);
  }

  /* If only one arg is supplied
   * respawn with `-vns' */
  if (argc == 2) {
    char* argv_respawn[] = {argv[0], "-vns", argv[1], NULL };
    execvp(argv[0], argv_respawn);
  }

  /* The same as above but runs when
   * only `-S' is passed */
  if (argc == 3 && (strcmp(argv[1], "-S")) == 0) {
    char* argv_respawn[] = {argv[0], "-Svns", argv[2], NULL };
    execvp(argv[0], argv_respawn);
  }

  /* Allocate space for permissions converted into a struct */
  Perm * perm = malloc(sizeof(Perm));

  /* Switch options */
  int option;
  while ((option = getopt(argc, argv, "Snsvh")) != -1) {
    switch (option) {
      case 'S':
        specialp = TRUE;
        break;
      case 'n':
        if (!perm->initialized) perm = new_perm_from_value(argv[argc-1]);
        print_numeric(perm);
        break;
      case 's':
        if (!perm->initialized) perm = new_perm_from_value(argv[argc-1]);
        print_symbolic(perm);
        break;
      case 'v':
        if (!perm->initialized) perm = new_perm_from_value(argv[argc-1]);
        print_verbose(perm);
        break;
      case 'h':
        usage();
        exit(0);
        break;
      default:
        usage();
        exit(1);
        break;
    }
  }

  return (0);
} /* End of main() */

void
usage() {
  fprintf(stderr, "usage: per [-S] [-v] [-n] [-s] [PATH | SYMBOLIC | NUMERIC]\n");
} /* End of usage() */


int
expt(int x, unsigned int exp) {
  /* This implementation is lacking but there's
   * no need for more. */
  int res = 1;
  for ( ; exp != 0; exp--) res *= x;
  
  return res;
} /* End of expt() */

/* Printing Functions */
void
print_numeric(Perm *perm) { printf("%o\n", perm->numeric); }

void
print_symbolic(Perm *perm) { printf("%s\n", perm->symbolic); }

void
print_verbose(Perm *perm) {
  printf("user: %s\n", numeric_to_verbose((perm->numeric & 0700) >> 6, NULL, NULL, NULL));
  printf("group: %s\n", numeric_to_verbose((perm->numeric & 0070) >> 3, NULL, NULL, NULL));
  printf("other: %s\n", numeric_to_verbose((perm->numeric & 0007), NULL, NULL, NULL));
  
  if (specialp) {
    printf("special: %s\n", numeric_to_verbose((perm->numeric & 07000) >> 9, "suid", "sgid", "sticky"));
  }
  
} /* End of print_verbose */

/* End of Printing Functions */

_Bool
symbolicp(char *str) {
  char chars[] = "rwx";
  char chars_spec[] = "sst";
  char chars_spec_upp[] = "SST";

  if (strlen(str) > 9) ERR(1, EINVAL, str);

  if (specialp) {
    for (unsigned int i = 0; i <= strlen(str)-1; i++) {
      if (str[i] != chars[i % 3] && str[i] != '-') {
        if ((i + 1) % 3 == 0) {
          if (str[i] != chars_spec[i / 3] &&
              str[i] != chars_spec_upp[i / 3]) {
            return (FALSE);
          }
        } 
        else return (FALSE);
      }
    }
  }
  else {
    for (unsigned int i = 0; i <= strlen(str)-1; i++) {
      if (str[i] != chars[i % 3] && str[i] != '-') {
        return (FALSE);
      }
    }
  }


  return (TRUE);
} /* End of symbolicp() */

Perm *
new_perm_from_value(char *target) {
  /* N of bits (12 for special, 9 for normal) */
  int bitn;
  if (specialp) bitn = 12;
  else          bitn = 9;
  
  Perm *perm = malloc(sizeof(Perm));
  char *endptr;
  long numeric = strtol(target, &endptr, 8);

  /* Exit if ``NUMERIC'' is negative or is longer than ``BITN'' bits  */
  if (numeric < 0 || (numeric >> bitn) != 0) 
    ERR(1, EINVAL, "%lo", numeric);

  /* Checking if there weren't any strings in ``TARGET''
     i.e. runs if ``TARGET'' is a number. */
  if (*endptr == '\0') {
    perm->numeric = (uint16_t) numeric;
    perm->symbolic = numeric_to_symbolic((uint16_t) numeric);
  }

  /* Runs if ``TARGET'' is a valid path */
  else if (!access(target, F_OK)) {
    struct stat statbuf;
    stat(target, &statbuf);
    numeric = statbuf.st_mode & (S_IRWXU + S_IRWXG + S_IRWXO);
    if (specialp) numeric += statbuf.st_mode & 07000;

    perm->numeric = (uint16_t) numeric;
    perm->symbolic = numeric_to_symbolic((uint16_t) numeric);
  }

  /* Runs if TARGET is a valid symbolic notation */
  else if (symbolicp(target)) {
    perm->numeric = symbolic_to_numeric(target);
    perm->symbolic = target;
  }

  else ERR(1, EINVAL, target);

  perm->initialized = TRUE;

  return perm;
} /* End of new_perm_from_value() */

char *
numeric_to_symbolic(uint16_t num) {
  char *symbolic = calloc(10, sizeof(char));
  const char chars[] = "rwxrwxrwx";
  const char chars_spec[] = "sstSST";
  uint8_t numeric_len = 9;

  for (unsigned int i = 0; i < numeric_len; i++) {
    symbolic[i] = (num & (1 << (8-i))) ? chars[i] : '-';
  }

  if (specialp) {
    for (unsigned int i = 0; i < 3; i++) {
      if (num & (1 << (11 - i))) {
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

char*
numeric_to_verbose(unsigned int octal, char* read_str, char* write_str, char* exec_str) {
  /* Converts 3 bits into symbolic */
  char* comma_str = ", ";
  if (!read_str)   read_str = "read";
  if (!write_str)  write_str = "write";
  if (!exec_str)   exec_str = "execute";

  char* buff_str = calloc(22, sizeof(char));

  /* Bad but works */
  if (octal & 04) strcpy(buff_str, read_str);
  if (octal & 02) {
    if (buff_str[0] != '\0')
      buff_str = strcat(buff_str, comma_str);
    buff_str = strcat(buff_str, write_str);
  }
  if (octal & 01) {
    if (buff_str[0] != '\0')
      buff_str = strcat(buff_str, comma_str);
    buff_str = strcat(buff_str, exec_str);
  }

  return buff_str;
} /* End of numeric_to_verbose */

/* End of Function Definitions */
