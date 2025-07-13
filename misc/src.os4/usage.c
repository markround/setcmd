#include <proto/dos.h>
#include "usage.h"
#include "utility.h"

void usage()
{
  IDOS->Printf("SetCmd " SETCMD_VERSION " " SETCMD_BANNER "\n");
  IDOS->Printf("\n");
  IDOS->Printf("Usage:\n");
  IDOS->Printf("\n");
  IDOS->Printf("%sinit%s %squiet | verbose%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_WHITE), fmt(NORMAL));
  IDOS->Printf("  Sets up the SetCmd path for the current shell.\n");
  IDOS->Printf("  example: setcmd init verbose\n");

  IDOS->Printf("%slist%s %sverbose%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_WHITE), fmt(NORMAL));
  IDOS->Printf("  List all commands managed by setcmd. The optional %sverbose%s argument shows additional detail.\n", fmt(FG_WHITE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd list\n");


  IDOS->Printf("%sshow%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Shows details for specified command %scmd%s\n", fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd show uae\n");

  IDOS->Printf("%sset-version%s %scmd version%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Sets %scmd%s to %sversion%s\n", fmt(FG_BLUE), fmt(FG_BLACK), fmt(FG_BLUE), fmt(FG_BLACK));
  IDOS->Printf("  example: setcmd set-version uae system\n");

  IDOS->Printf("%sadd-cmd%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Adds a new command %scmd%s and points it to a stub\n", fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd add-cmd uae\n");

  IDOS->Printf("%sdelete-cmd%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Removes command %scmd%s from setcmd\n", fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd delete-cmd uae\n");

  IDOS->Printf("%sadd-version%s %scmd version target%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Adds a new version for %scmd%s, named %sversion%s and pointing at the path %starget%s\n", fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd add-version uae system System:Emulation/E-UAE-amigfx/uae\n");

  IDOS->Printf("%sdelete-version%s %scmd version%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  Removes version %sversion%s from %scmd%s\n", fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL));
  IDOS->Printf("  example: setcmd delete-version uae 1.0.0-beta\n");
  return;
}

