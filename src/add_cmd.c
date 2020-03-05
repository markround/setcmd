#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int add_cmd(char *cmd) 
{
  IDOS->Printf("Adding command\n");
  return RETURN_OK;
}
