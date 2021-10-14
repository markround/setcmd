#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

typedef struct
{
  BPTR next;
  BPTR lock;
} PathEntry, *PathEntryPtr;


#define PE(x) ((PathEntry* )(BADDR(x)))

int main()
{
  struct DOSBase *DOSBase;

  int buffer[2048];
  PathEntryPtr cur = NULL;
  struct CommandLineInterface *cli;

  printf("Hello, world from stdio.h land!\n");
  
  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    PutStr("In AmigaDOS land!\n");

    PutStr("Current Directory\n");
    
    cli = Cli();
    PathEntryPtr cur = NULL;
    
    for (cur = PE(cli->cli_CommandDir); cur; cur = PE(cur->next))
    {
    	NameFromLock(cur->lock, buffer, sizeof(buffer));
    	PutStr(buffer);
    	PutStr("\n");
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
