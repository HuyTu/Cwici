/*
 * playing with strtol to convert digit characters into int values
 *
 * C used to use atoi() for this, but atoi is deprecated (see man atoi)
 *
 * Updated version from Tobin MacDermott who found errors with original.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void convert(char *);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    // user didn't give us a string to try, do a few of our own...
    convert("1234");
    convert("  1234");
    convert("  1234  ");
    convert("  1234xxx");
    convert("0");
    convert("xyz");
    convert("    xyz");
    convert("0");
    convert("-1");
  }
  else
    for (int i=1; i < argc; i++) {
      convert(argv[i]);
    }
}

void convert(char *s) {
  // setup another pointer to the begining of the string
  char * sEnd = s;
  // strtol will move the second pointer as it
  // parses characters
  int val = (int) strtol(s, &sEnd, 10);
  
  printf("strtol: %s converted to %d\n", s, val);
  if (val == 0) {
    printf("\t0 is special\n");
    if ( sEnd == s )
      printf("\tstrtol did not parse any characters\n");
    else 
      printf("\tthis 0 actually means 0\n");
  }
}
