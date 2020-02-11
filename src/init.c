#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int init(int opt)
{
  struct PathNode *pathlist;
  struct PathNode *check_dup;
  struct CommandLineInterface *cli = (struct CommandLineInterface *)IDOS->Cli();

  pathlist = BADDR(cli->cli_PathList);
  if (DEBUG) {
   IDOS->Printf("---> Before\n");
   DumpPathNode(pathlist);
  }

  BPTR lock = IDOS->Lock(SETCMD_PATH, SHARED_LOCK);
  if (lock) {
    if (DEBUG) IDOS->Printf("Obtained lock on " SETCMD_PATH "\n");
    struct Pathnode *check_dup = (struct PathNode *)IDOS->RemoveCmdPathNode(pathlist, lock);
    if (check_dup) {
      if (DEBUG) {
        IDOS->Printf("Found and removed existing SetCmd paths\n");
        DumpPathNode(check_dup);
      }
      pathlist=check_dup;
    }
    struct PathNode *new_path = IDOS->AddCmdPathNode(pathlist, lock, ADDCMDPATHNODE_HEAD);
    if (DEBUG) {
      IDOS->Printf("---> After\n");
      DumpPathNode(new_path);
    }

    if (IDOS->SetCurrentCmdPathList(new_path)) {
      if (opt != OPT_QUIET) IDOS->Printf("SetCmd " SC_VERSION " initialised\n");
      return RETURN_OK;
    }
  } else {
    IDOS->Printf("ERROR: Failed to lock the " SETCMD_PATH " directory\n");
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }
}
