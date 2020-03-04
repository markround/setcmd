#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"
#include "list.h"

int init(int opt)
{
  struct PathNode *pathlist, *check_dup, *new_path;
  struct CommandLineInterface *cli = (struct CommandLineInterface *)IDOS->Cli();
  BPTR lock;

  pathlist = BADDR(cli->cli_PathList);
  if (DEBUG) {
   IDOS->Printf("---> Before\n");
   dump_path_node(pathlist);
  }

  lock = IDOS->Lock(SETCMD_PATH, SHARED_LOCK);
  if (!lock) {
    IDOS->Printf("ERROR: Failed to lock the " SETCMD_PATH " directory\n");
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  } 

  if (DEBUG) {
    IDOS->Printf("Obtained lock on " SETCMD_PATH "\n");
  }

  check_dup = (struct PathNode *)IDOS->RemoveCmdPathNode(pathlist, lock);
  if (check_dup) {
    if (DEBUG) {
      IDOS->Printf("Found and removed existing SetCmd paths\n");
      dump_path_node(check_dup);
    }
    pathlist=check_dup;
  }

  new_path = IDOS->AddCmdPathNode(pathlist, lock, ADDCMDPATHNODE_HEAD);
  if (DEBUG) {
    IDOS->Printf("---> After\n");
    dump_path_node(new_path);
  }

  if (IDOS->SetCurrentCmdPathList(new_path)) {
    // Yay, we're all setup, now just display some info.
    if (opt != OPT_QUIET) IDOS->Printf("SetCmd " SC_VERSION " initialised\n");
    if (opt == OPT_VERBOSE) {
      list(OPT_NONE);
    }
    return RETURN_OK;
  } else {
    // This shouldn't fail!
    IDOS->Printf("ERROR: Failed to set the new command path!\n");
    IDOS->Printf("This should never happen :(\n");
    if (DEBUG) {
      // %m and %n magic modifiers only available in kickstart 51.59
      IDOS->Printf("DOS error message = %m, error code = %n\n",0);
    }
    return RETURN_FAIL;
  }
  
}
