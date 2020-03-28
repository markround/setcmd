#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <dos/dosextens.h>
#include <dos/dos.h>
#include <string.h>
#include "utility.h"

int add_version(const char *cmd, const char *version, const char *target)
{
  int32 rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];

  // Sanity check, make sure we can access the cmd directory
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  if (!can_lock(cmd_dir)) {
    IDOS->Printf("%sERROR %s: Command %s does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    dos_debug();  
    return RETURN_FAIL;
  }

  // Now check if the version exists
  strcpy(path, cmd_dir);
  IDOS->AddPart(path, version, MAX_PATH_BUF);
  if (can_lock(path)) {
    IDOS->Printf("%sERROR %s: version %s already exists for command %s.\n", fmt(FG_RED), fmt(NORMAL), version, cmd);
    dos_debug();
    return RETURN_FAIL;  
  }

  // Create the link
  rc = IDOS->MakeLink((char *)path, (char *)target, LINK_SOFT);
  if (!rc) {
    IDOS->Printf("%sERROR %s: unexpected error creating link %s => %s.\n", fmt(FG_RED), fmt(NORMAL), path, target);  
    dos_debug();
  }

  return RETURN_OK;
}

