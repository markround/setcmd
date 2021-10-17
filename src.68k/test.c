#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

/*---------------------------------------------------------------------------*/
/* The PathNode structure is not documented anywhere I could find, but I     */
/* have had occasion to access it in the past.  Obviously, it works!  Q.E.D. */
/*---------------------------------------------------------------------------*/
struct PathNode {
  BPTR next;
  BPTR lock;
};

int main()
{
  struct DOSBase *DOSBase;
  int buffer[2048];
  struct CommandLineInterface *cli;
  struct PathNode *pathNode, *nextNode, *tempNode;
  BPTR lock;

  printf("Hello, world from stdio.h land!\n");
  printf("Version: %s\n", SETCMD_VERSION);
  utility_test();

  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    printf("In AmigaDOS land!\n");
    
    printf("[+] Getting CLI struct\n")
    cli = Cli();
    pathNode = NULL;

     // get the pointer to the head of the PathNode list from the CLI struct
    pathNode = (struct PathNode *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
    
    printf("[+] Dumping current path\n");
    while (pathNode) {
      NameFromLock(pathNode->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      nextNode = (struct PathNode *)BADDR(pathNode->next);
      pathNode = nextNode;
    }

    printf("[+] Attempting to modify path\n");
   
    

    printf("[+] Dumping new path\n");
    // Show current path
    // get the pointer to the head of the PathNode list from the CLI struct
    pathNode = (struct PathNode *)BADDR(cli->cli_CommandDir);
    // TODO: Check this isn't NULL
     while (pathNode) {
      NameFromLock(pathNode->lock, buffer, sizeof(buffer));
    	printf("-> %s\n", buffer);
      nextNode = (struct PathNode *)BADDR(pathNode->next);
      pathNode = nextNode;
    }


  }
  else {
    printf("Failed to open DOS library.\n");
    exit(5);
  }
  
  if (DOSBase) CloseLibrary( (struct DOSBase *)DOSBase);
  printf("Returned from AmigaDOS land.\n");
  
  return 0;


}
