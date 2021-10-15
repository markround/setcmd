#include "utility.h"
#include <stdio.h>
#include <proto/dos.h>
#include <dos/dos.h>

void utility_test()
{
  printf("Utilities loaded\n");
}

BOOL is_directory(BPTR lock, APTR DOSBase)
{
  BOOL isDirectory = FALSE;
    
  struct FileInfoBlock* fib = AllocDosObject(DOS_FIB, NULL);
    
  if (Examine(lock, fib)) {
    LONG entryType = fib->fib_EntryType;

    if (entryType >= ST_ROOT && entryType <= ST_LINKDIR) {
      if (entryType != ST_SOFTLINK) {
          isDirectory = TRUE;
      }
      else {
        BPTR dupLock = DupLock(lock);    
        if (dupLock) {
          BPTR file = OpenFromLock(dupLock);          
          if (file) {
            /* lock was on a file. dupLock is relinquished by OpenFromLock */
            Close(file);
                       
            isDirectory = FALSE;
          }
          else {
            UnLock(dupLock);
          }
        }
        else {
          isDirectory = FALSE;
        }
      }
    }

    FreeDosObject(DOS_FIB, fib);
  }  

  return(isDirectory);
}
