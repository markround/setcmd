#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"

int list(int opt) 
{
  char cmd[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  char cmd_path[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  struct FileInfoBlock path_data, cmd_data;
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

  // Now we need to get our proc struct with MsgPort so we can use ReadLink later
  proc = NULL;
  proc = GetDeviceProc(SETCMD_PATH, proc);
  // Todo: Error handling
  if (proc != NULL) {
  }

  // Iterate over all the cmds. First get the data for the path dir
  rc = Examine(path_lock, &path_data);
  if (!rc) {
    printf("Failed to examine\n");
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

  // Now loop over all directory entries
  while (ExNext(path_lock, &path_data)) {

    // Extract the cmd name from path_data struct
    strcpy (cmd, path_data.fib_FileName);
    
    // Get the full path to the command
    strcpy(cmd_path, SETCMD_CMDS);
    AddPart(cmd_path, cmd, MAX_PATH_BUF);
      
    // Follow symlink
    if (ReadLink(proc->dvp_Port, path_lock, cmd, link, MAX_PATH_BUF)) {
      if (DEBUG) {
        printf("Following symlink for %s to %s\n", cmd_path, link);
      }
      
      strcpy (current_version, FilePart(link));
      // skip if for whatever reason we don't have a link, e.g. it's not a valid command
      if (strlen(link) > 0) {
        // Basic listing
        printf("%s [%s%s%s]\n", cmd, fmt(SELECTED), current_version, fmt(NORMAL));

        if (opt == OPT_VERBOSE) {
          /* Verbose mode - for every link, get a list of the versions under the
           * relevant "cmd" directory, and highlight it if it's also the currently
           * selected version.
           */

          // Full path to the relevant cmd dir
          strcpy(cmd_dir, SETCMD_CMDS);
          AddPart(cmd_dir, cmd, MAX_PATH_BUF);

          // Now, get a directory listing of the available versions underneath this cmd directory
          cmd_lock = Lock(cmd_dir, ACCESS_READ);
          if (!cmd_lock) {
            printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
            printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
            printf("For more information see the SetCmd manual.\n");
            cmd_rc = RETURN_FAIL;
            goto cleanup;
          }

          rc = Examine(cmd_lock, &cmd_data);
          if (!rc) {
            printf("Failed to examine\n");
            cmd_rc = RETURN_FAIL;
            goto cleanup;
          }

          while (ExNext(cmd_lock, &cmd_data)) {
            // Extract the version name from cmd_data struct
            strcpy (version, cmd_data.fib_FileName);
            rc = get_target(cmd, version, target);
            if (rc == SETCMD_ERROR) {
              printf("%sERROR %s: Could not read link for %s/%s.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
              return RETURN_FAIL;
            }
            if (strcmp(version,current_version) == 0) {
              printf(" CURRENT %s%s%s (%s)\n", fmt(FG_BLUE), version, fmt(NORMAL), target);
            }
            else {
              printf("  %s%s%s (%s)\n", fmt(FG_BLUE), version, fmt(NORMAL), target);
            }
            
          } // End of looping over available versions

          if (cmd_lock)  { UnLock(cmd_lock); }
        }
      }
    } else if (DEBUG) {
      printf("ReadLink for command %s failed\n", cmd); 
    }

  } // End of looping over cmds

cleanup:
  if (path_lock)  { UnLock(path_lock); }
  if (proc)       { FreeDeviceProc(proc); }
  
  return cmd_rc;
}
