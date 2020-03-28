#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>
#include "utility.h"

void dos_debug()
{
  if (DEBUG) {
      // %m and %n magic modifiers only available in kickstart 51.59+
      IDOS->Printf("DOS error message = %m, error code = %n\n",0);
    }  
}

void dump_path_node(struct PathNode *node)
{
  while (node->pn_Next) {
    char path[MAX_PATH_BUF];
    if(IDOS->NameFromLock(node->pn_Lock, path, MAX_PATH_BUF)) {
      IDOS->Printf("  %s\n", path);
    }
    node = BADDR(node->pn_Next);
  }
}

int get_target(const char *cmd, const char *version, char *target)
{
  char cmd_dir[MAX_PATH_BUF];
  char cmd_version[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  BPTR lock;
  int32 rc;
 
  // Check the command directory exists
  strcpy(cmd_dir, SETCMD_CMDS);
  IDOS->AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  if (!can_lock(cmd_dir)) {
    if (DEBUG) {
      IDOS->Printf("%s ERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
      IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      IDOS->Printf("For more information see the SetCmd manual.\n");
    }
    return SETCMD_ERROR;
  }

  // Now get a lock on the specified version
  strcpy(cmd_version, cmd_dir);
  IDOS->AddPart(cmd_version, version, MAX_PATH_BUF);
  lock = IDOS->Lock(cmd_version, ACCESS_READ);
  if (!lock) {
    if (DEBUG) {
      IDOS->Printf("%s ERROR %s: Failed to lock the %s version\n", fmt(FG_RED), fmt(NORMAL), cmd_version);
      IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      IDOS->Printf("For more information see the SetCmd manual.\n");
    }
    return SETCMD_ERROR;
  }

  rc = IDOS->NameFromLock(lock, target, MAX_PATH_BUF);
  if (!rc) {
    IDOS->Printf("%s ERROR %s: Failed to read the link from %s\n", fmt(FG_RED), fmt(NORMAL), cmd_version);
    dos_debug();
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
    if (lock) {
      IDOS->UnLock(lock);
    }
    return SETCMD_ERROR;
  }

  if (lock) {
    IDOS->UnLock(lock);
  }
  
  return SETCMD_OK;
}

int current_version(const char *cmd, char *version)
{
  char link[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  struct ExamineData *data;
  APTR context;
  BPTR lock;
  BOOL found = FALSE;
  int32 rc;

  strcpy(path, SETCMD_PATH);
  IDOS->AddPart(path, cmd, MAX_PATH_BUF);
  lock = IDOS->Lock(path, ACCESS_READ);
  if (!lock) {
    if (DEBUG) {
      IDOS->Printf("%s ERROR %s: Failed to lock the %s path\n", fmt(FG_RED), fmt(NORMAL),  path);
      IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      IDOS->Printf("For more information see the SetCmd manual.\n");
    }
    dos_debug();
    return SETCMD_ERROR;
  }

  rc = IDOS->NameFromLock(lock, target, MAX_PATH_BUF);
  if (!rc) {
    if (DEBUG) {
      IDOS->Printf("%s ERROR %s: Failed to read the link from %s\n", fmt(FG_RED), fmt(NORMAL), path);
      IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      IDOS->Printf("For more information see the SetCmd manual.\n");
    }
    dos_debug();
    if (lock) {
      IDOS->UnLock(lock);
    }
    return SETCMD_ERROR;
  }

  // check if we are just pointing at the stub, if so then return "stub"
  if (strcmp(IDOS->FilePart(target), "stub") == 0) {
    strcpy(version, "stub");
    if (lock) {
      IDOS->UnLock(lock);
    }
    return SETCMD_OK;
  }
  
  // OK, so we're not pointing at the stub. Let's move on.
  IDOS->UnLock(lock);

  lock = IDOS->Lock(SETCMD_PATH, ACCESS_READ);
  if (!lock) {
    if (DEBUG) {
      IDOS->Printf("%s ERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
      IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      IDOS->Printf("For more information see the SetCmd manual.\n");
    }
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

  if (lock) {
    IDOS->UnLock(lock);
  }

  IDOS->ReleaseDirContext(context);

  if (!found) {
    return SETCMD_ERROR;  
  } 

  strcpy(version, current_version);
  return SETCMD_OK;
}

BOOL can_lock(const char *path)
{
  BPTR lock;
  lock = IDOS->Lock(path, ACCESS_READ);
  if (lock) {
    IDOS->UnLock(lock);
        return TRUE;
  } else {
    return FALSE;
  }

}

char *fmt(const char *fmt_string)
{
  int32 len;
  char buf[MAX_PATH_BUF];

  len = IDOS->GetVar(SETCMD_NOFORMAT_VAR, buf, MAX_PATH_BUF,  LV_VAR);
  if (len == -1) {
    // NO_FORMAT not set so we just return what was passed
    return fmt_string;
  } else {
    // The env var was set, so return nothing
    return "";
  }
}
