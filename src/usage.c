#include <proto/dos.h>
#include "usage.h"
#include "utility.h"

void usage()
{
  IDOS->Printf("SetCmd " SC_VERSION " " SC_BANNER "\n");
  IDOS->Printf("\n");
  IDOS->Printf("Usage:\n");
  IDOS->Printf("\n");
  IDOS->Printf(BOLD_ON "init " BOLD_OFF FG_WHITE "quiet | verbose" FG_BLACK "\n");
  IDOS->Printf("  Sets up the SetCmd path for the current shell.\n");
  IDOS->Printf("  example: setcmd init verbose\n");

  IDOS->Printf(BOLD_ON "list " BOLD_OFF FG_WHITE "verbose" FG_BLACK "\n");
  IDOS->Printf("  List all commands managed by setcmd. The optional " FG_WHITE "verbose " FG_BLACK "argument shows additional detail.\n");
  IDOS->Printf("  example: setcmd list\n");


  IDOS->Printf(BOLD_ON "show " BOLD_OFF FG_BLUE "cmd" FG_BLACK "\n");
  IDOS->Printf("  Shows details for specified command " FG_BLUE "cmd" FG_BLACK "\n");
  IDOS->Printf("  example: setcmd show uae\n");

  IDOS->Printf(BOLD_ON "set " BOLD_OFF FG_BLUE "cmd version" FG_BLACK "\n");
  IDOS->Printf("  Sets " FG_BLUE "cmd " FG_BLACK "to " FG_BLUE "version" FG_BLACK "\n");
  IDOS->Printf("  example: setcmd set uae system\n");

  IDOS->Printf(BOLD_ON "add-cmd " BOLD_OFF FG_BLUE "cmd" FG_BLACK "\n");
  IDOS->Printf("  Adds a new command " FG_BLUE "cmd " FG_BLACK "and points it to a stub\n");
  IDOS->Printf("  example: setcmd create-cmd uae\n");

  IDOS->Printf(BOLD_ON "delete-cmd " BOLD_OFF FG_BLUE "cmd" FG_BLACK "\n");
  IDOS->Printf("  Removes command " FG_BLUE "cmd " FG_BLACK "from setcmd\n");
  IDOS->Printf("  example: setcmd delete-cmd uae\n");

  IDOS->Printf(BOLD_ON "add-version " BOLD_OFF FG_BLUE "cmd version target" FG_BLACK "\n");
  IDOS->Printf("  Adds a new version for " FG_BLUE "cmd" FG_BLACK ", named " FG_BLUE "version " FG_BLACK "and pointing at the path " FG_BLUE "target" FG_BLACK ".\n");
  IDOS->Printf("  example: setcmd create-version uae system System:Emulation/E-UAE-amigfx/uae\n");

  IDOS->Printf(BOLD_ON "delete-version " BOLD_OFF FG_BLUE "cmd version" FG_BLACK "\n");
  IDOS->Printf("  Removes version " FG_BLUE "version " FG_BLACK "from " FG_BLUE "cmd" FG_BLACK "\n");
  IDOS->Printf("  example: setcmd delete-version uae 1.0.0-beta\n");
  return;
}

