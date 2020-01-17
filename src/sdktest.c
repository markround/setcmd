#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
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
  BSTR path;
  path = cli->cli_CurrentDirName;
  IDOS->Printf("String version of path : %s\n", btos(path));
  return 0;
} 

