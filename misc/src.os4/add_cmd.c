#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int add_cmd(const char *cmd) 
{
  BPTR lock;
  int32 rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];

  // Sanity check, make sure we can access the SETCMD:cmds directory
  if (!can_lock(SETCMD_PATH)) {
    IDOS->Printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }
  
  // Now try and lock the requested directory to see if it already exists
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  if (can_lock(cmd_dir)) {
    IDOS->Printf("%sERROR %s: The command %s has already been added.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  } 

  // OK, we couldn't lock it, make sure it's just because it doesn't exist.
  if (IDOS->IoErr() != ERROR_OBJECT_NOT_FOUND) {
    IDOS->Printf("%sERROR %s:  Unexpected error when locking %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    dos_debug();
    return RETURN_FAIL;
  } 

  // Create the directory
  lock = IDOS->CreateDir(cmd_dir);
  if (!lock) {
    IDOS->Printf("%sERROR %s:  Unexpected error when creating directory  %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    dos_debug();
    return RETURN_FAIL;
  }

  IDOS->UnLock(lock);

  // Now create the soft link under SETCMD:path, checking first to see if it already exists
  strcpy(path, SETCMD_PATH);
  IDOS->AddPart(path, cmd, MAX_PATH_BUF);
  if (can_lock(path)) {
    IDOS->Printf("%sERROR %s:  The command %s has already been added to the path %s.\n", fmt(FG_RED), fmt(NORMAL), cmd, path);
    return RETURN_FAIL;
  } 

  rc = IDOS->MakeLink(path, SETCMD_STUB, LINK_SOFT);
  if (!rc) {
    IDOS->Printf("%sERROR %s:  Unexpected error when creating soft link  %s => %s.\n", fmt(FG_RED), fmt(NORMAL), path, SETCMD_STUB);
    dos_debug();  
    return RETURN_FAIL;
  }

  return RETURN_OK;
      
  
}
