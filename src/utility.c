#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include "utility.h"

STRPTR btos(BSTR bstr)
{
  return IUtility->ASPrintf("%b", bstr);
}


void DumpPathNode(struct PathNode *node)
{
  while (node->pn_Next) {
    char path[MAX_PATH_BUF];
    if(IDOS->NameFromLock(node->pn_Lock, path, MAX_PATH_BUF)) {
      IDOS->Printf("%s\n", path);
    }
    node = BADDR(node->pn_Next);
  }
}
