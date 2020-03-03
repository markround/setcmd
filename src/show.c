#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int show(char *cmd) 
{
  char version[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  int rc;

  rc = current_version(cmd, version); 
  if (rc == SETCMD_ERROR) {
    IDOS->Printf("Error: Command %s does not have a version set or does not exist.\n", cmd);
    return RETURN_FAIL;
  }

  rc = get_target(cmd, version, target);
  if (rc == SETCMD_ERROR) {
    IDOS->Printf("Error: Could not read link for %s/%s.\n", cmd, version);
    return RETURN_FAIL;
  }

  IDOS->Printf("Command:         %s\n", cmd);
  IDOS->Printf("Current version: " FG_BLUE "%s" FG_BLACK " (%s)\n", version, target);


  return RETURN_OK;
}
