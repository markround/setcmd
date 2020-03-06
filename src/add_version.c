#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
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
    IDOS->Printf("ERROR: Command %s does not exist.\n", cmd);
    dos_debug();  
    return RETURN_FAIL;
  }

  // Now check if the version exists
  strcpy(path, cmd_dir);
  IDOS->AddPart(path, version, MAX_PATH_BUF);
  if (can_lock(path)) {
    IDOS->Printf("ERROR: version %s already exists for command %s.\n", version, cmd);
    dos_debug();
    return RETURN_FAIL;  
  }

  // Create the link
  rc = IDOS->MakeLink(path, target, LINK_SOFT);
  if (!rc) {
    IDOS->Printf("ERROR: unexpected error creating link %s => %s.\n", path, target);  
    dos_debug();
  }

  return RETURN_OK;
}

