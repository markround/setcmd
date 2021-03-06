#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int delete_cmd(const char *cmd) 
{
  BPTR lock;
  int32 rc;
  char cmd_dir[MAX_PATH_BUF];
  char path_dir[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  APTR path_context;
  struct ExamineData *path_data;

  // Sanity check, make sure we can access the SETCMD:cmds directory
  if (!can_lock(SETCMD_PATH)) {
    IDOS->Printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }
  
  // Now try and lock the requested cmd directory to see if it already exists
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  lock = IDOS->Lock(cmd_dir, ACCESS_READ);
  if (!lock) {
    IDOS->Printf("%sERROR %s: The command %s does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  } 

  // Iterate over all the installed versions and delete them
  path_context = IDOS->ObtainDirContextTags(EX_LockInput, lock, TAG_END);
  while (path_data = IDOS->ExamineDir(path_context)) {
    strcpy(path, SETCMD_CMDS);
    IDOS->AddPart(path, cmd, MAX_PATH_BUF);
    IDOS->AddPart(path, path_data->Name, MAX_PATH_BUF);
    if (!IDOS->Delete(path)) {
      IDOS->Printf("%sERROR %s: Unexpected error when deleting %s.\n", fmt(FG_RED), fmt(NORMAL), path);
      dos_debug();
      IDOS->ReleaseDirContext(path_context);
      IDOS->UnLock(lock);
      return RETURN_FAIL;
    }
  }

  IDOS->ReleaseDirContext(path_context);
  IDOS->UnLock(lock);

  // Now delete the cmd directory itself
  if (!IDOS->Delete(cmd_dir)) {
    IDOS->Printf("%sERROR %s: Unexpected error when deleting %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    dos_debug();
    return RETURN_FAIL;
  }

  // And delete the link in the path dir
  strcpy(path_dir, SETCMD_PATH);
  IDOS->AddPart(path_dir, cmd, MAX_PATH_BUF);
  if (!IDOS->Delete(path_dir)) {
    IDOS->Printf("%sERROR %s: Unexpected error when deleting %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    dos_debug();
    return RETURN_FAIL;
  }

  return RETURN_OK;
      
  
}
