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

    if (is_directory(lock, DOSBase)) {
      PutStr("  [I] Is a directory\n");
    }
    else {
      PutStr("  [I] Is not a directory\n");  
    }

    UnLock(lock);

    PutStr("  [-] Getting lock on S:User-Startup\n");
    lock = Lock("S:User-Startup", SHARED_LOCK);
    if (!lock) {
      PutStr("  [!] Failed to get lock\n");
      return RETURN_FAIL;
    }

    if (is_directory(lock, DOSBase)) {
      PutStr("  [I] Is a directory\n");
    }
    else {
      PutStr("  [I] Is not a directory\n");  
    }

    UnLock(lock);
    
    PutStr("[+] Dumping current path\n");
    // Show current path
    for (cur = PE(cli->cli_CommandDir); cur; cur = PE(cur->next))
    {
    	NameFromLock(cur->lock, buffer, sizeof(buffer));
    	PutStr(buffer);
    	PutStr("\n");
    }

    PutStr("[+] Attempting to modify path\n");
    PutStr("  [-] Getting current PathEntry head\n");
    PathEntryPtr pe = (PathEntryPtr)&cli->cli_CommandDir;

    PutStr("  [-] Allocating memory for new PathEntry\n");
    PathEntryPtr new = AllocVec(sizeof(PathEntry), MEMF_ANY);
    if (!new) {
      PutStr("  [!] Failed to allocate memory\n");
      return RETURN_FAIL;
    }

    PutStr("  [-] Getting lock on RAM:\n");
    lock = Lock("RAM:", SHARED_LOCK);
    if (!lock) {
      PutStr("  [!] Failed to get lock\n");
      return RETURN_FAIL;
    }

    NameFromLock(lock, buffer, sizeof(buffer));
    printf("    [D] Name from lock is: %s\n", buffer);

    if (is_directory(lock, DOSBase) {
      PutStr("  [I] Setting path now...\n");
      new->lock = lock;

      NameFromLock(new->lock, buffer, sizeof(buffer));
      printf("    [D] New Lock name is %s\n", buffer);
        
    } else {
      PutStr("  [-] Freeing memory for new PathEntry\n");
      if (new != NULL) {
        FreeVec(new);
      }
    }

    UnLock(lock);


    PutStr("[+] Dumping new path\n");
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
