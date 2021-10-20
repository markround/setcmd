#include "utility.h"
#include <stdio.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

void utility_test()
{
  printf("Utilities loaded\n");
}

void dump_current_path()
{
}


BOOL is_directory(BPTR lock)
{
  BOOL is_directory = FALSE;
    
  struct FileInfoBlock* fib = AllocDosObject(DOS_FIB, NULL);
    
  if (Examine(lock, fib)) {
    LONG entry_type = fib->fib_EntryType;

    if (entry_type >= ST_ROOT && entry_type <= ST_LINKDIR) {
      if (entry_type != ST_SOFTLINK) {
        is_directory = TRUE;
      }
      else {
        BPTR lock_copy = DupLock(lock);    
        if (lock_copy) {
          BPTR file = OpenFromLock(lock_copy);          
          if (file) {
            // lock was on a file, it's now been
            // relinquished when we opened it with OpenFromLock
            Close(file);                       
            is_directory = FALSE;
          }
          else {
            UnLock(lock_copy);
          }
        }
        else {
          is_directory = FALSE;
        }
      }
    }

    FreeDosObject(DOS_FIB, fib);
  }  

  return(is_directory);
}
