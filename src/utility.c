#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include "utility.h"

STRPTR btos(BSTR bstr)
{
  return IUtility->ASPrintf("%b", bstr);
}
