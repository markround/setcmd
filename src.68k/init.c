#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"
#include "list.h"

int init(int opt)
{
  struct DOSBase *DOSBase;
  char buffer[MAX_PATH_BUF], lock_name[MAX_PATH_BUF], assign_path[MAX_PATH_BUF];
  struct CommandLineInterface *cli;
  struct PathNode *path_node, *next_node, *new_node, *temp_node;
  BPTR lock, new_node_bptr, test_lock;
  struct FileInfoBlock fi;
  BOOL rc;
  BOOL new_node_error = FALSE;

  // Sanity check
  lock = Lock(SETCMD_ASSIGN, ACCESS_READ);
  if (!lock)
  {
    printf("%sERROR %s: Failed to lock the " SETCMD_ASSIGN " directory\n", fmt(FG_RED), fmt(NORMAL));
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return RETURN_FAIL;
  }
  // Plus it's also useful to get the resolved path to display in verbose mode
  rc = NameFromLock(lock, assign_path, MAX_PATH_BUF);
  if (!rc) {
    UnLock(lock);
    return RETURN_FAIL;
  }
  UnLock(lock);

  // Need this for debugging
  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    // Get CLI struct
    cli = Cli();
    path_node = NULL;

    if (DEBUG) {
      dump_current_path(DOSBase);
    }

    // Reset pointer to the head of the path_node list
    path_node = NULL;
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);
    if (path_node == NULL) {
      printf("%sERROR %s: Failed to retrieve current path!\n", fmt(FG_RED), fmt(NORMAL));
      goto cleanup;
    }

    // Allocate memory for new node
    new_node = NULL;
    if ((new_node = AllocVec(sizeof(struct PathNode),MEMF_PUBLIC|MEMF_CLEAR)) == NULL) {
      printf("%sERROR %s: Failed to allocate memory for new node!\n", fmt(FG_RED), fmt(NORMAL));
      goto cleanup;
    }
   
    // Get a lock on the specified path
    new_node->lock = Lock(SETCMD_PATH, ACCESS_READ);
    if (!new_node) {
      printf("%sERROR %s: Failed to lock the " SETCMD_PATH " directory\n", fmt(FG_RED), fmt(NORMAL));
      printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      printf("For more information see the SetCmd manual.\n");
      new_node_error = TRUE;
      goto cleanup;
    }
    
    // Make sure specififed path is a directory
    rc = is_directory(new_node->lock);
    if (!rc) {
      printf("%sERROR %s: " SETCMD_PATH " is not a directory\n", fmt(FG_RED), fmt(NORMAL));
      printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
      printf("For more information see the SetCmd manual.\n");
      new_node_error = TRUE;
      goto cleanup;
    }

    // Check for duplicate entry at head, if so just skip
    if (SameLock(new_node->lock, path_node->lock) == LOCK_SAME) {
      new_node_error = TRUE;
      goto cleanup;  
    }

    // Set next pointer to the current path
    new_node->next = cli->cli_CommandDir;

    // Convert the new node to a BPTR
    new_node_bptr = MKBADDR(new_node);

    // Set the new path!
    cli->cli_CommandDir = new_node_bptr;

    if (DEBUG) {
      dump_current_path(DOSBase);
    }

  }
  else {
    printf("%sERROR %s: Failed to retrieve current path!\n", fmt(FG_RED), fmt(NORMAL));
    return RETURN_FAIL;
  }
  
cleanup:
  // clean up new_node struct if failed somewhere above
  if (new_node && new_node_error) {
    if (new_node->lock) {
      UnLock(new_node->lock);
    }
    FreeVec(new_node);
  }

  if (DOSBase) CloseLibrary( (struct DOSBase *)DOSBase);
  

  // Yay, we're all setup, now just display some info.
  if (opt == OPT_NONE) {
    printf("SetCmd " SETCMD_VERSION " initialised\n");
  }
  if (opt == OPT_VERBOSE) {
    printf("SetCmd " SETCMD_VERSION " initialised [%s]\n", assign_path);
    list(OPT_NONE);
  }

  if (new_node_error) { 
    return RETURN_FAIL;
  } else {
    return RETURN_OK;
  }

}
