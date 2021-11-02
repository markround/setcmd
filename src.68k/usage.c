#include <stdio.h>
#include <proto/dos.h>
#include "usage.h"
#include "utility.h"

void usage()
{
  printf("SetCmd " SETCMD_VERSION " " SETCMD_BANNER "\n");
  printf("\n");
  printf("Usage:\n");
  printf("\n");
  printf("%sinit%s %squiet | verbose%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_WHITE), fmt(NORMAL));
  printf("  Sets up the SetCmd path for the current shell.\n");
  printf("  example: setcmd init verbose\n");

  printf("%slist%s %sverbose%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_WHITE), fmt(NORMAL));
  printf("  List all commands managed by setcmd. The optional %sverbose%s argument shows additional detail.\n", fmt(FG_WHITE), fmt(NORMAL));
  printf("  example: setcmd list\n");


  printf("%sshow%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Shows details for specified command %scmd%s\n", fmt(FG_BLUE), fmt(NORMAL));
  printf("  example: setcmd show uae\n");

  printf("%sset-version%s %scmd version%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Sets %scmd%s to %sversion%s\n", fmt(FG_BLUE), fmt(FG_BLACK), fmt(FG_BLUE), fmt(FG_BLACK));
  printf("  example: setcmd set-version uae system\n");

  printf("%sadd-cmd%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Adds a new command %scmd%s and points it to a stub\n", fmt(FG_BLUE), fmt(NORMAL));
  printf("  example: setcmd add-cmd uae\n");

  printf("%sdelete-cmd%s %scmd%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Removes command %scmd%s from setcmd\n", fmt(FG_BLUE), fmt(NORMAL));
  printf("  example: setcmd delete-cmd uae\n");

  printf("%sadd-version%s %scmd version target%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Adds a new version for %scmd%s, named %sversion%s and pointing at the path %starget%s\n", fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL));
  printf("  example: setcmd add-version uae system System:Emulation/E-UAE-amigfx/uae\n");

  printf("%sdelete-version%s %scmd version%s\n", fmt(BOLD_ON), fmt(BOLD_OFF), fmt(FG_BLUE), fmt(NORMAL));
  printf("  Removes version %sversion%s from %scmd%s\n", fmt(FG_BLUE), fmt(NORMAL), fmt(FG_BLUE), fmt(NORMAL));
  printf("  example: setcmd delete-version uae 1.0.0-beta\n");
  return;
}

