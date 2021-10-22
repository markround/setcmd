#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

int main()
{
  struct DOSBase *DOSBase;
  char buffer[MAX_PATH_BUF], lock_name[MAX_PATH_BUF];
  struct CommandLineInterface *cli;
  struct PathNode *path_node, *next_node, *new_node, *temp_node;
  BPTR lock, new_node_bptr, test_lock;
  struct FileInfoBlock fi;
  BOOL rc;
  BOOL new_node_error = FALSE;

  printf("Hello, world from stdio.h land!\n");
  printf("Version: %s\n", SETCMD_VERSION);
  utility_test();

  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    printf("In AmigaDOS land!\n");
    
    printf("[+] Getting CLI struct\n")
    cli = Cli();
    path_node = NULL;

    if (DEBUG) {
      dump_current_path(DOSBase);
    }

    printf("[+] Attempting to modify path\n");

    // Reset pointer to the head of the path_node list
    path_node = NULL;
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);
    if (path_node == NULL) {
      printf("Failed\n");
      goto cleanup;
    }

    // Allocate memory for new node
    new_node = NULL;
    if ((new_node = AllocVec(sizeof(struct PathNode),MEMF_PUBLIC|MEMF_CLEAR)) == NULL) {
      printf("Failed\n");
      goto cleanup;
    }
   
    // Get a lock on the specified path
    new_node->lock = Lock("RAM:", ACCESS_READ);
    if (!new_node) {
      printf("Failed\n");
      new_node_error = TRUE;
      goto cleanup;
    }
    
    // Make sure specififed path is a directory
    rc = is_directory(new_node->lock)
    if (!rc) {
      printf("Failed\n");
      new_node_error = TRUE;
      goto cleanup;
    }

    // Check for duplicate entry at head
    if (SameLock(new_node->lock, path_node->lock) == LOCK_SAME) {
      printf("Already set, skipping...\n");
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
    printf("Failed to open DOS library.\n");
    exit(5);
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
  printf("Returned from AmigaDOS land.\n");
  
  return 0;


}