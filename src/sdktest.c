#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include "utility.h"


int main()
{
  /*
  IDOS->Printf("\n");
  UBYTE *linkname = "RAM:link";
  int32 success = IDOS->MakeLink(linkname,"RAM:dest",LINK_SOFT);
  if (success) {
    IDOS->Printf("Made link!\n");
  }
  */
  struct CommandLineInterface *cli = (struct CommandLineInterface *)IDOS->Cli();
  BSTR cwd;
  cwd = cli->cli_CurrentDirName;

  struct PathNode *pathlist;
  pathlist = BADDR(cli->cli_PathList);
  IDOS->Printf("---> Before\n");
  DumpPathNode(pathlist);

  BPTR lock = IDOS->Lock(SETCMD_PATH, SHARED_LOCK);
  if (lock) {
    IDOS->Printf("Obtained lock on " SETCMD_PATH "\n");
    struct Pathnode *check_dup = IDOS->RemoveCmdPathNode(pathlist, lock);
    if (check_dup) {
      IDOS->Printf("Found and removed existing SetCmd paths\n");
      DumpPathNode(check_dup);
      pathlist=check_dup;
    }
    struct PathNode *new_path = IDOS->AddCmdPathNode(pathlist, lock, ADDCMDPATHNODE_HEAD);
    IDOS->Printf("---> After\n");
    DumpPathNode(new_path);

    if (IDOS->SetCurrentCmdPathList(new_path)) {
      IDOS->Printf("Set Path\n");
    }
  } else {
    IDOS->Printf("ERROR: Failed to lock the " SETCMD_PATH " directory\n");
    IDOS->Printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    IDOS->Printf("For more information see the SetCmd manual.\n");
  }

  return 0;
} 

