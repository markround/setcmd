#include <proto/dos.h>
#include "utility.h"

// Used by the version DOS command
const char __ver[40] =  "$VER: SetCmd stub " SETCMD_VERSION;

int main (int argc, char const *argv[]) 
{
  char *cmd = (char *)argv[0];
  IDOS->Printf("SetCmd " SETCMD_VERSION " " SETCMD_BANNER "\n");
  IDOS->Printf("\n");
  IDOS->Printf("This command " FG_BLUE "%s " FG_BLACK "has been installed as a setcmd stub.\n", argv[0]);
  IDOS->Printf("You now need to add a version and set it. See the setcmd documentation or\n");
  IDOS->Printf("built-in help for more details (run " FG_BLUE "setcmd" FG_BLACK " with no arguments).\n");
}
