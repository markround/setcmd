#include <proto/dos.h>
#include "utility.h"

// Used by the version DOS command
const char __ver[40] =  "$VER: SetCmd stub " SC_VERSION;

int main (int argc, char const *argv[]) 
{
  char *cmd = argv[0];
  IDOS->Printf("SetCmd " SC_VERSION " " SC_BANNER "\n");
  IDOS->Printf("\n");
  IDOS->Printf("This command " FG_BLUE "%s " FG_BLACK "has been installed as a setcmd stub.\n", argv[0]);
  IDOS->Printf("You now need to add a version and set it. See the setcmd documentation or\n");
  IDOS->Printf("built-in help for more details (run setcmd with no arguments).\n");
}
