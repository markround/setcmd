#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int list(int opt) {

  char cmd[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  struct ExamineData *path_data;
  struct ExamineData *cmd_data;
  APTR path_context, cmd_context;
  BPTR path_lock, cmd_lock;
  int32 rc = 0;

  path_lock = IDOS->Lock(SETCMD_PATH, ACCESS_READ);

  if (!path_lock) {
    IDOS->Printf("ERROR: Failed to lock the " SETCMD_PATH " directory\n");
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }

  path_context = IDOS->ObtainDirContextTags(EX_LockInput, path_lock, TAG_END);

  while (path_data = IDOS->ExamineDir(path_context)) {
    strcpy (cmd, path_data->Name);
    strcpy (link, path_data->Link);
    strcpy (current_version, IDOS->FilePart(link));
    // If the file is not a link, we just ignore it as it's not a valid command
    if (strlen(link) > 0) {
      IDOS->Printf("%s\n", cmd);
      if (opt == OPT_VERBOSE) {
        /* Verbose mode - for every link, get a list of the versions under the
         * relevant "cmd" directory, and highlight it if it's also the currently
         * selected version.
         */
        strcpy(cmd_dir, SETCMD_CMDS);
        IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
        cmd_lock = IDOS->Lock(cmd_dir, ACCESS_READ);
        if (!cmd_lock) {
          IDOS->Printf("ERROR: Failed to lock the %s directory\n", cmd_dir);
          IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
          IDOS->Printf("For more information see the SetCmd manual.\n");
          return RETURN_FAIL;
        }
        // Now, get a directory listing o fthe available versions underneath this cmd directory
        cmd_context = IDOS->ObtainDirContextTags(EX_LockInput, cmd_lock, TAG_END);
        while (cmd_data = IDOS->ExamineDir(cmd_context)) {
          strcpy(version, cmd_data->Name);
          if (strcmp(current_version, version) == 0) {
            IDOS->Printf("  [" FG_BLUE "%s" FG_BLACK "]\n", version);
          }
          else {
            IDOS->Printf("  " FG_WHITE "%s" FG_BLACK "\n", version);
          }
        }
        IDOS->ReleaseDirContext(cmd_context);

        if (cmd_lock) IDOS->UnLock(cmd_lock);
      }
    }
  }

  if (path_lock) IDOS->UnLock(path_lock);
  IDOS->ReleaseDirContext(path_context);

  return RETURN_OK;
}
