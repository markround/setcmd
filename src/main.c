#include <proto/dos.h>
#include <stdio.h>
#include "utility.h"
#include "usage.h"

int main (int argc, char const *argv[]) 
{
  if (argc <= 1) {
    usage();
    return 0;
  }

  if (strcmp(argv[1], "usage") == 0) { 
    usage(); 
  }
  else {
    usage();
  }  

  return 0;
}
