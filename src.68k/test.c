#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "utility.h"

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
  BPTR lock;
  BOOL is_dir;

  printf("Hello, world from stdio.h land!\n");
  printf("Version: %s\n", SETCMD_VERSION);
  utility_test();

  DOSBase = (struct DOSBase *)OpenLibrary("dos.library", 0);
  
  if (DOSBase) {
    PutStr("In AmigaDOS land!\n");
    
    PutStr("[+] Getting CLI struct\n")
    cli = Cli();
    PathEntryPtr cur = NULL;

    PutStr("[+] Getting PathList struct pointer\n");
    PathEntryPtr insertAfter = (PathEntryPtr)&cli->cli_CommandDir;

    PutStr("[+] Testing is_directory function\n");
    PutStr("  [-] Getting lock on C:\n");
    lock = Lock("C:", SHARED_LOCK);
    if (!lock) {
      PutStr("  [!] Failed to get lock\n");
      return RETURN_FAIL;
    }

    is_dir = is_directory(lock, DOSBase);

    if (is_dir) {
      PutStr("  [I] Is a directory\n");
    }
    else {
      PutStr("  [I] Is not a directory\n");  
    }

    UnLock(lock);

    PutStr("  [-] Getting lock on RAM:Disk.info\n");
    lock = Lock("RAM:Disk.info", SHARED_LOCK);
    if (!lock) {
      PutStr("  [!] Failed to get lock\n");
      return RETURN_FAIL;
    }

    is_dir = is_directory(lock, DOSBase);

    if (is_dir) {
      PutStr("  [I] Is a directory\n");
    }
    else {
      PutStr("  [I] Is not a directory\n");  
    }

    UnLock(lock);




    // Loop through path 
    
    

    // Show current path
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
