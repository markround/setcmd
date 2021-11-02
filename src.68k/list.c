#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int list(int opt) 
{
  char cmd[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  struct FileInfoBlock path_data;
  struct DevProc *proc;
  BPTR path_lock, cmd_lock;
  int rc = 0;
  int cmd_rc = RETURN_OK;

  path_lock = Lock(SETCMD_PATH, ACCESS_READ);

  if (!path_lock) {
    printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  // Now we need to get our MsgPort so we can use ReadLink later
  //	GetDeviceProc -- Finds a handler to send a message to (V36), improved over DeviceProc
  proc = NULL;
  proc = GetDeviceProc(SETCMD_PATH, proc);
  // Todo: Error handling
  if (proc != NULL) {
  }


  printf("In Listing module\n");

  // Iterate over all the cmds. First get the data for the path dir
  rc = Examine(path_lock, &path_data);
  if (!rc) {
    printf("Failed to examine\n");
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

  // Now loop over all directory entries
  while (ExNext(path_lock, &path_data)) {
    /* path_data.fib_FileName has the filename so if we join it to SETCMD_PATH
     * we can then lock and examine the individual commands / symlinks
     */

    // Extract the cmd name
    strcpy (cmd, path_data.fib_FileName);
    if (DEBUG) {
      printf("Examining command %s\n", cmd);
    }

     // proc-> dvp_Port for readlink

    
  }

cleanup:
  if (path_lock) {
    UnLock(path_lock);
  }
  if (proc) {
    FreeDeviceProc(proc)
  }
  

  return cmd_rc;
}
