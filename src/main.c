#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "init.h"

int main (int argc, char const *argv[]) 
{

  struct RDArgs *rd;
  char template[50];
  LONG params[5];
  int c;

  strcpy(template, "CMD,ARG1,ARG2,ARG3,ARG4");
  for (c=0; c<5; c++) {
    params[c] = 0;
  }

  rd = IDOS->ReadArgs(template, params, NULL);
  if (rd) {
    if (strcmp(params[0], "usage") == 0) {
      usage();
    }
    else if (strcmp(params[0], "init") == 0) {
      init((STRPTR)params[1]);
    }
    else {
      usage();
    }
    IDOS->FreeArgs(rd);
  }
  else {
    usage();
  }
  return 0;
}
