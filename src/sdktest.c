#include <proto/dos.h>
#include <dos/dos.h>
#include <string.h>
#include <stdio.h>
int main(void) 
{
  struct RDArgs *rd;
  char template[50], out[100];
  LONG params[5];
  int c;

  strcpy(template, "CMD,ARG1,ARG2,ARG3,ARG4");
  for (c=0; c<5; c++) {
    params[c] = 0;
  }

  rd = IDOS->ReadArgs(template, params, NULL);
  if (rd) {
    if ((params[0])) {
      sprintf(out, "CMD is %s\n", (STRPTR)params[0]);
      IDOS->PutStr(out);
    }
    if ((params[1]))
     IDOS->Printf("ARG1 options specified %s.\n", params[1]);
    if ((params[2]))
     IDOS->PutStr("ARG2 option specified.\n");
    if ((params[3]))
     IDOS->PutStr("ARG3 options specified.\n");
    if ((params[4]))
     IDOS->PutStr("ARG4 options specified.\n");
    IDOS->FreeArgs(rd);
  }
  else {
    IDOS->PutStr("poop\n");
  }
}
