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
