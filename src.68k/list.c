#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include "utility.h"

int list(int opt) 
{
  char cmd[MAX_PATH_BUF];
  char link[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char current_version[MAX_PATH_BUF];
  char cmd_dir[MAX_PATH_BUF];
  char target[MAX_PATH_BUF];
  struct ExamineData *path_data, *cmd_data;
  BPTR path_lock, cmd_lock;
  int rc = 0;

  path_lock = Lock(SETCMD_PATH, ACCESS_READ);

  if (!path_lock) {
    printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }


  printf("In Listing module\n");



  if (path_lock) {
    UnLock(path_lock);
  }
  
  return RETURN_OK;
}
