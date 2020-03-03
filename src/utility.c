#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include "utility.h"

STRPTR btos(BSTR bstr)
{
  return IUtility->ASPrintf("%b", bstr);
}


void dump_path_node(struct PathNode *node)
{
  while (node->pn_Next) {
    char path[MAX_PATH_BUF];
    if(IDOS->NameFromLock(node->pn_Lock, path, MAX_PATH_BUF)) {
      IDOS->Printf("%s\n", path);
    }
    node = BADDR(node->pn_Next);
  }
}

int get_target(char *cmd, char *version, char *target)
{
  char cmd_dir[MAX_PATH_BUF];
  char cmd_version[MAX_PATH_BUF];
  BPTR version_lock, cmd_lock;
  int32 rc;
 

  // Check the command directory exists
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  cmd_lock = IDOS->Lock(cmd_dir, ACCESS_READ);
  if (!cmd_lock) {
    IDOS->Printf("ERROR: Failed to lock the %s directory\n", cmd_dir);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }
  if (cmd_lock) IDOS->UnLock(cmd_lock);

  // Now get a lock on the specified version
  strcpy(cmd_version, cmd_dir);
  IDOS->AddPart(cmd_version, version, MAX_PATH_BUF);
  version_lock = IDOS->Lock(cmd_version, ACCESS_READ);
  if (!version_lock) {
    IDOS->Printf("ERROR: Failed to lock the %s version\n", cmd_version);
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }

  rc = IDOS->NameFromLock(version_lock, target, MAX_PATH_BUF);
  if (!rc) {
    int32 io_error =  IDOS->IoErr();
    IDOS->Printf("ERROR: Failed to read the link from %s\n", cmd_version);
    if (DEBUG) {
      IDOS->Printf("DOS Error Code: %d\n", io_error);
      IDOS->Printf("Name from lock: %s\n", target);
      IDOS->PrintFault(io_error, NULL);
    }
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    if (version_lock) IDOS->UnLock(version_lock);
    return SETCMD_ERROR;
  }

  if (version_lock) IDOS->UnLock(version_lock);
  return SETCMD_OK;
}

int current_version(char *cmd, char *version)
{
  char link[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  struct ExamineData *data;
  APTR context;
  BPTR lock;
  BOOL found = FALSE;

  lock = IDOS->Lock(SETCMD_PATH, ACCESS_READ);

  if (!lock) {
    IDOS->Printf("ERROR: Failed to lock the " SETCMD_PATH " directory\n");
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }

  context = IDOS->ObtainDirContextTags(EX_LockInput, lock, TAG_END);

  while (data = IDOS->ExamineDir(context)) {
    strcpy (link, data->Link);
    // If the file is not a link, we just ignore it as it's not a valid command
    if (strlen(link) > 0 && strcmp(data->Name, cmd) == 0) {
      strcpy(current_version, IDOS->FilePart(link));
      found = TRUE;
      break;
    }
  }

  if (lock) IDOS->UnLock(lock);
  IDOS->ReleaseDirContext(context);

  if (!found) return SETCMD_ERROR;   

  strcpy(version, current_version);
  return SETCMD_OK;
}
