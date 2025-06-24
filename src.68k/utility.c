#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

void utility_test()
{
  printf("Utilities loaded\n");
}

// Don't need this on AmigaOS4 as we have ExamineObject
#if !defined(__amigaos4__)
BOOL is_directory(BPTR lock)
{
  BOOL is_dir = FALSE;
    
  struct FileInfoBlock* fib = AllocDosObject(DOS_FIB, NULL);
    
  if (Examine(lock, fib)) {
    LONG entry_type = fib->fib_EntryType;

    if (entry_type >= ST_ROOT && entry_type <= ST_LINKDIR) {
      if (entry_type != ST_SOFTLINK) {
        is_dir = TRUE;
      }
      else {
        BPTR lock_copy = DupLock(lock);    
        if (lock_copy) {
          BPTR file = OpenFromLock(lock_copy);          
          if (file) {
            // lock was on a file, it's now been
            // relinquished when we opened it with OpenFromLock
            Close(file);                       
            is_dir = FALSE;
          }
          else {
            UnLock(lock_copy);
          }
        }
        else {
          is_dir = FALSE;
        }
      }
    }

    FreeDosObject(DOS_FIB, fib);
  }  

  return(is_dir);
}
#endif

BOOL path_is_directory(char *path)
{
  BOOL is_dir = FALSE;

// AmigaOS 4 has ExamineData
#if defined(__amigaos4__)
  struct ExamineData *dat;
  dat = ExamineObjectTags(EX_StringNameInput, path, TAG_END);
	if (dat) {
    is_dir = EXD_IS_DIRECTORY(dat);
	  FreeDosObject(DOS_EXAMINEDATA,dat);
	} else {
	  PrintFault(IoErr(),NULL);
	}
// Fall back to our own test function
#else
  BPTR test_lock;
  test_lock = Lock(path, ACCESS_READ);
  if (test_lock) {
    is_dir = is_directory(test_lock);
    UnLock(test_lock);
    test_lock = (BPTR)NULL;
  }
#endif

  return is_dir;
}


char *fmt(char *fmt_string)
{
  int len;
  char buf[MAX_PATH_BUF];

  len = GetVar(SETCMD_NOFORMAT_VAR, buf, MAX_PATH_BUF,  LV_VAR);
  if (len == -1) {
    // NO_FORMAT not set so we just return what was passed
    return fmt_string;
  } else {
    // The env var was set, so return nothing
    return "";
  }
}


BOOL can_lock(const char *path)
{
  BPTR lock;
  lock = Lock(path, ACCESS_READ);
  if (lock) {
    UnLock(lock);
    return TRUE;
  } else {
    return FALSE;
  }
}


int get_target(const char *cmd, const char *version, char *target)
{
  char cmd_dir[MAX_PATH_BUF];
  char cmd_version[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  BPTR lock;
  int rc;
 
  // Check the command directory exists
  strcpy(cmd_dir, SETCMD_CMDS);
  AddPart(cmd_dir, cmd, MAX_PATH_BUF);
  if (!can_lock(cmd_dir)) {
    if (DEBUG) {
      printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
      printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      printf("For more information see the SetCmd manual.\n");
    }
    return SETCMD_ERROR;
  }

  // Now get a lock on the specified version
  strcpy(cmd_version, cmd_dir);
  AddPart(cmd_version, version, MAX_PATH_BUF);
  lock = Lock(cmd_version, ACCESS_READ);
  if (!lock) {
    if (DEBUG) {
      printf("%sERROR %s: Failed to lock the %s version\n", fmt(FG_RED), fmt(NORMAL), cmd_version);
      printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      printf("For more information see the SetCmd manual.\n");
    }
    return SETCMD_ERROR;
  }

  rc = NameFromLock(lock, target, MAX_PATH_BUF);
  if (!rc) {
    printf("%sERROR %s: Failed to read the link from %s\n", fmt(FG_RED), fmt(NORMAL), cmd_version);
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    if (lock) {
      UnLock(lock);
    }
    return SETCMD_ERROR;
  }

  if (lock) {
    UnLock(lock);
  }
  
  return SETCMD_OK;
}


int current_version(const char *cmd, char *version) {
  char current_version[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  BPTR lock, cmd_lock, path_lock;
  struct FileInfoBlock cmd_data;
  struct DevProc *proc;
  int rc;
  int cmd_rc = SETCMD_OK;

  strcpy(path, SETCMD_PATH);
  AddPart(path, cmd, MAX_PATH_BUF);
  lock = Lock(path, ACCESS_READ);
  if (!lock) {
    if (DEBUG) {
      printf("%sERROR %s: Failed to lock the %s path\n", fmt(FG_RED), fmt(NORMAL),  path);
    }
    return SETCMD_ERROR;
  }

  rc = NameFromLock(lock, target, MAX_PATH_BUF);
  if (!rc) {
    if (DEBUG) {
      printf("%sERROR %s: Failed to read the link from %s\n", fmt(FG_RED), fmt(NORMAL), path);
    }
    if (lock) {
      UnLock(lock);
    }
    return SETCMD_ERROR;
  }

  // check if we are just pointing at the stub, if so then return "stub"
  if (strcmp(FilePart(target), "stub") == 0) {
    strcpy(version, "stub");
    if (lock) {
      UnLock(lock);
    }
    return SETCMD_OK;
  }
  
  // OK, so we're not pointing at the stub. Let's move on.
  // Need to lock ther SETCMD_PATH
  path_lock = Lock(SETCMD_PATH, ACCESS_READ);

  if (!path_lock) {
    printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    cmd_rc = SETCMD_ERROR;
    goto cleanup;
  }

  // Now we need to get our proc struct with MsgPort so we can use ReadLink later
  proc = NULL;
  proc = GetDeviceProc(SETCMD_PATH, proc);
  if (!proc) {
    cmd_rc = SETCMD_ERROR;
    goto cleanup;
  }

#if defined(__amigaos4__)
  if (ReadSoftLink(proc->dvp_Port, path_lock, cmd, link, MAX_PATH_BUF)) {
    strcpy(version, FilePart(link));
  }
#else
  if (ReadLink(proc->dvp_Port, path_lock, cmd, link, MAX_PATH_BUF)) {
    strcpy(version, FilePart(link));
  }
#endif

  
cleanup:
  if (path_lock)  { UnLock(path_lock); }
  if (lock)       { UnLock(lock); }
  if (proc)       { FreeDeviceProc(proc); }
  return cmd_rc;    
  
}
