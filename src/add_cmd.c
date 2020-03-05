#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int add_cmd(char *cmd) 
{
  BPTR lock;
  int32 rc;
  char cmd_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];

  // Sanity check, make sure we can access the SETCMD:cmds directory
  lock = IDOS->Lock(SETCMD_CMDS, ACCESS_READ);
  if (!lock) {
    IDOS->Printf("ERROR: Failed to lock the %s directory\n", cmd_dir);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }
  if (lock) { 
    IDOS->UnLock(cmd_lock); 
  }

  // Now try and lock the requested directory to see if it already exists
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  lock = IDOS->Lock(SETCMD_CMDS, ACCESS_READ);
  if (lock) {
    IDOS->Printf("Error: The command %s has already been added.\n", cmd);
    IDOS->UnLock(lock); 
    return RETURN_FAIL;
  } 

  // OK, we couldn't lock it, make sure it's just because it doesn't exist.
  if (IoErr() != ERROR_OBJECT_NOT_FOUND) {
    IDOS->Printf("Error: Unexpected error when locking %s.\n", cmd_dir);
    if (DEBUG) {
      IDOS->Printf("DOS error message = %m, error code = %n\n",0);
    }
    return RETURN_FAIL;
  } 

  // Create the directory
  lock = IDOS->CreateDir(cmd_dir);
  if (!lock) {
    IDOS->Printf("Error: Unexpected error when creating directory  %s.\n", cmd_dir);
    if (DEBUG) {
      IDOS->Printf("DOS error message = %m, error code = %n\n",0);
    }
    return RETURN_FAIL;
  }

  IDOS->UnLock(lock);

  // Now create the soft link under SETCMD:path, checking first to see if it already exists
  strcpy(path, SETCMD_PATH);
  IDOS->AddPart(path, cmd, MAX_PATH_BUF);
  lock = IDOS->Lock(path, ACCESS_READ);
  if (lock) {
    IDOS->Printf("Error: The command %s has already been added to the path %s.\n", cmd, path);
    IDOS->UnLock(lock); 
    return RETURN_FAIL;
  } 

  rc = IDOS->MakeLink(path, SETCMD_STUB, LINK_SOFT);
  if (!rc) {
    IDOS->Printf("Error: Unexpected error when creating soft link  %s => %s.\n", path, SETCMD_STUB);
    if (DEBUG) {
      IDOS->Printf("DOS error message = %m, error code = %n\n",0);
    }  
    return RETURN_FAIL;
  }

  return RETURN_OK;
      
  
}
