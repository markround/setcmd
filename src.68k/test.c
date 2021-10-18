#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

/* Couldn't find this documented anywhere but there are several references to
   it from various sources, e.g. AROS
*/
struct path_node {
  BPTR next;
  BPTR lock;
};

int main()
{
  struct DOSBase *DOSBase;
  int buffer[2048];
  struct CommandLineInterface *cli;
  struct path_node *path_node, *next_node, *temp_node;
  BPTR lock;
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

     // get the pointer to the head of the path_node list from the CLI struct
    path_node = (struct path_node *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
    
    printf("[+] Dumping current path\n");
    while (path_node) {
      NameFromLock(path_node->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      next_node = (struct path_node *)BADDR(path_node->next);
      path_node = next_node;
    }

    printf("[+] Attempting to modify path\n");

    // Here we go!
    printf("  [-] resetting pointer to the head of the path_node list\n");
    path_node = (struct path_node *)BADDR(cli->cli_CommandDir);

    printf("  [-] Allocating memory for temp node\n");
    temp_node = NULL;
    if ((temp_node = AllocVec(sizeof(struct path_node),MEMF_PUBLIC|MEMF_CLEAR)) == NULL) {
      PrintFault(IoErr(),"Failed, bro.\n");
      goto cleanup;
    }
   
    printf("  [-] Locking RAM:\n")
    temp_node->lock = Lock("RAM:", ACCESS_READ);
    if (!temp_node) {
      PrintFault(IoErr(),"Failed, bro.\n");
      goto cleanup;
    }
    
    printf("  [-] Examining RAM:\n")
    rc = Examine(temp_node->lock, &fi)
    if (!rc) {
      PrintFault(IoErr(),"Failed, bro.\n");
      goto cleanup;
    }

    printf("[+] Dumping new path\n");
    // Show current path
    // get the pointer to the head of the path_node list from the CLI struct
    path_node = (struct path_node *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
     while (path_node) {
      NameFromLock(path_node->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      next_node = (struct path_node *)BADDR(path_node->next);
      path_node = next_node;
    }


  }
  else {
    printf("Failed to open DOS library.\n");
    exit(5);
  }
  

cleanup:
  // clean up temp_node struct
  if (temp_node) {
    if (temp_node->lock) {
      UnLock(temp_node->lock);
    }
    FreeVec(temp_node);
  }

  if (DOSBase) CloseLibrary( (struct DOSBase *)DOSBase);
  printf("Returned from AmigaDOS land.\n");
  
  return 0;


}
