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
  Perm *perm = calloc(1, sizeof(Perm));

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

Perm *
new_perm_from_value(char *target) {
  /* N of bits (12 for special, 9 for normal) */
  int bitn;
  if (specialp) bitn = 12;
  else          bitn = 9;
  
  /* calloc will set mem to 0 */
  Perm *perm = calloc(1, sizeof(Perm));
  char *endptr;
  long numeric = strtol(target, &endptr, 8);

  /* Exit if ``NUMERIC'' is negative or is longer than ``BITN'' bits  */
  if (numeric < 0 || (numeric >> bitn) != 0) 
    ERR(1, EINVAL, "Incorrect numeric notation");

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

  else ERR(1, EINVAL, "%s", target);

  perm->initialized = TRUE;

  return perm;
} /* End of new_perm_from_value() */

