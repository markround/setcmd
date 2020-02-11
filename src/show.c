#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int show(char *cmd) 
{
  IDOS->Printf("Showing\n");
  return RETURN_OK;
}
