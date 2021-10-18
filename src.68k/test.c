#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

/* Couldn't find this documented anywhere but there are several references to
   it from various sources, e.g. AROS
*/
struct PathNode {
  BPTR next;
  BPTR lock;
};

int main()
{
  struct DOSBase *DOSBase;
  char buffer[2048], lock_name[2048];
  struct CommandLineInterface *cli;
  struct PathNode *path_node, *next_node, *new_node, *temp_node;
  BPTR lock, new_node_bptr, test_lock;
  struct FileInfoBlock fi;
  BOOL rc;

  printf("Hello, world from stdio.h land!\n");
  printf("Version: %s\n", SETCMD_VERSION);
  utility_test();

  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    printf("In AmigaDOS land!\n");
    
    printf("[+] Getting CLI struct\n")
    cli = Cli();
    path_node = NULL;

    /* get the pointer to the head of the path_node list (as a BPTR)
       and cast it to a regular struct pointer defined above,
       using a helper function 
    */
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
    
    printf("[+] Dumping current path\n");
    while (path_node) {
      NameFromLock(path_node->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      next_node = (struct PathNode *)BADDR(path_node->next);
      path_node = next_node;
    }

    printf("[+] Attempting to modify path\n");

    // Here we go!
    printf("  [-] resetting pointer to the head of the path_node list\n");
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);

    printf("  [-] Allocating memory for temp node\n");
    new_node = NULL;
    if ((new_node = AllocVec(sizeof(struct PathNode),MEMF_PUBLIC|MEMF_CLEAR)) == NULL) {
      printf("Failed, bro.\n");
      goto cleanup;
    }
   
    printf("  [-] Locking RAM:\n")
    new_node->lock = Lock("RAM:", ACCESS_READ);
    if (!new_node) {
      printf("Failed, bro.\n");
      goto cleanup;
    }
    
    printf("  [-] Examining RAM:\n")
    rc = Examine(new_node->lock, &fi)
    if (!rc) {
      printf("Failed, bro.\n");
      goto cleanup;
    }

    printf("  [-] Making sure RAM: is a directory\n")
    rc = is_directory(new_node->lock, DOSBase)
    if (!rc) {
      printf("Failed, bro.\n");
      goto cleanup;
    }

  /* Debugging time here. 
     Is seems as the the path_node we get is still just pointing to SYS:
     At least, that's what it looks like as we crash right after showing the new
     PATH to be RAM:, SYS: and then boom. 
     
     Then it dies if we run it again so I guess that might be the whole duplicates thing.
     So, first let's check what we've currently got...
  */

    test_lock = path_node->lock;
    NameFromLock(test_lock, lock_name, sizeof(lock_name));
    printf("-> Lock name is %s\n", lock_name);

    printf( "  [-] Setting next pointer to the current path\n");
    new_node->next = MKBADDR(cli->cli_CommandDir);

    // Convert the new node to a BPTR, this goes bang
    //new_node_bptr = MKBADDR(new_node);

    //printf( "  [-] Setting the new path!\n");
    //cli->cli_CommandDir = new_node_bptr;

    printf("[+] Dumping new path\n");
    // Show current path
    // get the pointer to the head of the path_node list from the CLI struct
    path_node = (struct PathNode *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
     while (path_node) {
      NameFromLock(path_node->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      next_node = (struct PathNode *)BADDR(path_node->next);
      path_node = next_node;
    }


  }
  else {
    printf("Failed to open DOS library.\n");
    exit(5);
  }
  

cleanup:
  // clean up new_node struct
  if (new_node) {
    if (new_node->lock) {
      UnLock(new_node->lock);
    }
    FreeVec(new_node);
  }

  if (DOSBase) CloseLibrary( (struct DOSBase *)DOSBase);
  printf("Returned from AmigaDOS land.\n");
  
  return 0;


}
