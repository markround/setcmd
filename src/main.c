#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "init.h"

// Used by the version DOS command
const char __ver[40] =  "$VER: SetCmd " SC_VERSION;

static const char template[] =
	"CMD,"
	"ARG1,"
	"ARG2,"
	"ARG3,"
	"ARG4";

enum {
  ARG_CMD,
  ARG_1,
  ARG_2,
  ARG_3,
  ARG_4,
  NUM_ARGS
};

int main (int argc, char const *argv[]) 
{

  struct RDArgs *rd = NULL;
  LONG args[NUM_ARGS];
  memset(args, 0, sizeof(args));
  const char *cmd;
  const char *arg1;
  const char *arg2;
  const char *arg3;
  const char *arg4;

  // TODO : Check we are run from the CLI (count argc)


  rd = IDOS->ReadArgs(template, args, NULL);
  if (rd == NULL) {
    // No args could be passed, bomb out
    // TODO: What should the return codes be here ?
    IDOS->PrintFault(IDOS->IoErr(), NULL);
    return 1;
  }
  else {
    // Begin options parsing
    cmd   = (const char *)args[ARG_CMD];
    arg1  = (const char *)args[ARG_1];
    if (cmd) {

      // init
      if (strstr(cmd, "init")) {
        if (arg1) {
          if (strstr(arg1, "verbose")) {
            init(OPT_VERBOSE);
          }
          else if (strstr(arg1, "quiet")) {
            init(OPT_QUIET);
          }
          else {
            usage();
          }
        }
        else {
          init(OPT_NONE);
        }
      }

      // Next cmd

    }
    else {
      // No command passed, so display usage screen
      usage();
    }
  }


  // Clean up and exit
  if (rd != NULL)
	{
		IDOS->FreeArgs(rd);
		rd = NULL;
	}
  return 0;
}
