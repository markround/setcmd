#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int list(int opt) 
{
  char cmd[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char get_current_command_version[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  struct ExamineData *path_data, *cmd_data;
  APTR path_context, cmd_context;
  BPTR path_lock, cmd_lock;
  int32 rc = 0;

  path_lock = Lock(SETCMD_PATH, ACCESS_READ);

  if (!path_lock) {
    printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  path_context = ObtainDirContextTags(EX_LockInput, path_lock, TAG_END);

  while (path_data = ExamineDir(path_context)) {
    strcpy (cmd, path_data->Name);
    strcpy (link, path_data->Link);
    strcpy (get_current_command_version, FilePart(link));
    // If the file is not a link, we just ignore it as it's not a valid command
    if (strlen(link) > 0) {
      printf("%s [%s%s%s]\n", cmd, fmt(SELECTED), get_current_command_version, fmt(NORMAL));
      if (opt == OPT_VERBOSE) {
        /* Verbose mode - for every link, get a list of the versions under the
         * relevant "cmd" directory, and highlight it if it's also the currently
         * selected version.
         */
        strcpy(cmd_dir, SETCMD_CMDS);
        AddPart(cmd_dir, cmd, MAX_PATH_BUF);
        cmd_lock = Lock(cmd_dir, ACCESS_READ);
        if (!cmd_lock) {
          printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
          printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
          printf("For more information see the SetCmd manual.\n");
          return RETURN_FAIL;
        }
        // Now, get a directory listing of the available versions underneath this cmd directory
        cmd_context = ObtainDirContextTags(EX_LockInput, cmd_lock, TAG_END);
        while (cmd_data = ExamineDir(cmd_context)) {
          strcpy(version, cmd_data->Name);
          rc = get_target(cmd, version, target);
          if (rc == SETCMD_ERROR) {
            printf("%sERROR %s: Could not read link for %s/%s.\n", fmt(FG_RED), fmt(NORMAL), cmd, version);
            return RETURN_FAIL;
          }
          printf("  %s%s%s (%s)\n", fmt(FG_BLUE), version, fmt(NORMAL), target);
        }
        ReleaseDirContext(cmd_context);

        if (cmd_lock) {
          UnLock(cmd_lock);
        }
      }
    }
  }

  if (path_lock) {
    UnLock(path_lock);
  }
  
  ReleaseDirContext(path_context);

  return RETURN_OK;
}
