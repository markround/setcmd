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
  struct CommandLineInterface *cli = (struct CommandLineInterface *)Cli();
  char check_path[MAX_PATH_BUF];
  char our_lock_path[MAX_PATH_BUF];
  char assign_path[MAX_PATH_BUF];
  BPTR lock;
  int32 rc = 0;

  // Sanity check
  lock = Lock(SETCMD_ASSIGN, SHARED_LOCK);
  if (!lock)
  {
    printf("%sERROR %s: Failed to lock the " SETCMD_ASSIGN " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }
  // Plus it's also useful to get the resolved path to display in verbose mode
  rc = NameFromLock(lock, assign_path, MAX_PATH_BUF);
  if (!rc) {
    UnLock(lock);
    return RETURN_FAIL;
  }
  UnLock(lock);

  pathlist = BADDR(cli->cli_PathList);

  lock = Lock(SETCMD_PATH, SHARED_LOCK);
  if (!lock) {
    printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  if (DEBUG) {
    printf("Obtained lock on " SETCMD_PATH "\n");
  }

  // Get the first Node in the Path, which should be our own path directory if we've 
  // already been initialised with the same SETCMD: assign...
  rc = NameFromLock(lock, our_lock_path, MAX_PATH_BUF);
  if (!rc) {
    UnLock(lock);
    return RETURN_FAIL;
  }
  rc = NameFromLock(pathlist->pn_Lock, check_path, MAX_PATH_BUF);
  if (!rc) {
    UnLock(lock);
    return RETURN_FAIL;
  }

  // Are we being re-initialised with the same assign ?
  // If so, skip it as we will clutter up the path list with duplicates.
  if (strcmp(check_path, our_lock_path) == 0) {
    if (DEBUG) {
      printf("SetCmd has already been initialised at %s.\n", our_lock_path);
      printf("Path from lock is : %s\n", check_path);
      printf("Path from our CLI process lock is : %s\n", our_lock_path);
    }
  } else {
    // Set up the new pathlist with us at the start
    new_path = AddCmdPathNode(pathlist, lock, ADDCMDPATHNODE_HEAD);

    if (!SetCurrentCmdPathList(new_path)) {
      // This shouldn't fail!
      printf("%sERROR %s: Failed to set the new command path!\n", fmt(FG_RED), fmt(NORMAL));
      printf("This should never happen :(\n");
      UnLock(lock);
      return RETURN_FAIL;  
    } 
  }
    
  UnLock(lock);

  // Yay, we're all setup, now just display some info.
  if (opt == OPT_NONE) {
    printf("SetCmd " SETCMD_VERSION " initialised\n");
  }
  if (opt == OPT_VERBOSE) {
    printf("SetCmd " SETCMD_VERSION " initialised [%s]\n", assign_path);
    list(OPT_NONE);
  }
  
  return RETURN_OK;
  
}
