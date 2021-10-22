#include <proto/dos.h>
#include <stdio.h>
#include "utility.h"

// Used by the version DOS command
const char __ver[40] =  "$VER: SetCmd stub " SETCMD_VERSION;

int main (int argc, char const *argv[]) 
{
  char *cmd = (char *)argv[0];
  printf("SetCmd " SETCMD_VERSION " " SETCMD_BANNER "\n");
  printf("\n");
  printf("This command " FG_BLUE "%s " FG_BLACK "has been installed as a setcmd stub.\n", argv[0]);
  printf("You now need to add a version and set it. See the setcmd documentation or\n");
  printf("built-in help for more details (run " FG_BLUE "setcmd" FG_BLACK " with no arguments).\n");
}
