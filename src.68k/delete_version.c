#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <dos/dos.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int delete_version(const char *cmd, const char *version)
{
  int rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];

  // Sanity check, make sure we can access the cmd directory
  strcpy(cmd_dir, SETCMD_CMDS);
  AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  if (!can_lock(cmd_dir)) {
    printf("%sERROR %s: Command %s does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  }

  // Now check if the version exists
  strcpy(path, cmd_dir);
  AddPart(path, version, MAX_PATH_BUF);
  if (!can_lock(path)) {
    printf("%sERROR %s: version %s does not exist for command %s.\n", fmt(FG_RED), fmt(NORMAL), version, cmd);
    return RETURN_FAIL;  
  }

  // Delete the link
  rc = DeleteFile((char *)path);
  if (!rc) {
    printf("%sERROR %s: unexpected error deleting %s.\n", fmt(FG_RED), fmt(NORMAL), path);  
  }

  return RETURN_OK;
}

