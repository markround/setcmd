#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

int main()
{
 
  char foo[MAX_PATH_BUF]="Hello world";
  

  printf("1: %s\n", foo);
  printf("2: %s\n", SETCMD_STUB);

  printf("1: %s\n", (STRPTR)foo);
  printf("2: %s\n", (LONG)SETCMD_STUB);


  return 0;
}
