#include <proto/dos.h>
#include <stdio.h>
#include "utility.h"
#include "usage.h"
#include "init.h"

int main (int argc, char const *argv[]) 
{
  if (argc <= 1) {
    usage();
    return 0;
  }

  if (strcmp(argv[1], "usage") == 0) { 
    usage(); 
  }
  else if (strcmp(argv[1], "init") == 0) { 
    init(); 
  }
  else {
    usage();
  }  

  return 0;
}
