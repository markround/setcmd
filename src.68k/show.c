#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int show(const char *cmd) 
{
  char version[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char cmd_link[MAX_PATH_BUF];
  APTR cmd_context;
  BPTR cmd_lock;
  struct FileInfoBlock cmd_data;
  int rc;
  int cmd_rc = RETURN_OK;

  rc = get_current_command_version(cmd, version); 
  if (rc == SETCMD_ERROR) {
    printf("%sERROR %s: Command %s does not have a version set or does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  }

  printf("Command:         %s\n", cmd);
  
  // If we're just pointing at the stub, don't bother retrieving a link target
  if (strcmp(version, "stub") == 0) {
    printf("Current version: %s%s%s (%s)\n", fmt(SELECTED), version, fmt(NORMAL), SETCMD_STUB);
  } else {
    // Need to get full path to current version
    rc = get_target(cmd, version, cmd_link);
    if (rc == SETCMD_ERROR) {
      printf("%sERROR %s: Command %s version %s link does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
      return RETURN_FAIL;
    }
    printf("Current version: %s%s%s (%s)\n", fmt(SELECTED), version, fmt(NORMAL), cmd_link);
  }

  printf("\n");
  printf("Available versions\n");
  printf("------------------\n");

  // Full path to the relevant cmd dir
  strcpy(cmd_dir, SETCMD_CMDS);
  AddPart(cmd_dir, cmd, MAX_PATH_BUF);

  // Now, get a directory listing of the available versions underneath this cmd directory
  cmd_lock = Lock(cmd_dir, ACCESS_READ);
  if (!cmd_lock) {
    printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  rc = Examine(cmd_lock, &cmd_data);
  if (!rc) {
    printf("Failed to examine\n");
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

  while (ExNext(cmd_lock, &cmd_data)) {
    // Extract the version name from cmd_data struct
    strcpy (version, cmd_data.fib_FileName);
    rc = get_target(cmd, version, target);
    if (rc == SETCMD_ERROR) {
      printf("%sERROR %s: Could not read link for %s/%s.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
      cmd_rc = RETURN_FAIL;
      goto cleanup;
    }
    printf("%s%s%s (%s)\n", fmt(FG_BLUE), version, fmt(NORMAL), target);
  } // End of looping over available versions


cleanup:
  if (cmd_lock)  { UnLock(cmd_lock); }
  return cmd_rc;
  
}
