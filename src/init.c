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

  /* If SetCMD has already been initialised, then doing this again in a new CLI process
   * will result in a DSI when the new CLI exits. I'm pretty sure this has to do with the
   * pathlist not being freed; I still have to work out the best way to deal with this, for now
   * we simply do not allow "setcmd init" to be run twice. 
   */
  check_dup = (struct PathNode *)IDOS->RemoveCmdPathNode(pathlist, lock);
  if (check_dup) {
    IDOS->Printf("ERROR: SetCmd already initialised.\n");
    IDOS->UnLock(lock);
    if (DEBUG) {
      dump_path_node(check_dup);
    }
    return RETURN_FAIL;
  }

  // Set up the new pathlist with us at the start
  new_path = IDOS->AddCmdPathNode(pathlist, lock, ADDCMDPATHNODE_HEAD);
  if (DEBUG) {
    IDOS->Printf("---> After\n");
    dump_path_node(new_path);
  }

  if (IDOS->SetCurrentCmdPathList(new_path)) {
    // Yay, we're all setup, now just display some info.
    if (opt != OPT_QUIET) IDOS->Printf("SetCmd " SETCMD_VERSION " initialised\n");
    if (opt == OPT_VERBOSE) {
      list(OPT_NONE);
    }
    return RETURN_OK;
  } else {
    // This shouldn't fail!
    IDOS->Printf("ERROR: Failed to set the new command path!\n");
    IDOS->Printf("This should never happen :(\n");
    debug_dos();
    return RETURN_FAIL;
  }
  
}
