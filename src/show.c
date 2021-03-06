#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int show(const char *cmd) 
{
  char version[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  struct ExamineData *cmd_data;
  APTR cmd_context;
  BPTR cmd_lock;
  int rc;

  rc = current_version(cmd, version); 
  if (rc == SETCMD_ERROR) {
    IDOS->Printf("%sERROR %s: Command %s does not have a version set or does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  }

  // If we're just pointing at the stub, don't bother retrieving a link target
  if (strcmp(version, "stub") == 0) {
    IDOS->Printf("Command:         %s\n", cmd);
    IDOS->Printf("Current version: %s%s%s (%s)\n", fmt(SELECTED), version, fmt(NORMAL), SETCMD_STUB);
  } else {
    // It's pointing at a proper command, so get the current version
    rc = get_target(cmd, version, target);
    if (rc == SETCMD_ERROR) {
      IDOS->Printf("%sERROR %s: Could not read link for %s/%s.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
      return RETURN_FAIL;
    }

    IDOS->Printf("Command:         %s\n", cmd);
    IDOS->Printf("Current version: %s%s%s (%s)\n", fmt(SELECTED), version, fmt(NORMAL), target);
  }

  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  
  cmd_lock = IDOS->Lock(cmd_dir, ACCESS_READ);
  if (!cmd_lock) {
    IDOS->Printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  IDOS->Printf("\n");
  IDOS->Printf("Available versions\n");
  IDOS->Printf("------------------\n");

  // Now, get a directory listing of the available versions underneath this cmd directory
  cmd_context = IDOS->ObtainDirContextTags(EX_LockInput, cmd_lock, TAG_END);
  while (cmd_data = IDOS->ExamineDir(cmd_context)) {
    strcpy(version, cmd_data->Name);
    rc = get_target(cmd, version, target);
    if (rc == SETCMD_ERROR) {
      IDOS->Printf("%sERROR %s: Could not read link for %s/%s.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
      return RETURN_FAIL;
    }

    IDOS->Printf("%s%s%s (%s)\n", fmt(FG_BLUE), version, fmt(NORMAL), target);
  }

  IDOS->ReleaseDirContext(cmd_context);
  
  if (cmd_lock) { 
    IDOS->UnLock(cmd_lock); 
  }

  return RETURN_OK;
}
