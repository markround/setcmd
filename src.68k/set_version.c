#include <exec/types.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <dos/dosextens.h>
#include <dos/dos.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int set_version(const char *cmd, const char *version)
{
  int rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];


  // Make sure the specified version exists
  strcpy(path, SETCMD_CMDS);
  AddPart(path, cmd, MAX_PATH_BUF);  
  AddPart(path, version, MAX_PATH_BUF);
  if (!can_lock(path)) {
    printf("%sERROR %s: version %s does not exist for command %s.\n", fmt(FG_RED), fmt(NORMAL), version, cmd);
    return RETURN_FAIL;  
  }

  strcpy(target, path);

  // Delete the link under the path directory
  strcpy(path, SETCMD_PATH);
  AddPart(path, cmd, MAX_PATH_BUF);
  rc = DeleteFile((char *)path);
  if (!rc) {
    printf("%sERROR %s: unexpected error deleting path link %s.\n", fmt(FG_RED), fmt(NORMAL), path);  
    return RETURN_FAIL;
  }

  // Make the new link
  rc = MakeLink((STRPTR)(char *)path, (LONG)(char *)target, LINK_SOFT);
  if (!rc) {
    printf("%sERROR %s: unexpected error creating link %s => %s.\n", fmt(FG_RED), fmt(NORMAL), path, target);  
  }

  return RETURN_OK;
  
}

