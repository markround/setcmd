#include <exec/types.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int delete_cmd(const char *cmd) 
{
  BPTR lock;
  char cmd_dir[MAX_PATH_BUF];
  char path_entry[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char version_path[MAX_PATH_BUF];
  APTR path_context;
  struct FileInfoBlock path_data;
  int rc = 0;
  int cmd_rc = RETURN_OK;

  if (DEBUG) {
    printf("Going in...\n");
  }

  // Sanity check, make sure we can access the SETCMD:cmds directory
  if (!can_lock(SETCMD_PATH)) {
    printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }
  
  // Now try and lock the requested cmd directory to see if it already exists
  strcpy(cmd_dir, SETCMD_CMDS);
  AddPart(cmd_dir, cmd, MAX_PATH_BUF);

  if (DEBUG) {
    printf("Locking %s\n", cmd_dir);
  }

  lock = Lock(cmd_dir, ACCESS_READ);
  if (!lock) {
    printf("%sERROR %s: The command %s does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  } 

  // Iterate over all the installed versions and delete them

  // First get the data for the cmd dir
  // TODO: Test it is a dir first
  rc = Examine(lock, &path_data);
  if (!rc) {
    printf("Failed to examine\n");
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

  // Now loop over all directory entries
  if (DEBUG) {
    printf("Starting to iterate over versions\n");
  }
  while (ExNext(lock, &path_data)) {
    // Extract the cmd name from path_data struct
    strcpy (version, path_data.fib_FileName);
    
    // Get the full path to the version
    strcpy(version_path, SETCMD_CMDS);
    AddPart(version_path, cmd, MAX_PATH_BUF);
    AddPart(version_path, version, MAX_PATH_BUF);

    if (DEBUG) {
      printf("Deleting %s\n", version_path);
    }

    // Delete the version link
    rc = DeleteFile((char *)version_path);
    if (!rc) {
      printf("%sERROR %s: unexpected error deleting link %s.\n", fmt(FG_RED), fmt(NORMAL), version_path);  
    }
  }

  UnLock(lock);
  lock = (BPTR)NULL;

  if (DEBUG) {
    printf("Finished iterating over versions\n");
  }

  // Now delete the cmd directory itself
  if (DEBUG) {
    printf("Deleting %s\n", cmd_dir);
  }
  rc = DeleteFile((char *) cmd_dir);
  if (!rc) {
     printf("%sERROR %s: Unexpected error when deleting cmd dir %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
     return RETURN_FAIL;
  }

  // And delete the link in the path dir
  strcpy(path_entry, SETCMD_PATH);
  AddPart(path_entry, cmd, MAX_PATH_BUF);
  if (DEBUG) {
    printf("Deleting %s\n", path_entry);
  }
  rc = DeleteFile((char *) path_entry);
  if (!rc) {
     printf("%sERROR %s: Unexpected error when deleting path link %s.\n", fmt(FG_RED), fmt(NORMAL), path_entry);
     return RETURN_FAIL;
  }


  if (DEBUG) {
    printf("Finished deleting files\n");
  }


cleanup:
  if (lock)  { 
    printf("Unlocking\n");
    UnLock(lock);
  }

  return cmd_rc;
}
