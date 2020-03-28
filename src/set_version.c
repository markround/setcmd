#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <dos/dosextens.h>
#include <dos/dos.h>
#include <string.h>
#include "utility.h"

int set_version(const char *cmd, const char *version)
{
  int32 rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];


  // Make sure the specified version exists
  strcpy(path, SETCMD_CMDS);
  IDOS->AddPart(path, cmd, MAX_PATH_BUF);  
  IDOS->AddPart(path, version, MAX_PATH_BUF);
  if (!can_lock(path)) {
    IDOS->Printf("%s ERROR %s: version %s does not exist for command %s.\n", fmt(FG_RED), fmt(NORMAL), version, cmd);
    dos_debug();
    return RETURN_FAIL;  
  }

  strcpy(target, path);

  // Delete the link under the path directory
  strcpy(path, SETCMD_PATH);
  IDOS->AddPart(path, cmd, MAX_PATH_BUF);
  rc = IDOS->Delete((char *)path);
  if (!rc) {
    IDOS->Printf("%s ERROR %s: unexpected error deleting path link %s.\n", fmt(FG_RED), fmt(NORMAL), path);  
    dos_debug();
    return RETURN_FAIL;
  }

  // Make the new link
  rc = IDOS->MakeLink((char *)path, (char *)target, LINK_SOFT);
  if (!rc) {
    IDOS->Printf("%s ERROR %s: unexpected error creating link %s => %s.\n", fmt(FG_RED), fmt(NORMAL), path, target);  
    dos_debug();
  }

  return RETURN_OK;
  
}

