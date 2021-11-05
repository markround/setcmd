#include "utility.h"
#include <stdio.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

void utility_test()
{
  printf("Utilities loaded\n");
}

void dump_current_path(APTR DOSBase)
{
  struct CommandLineInterface *cli;
  char buffer[MAX_PATH_BUF];
  struct PathNode *path_node, *next_node;

  if (DOSBase) {
    printf("[+] Dumping current path\n");
    cli = Cli();
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);    
    while (path_node) {
      NameFromLock(path_node->lock, buffer, MAX_PATH_BUF);
    	printf("-> %s\n", buffer);
      next_node = (struct PathNode *)BADDR(path_node->next);
      path_node = next_node;
    }
  }
}


BOOL is_directory(BPTR lock)
{
  BOOL is_directory = FALSE;
    
  struct FileInfoBlock* fib = AllocDosObject(DOS_FIB, NULL);
    
  if (Examine(lock, fib)) {
    LONG entry_type = fib->fib_EntryType;

    if (entry_type >= ST_ROOT && entry_type <= ST_LINKDIR) {
      if (entry_type != ST_SOFTLINK) {
        is_directory = TRUE;
      }
      else {
        BPTR lock_copy = DupLock(lock);    
        if (lock_copy) {
          BPTR file = OpenFromLock(lock_copy);          
          if (file) {
            // lock was on a file, it's now been
            // relinquished when we opened it with OpenFromLock
            Close(file);                       
            is_directory = FALSE;
          }
          else {
            UnLock(lock_copy);
          }
        }
        else {
          is_directory = FALSE;
        }
      }
    }

    FreeDosObject(DOS_FIB, fib);
  }  

  return(is_directory);
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