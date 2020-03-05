#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "init.h"
#include "list.h"

// Used by the version DOS command
const char __ver[40] =  "$VER: SetCmd " SETCMD_VERSION;

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
  const char *cmd;
  const char *arg1,*arg2,*arg3,*arg4;
  int rc = RETURN_OK;

  memset(args, 0, sizeof(args));

  // TODO : Check we are run from the CLI (count argc)
  rd = IDOS->ReadArgs(template, args, NULL);
  if (rd == NULL) {
    // No args could be passed, bomb out
    IDOS->PrintFault(IDOS->IoErr(), NULL);
    return RETURN_FAIL;
  } else {
    // Begin options parsing
    cmd   = (const char *)args[ARG_CMD];
    arg1  = (const char *)args[ARG_1];
    if (cmd) {

      /*
       * version
       */
      if (strstr(cmd, "version")) {
        IDOS->Printf("%s\n", SETCMD_VERSION);
        rc = RETURN_OK;
      }

      /*
       * init
       */
      if (strstr(cmd, "init")) {
        if (arg1) {
          if (strstr(arg1, "verbose")) {
            rc = init(OPT_VERBOSE);
          } else if (strstr(arg1, "quiet")) {
            rc = init(OPT_QUIET);
          } else {
            usage();
          }
        } else {
          rc = init(OPT_NONE);
        }
      }

      /*
       * list
       */
      if (strstr(cmd, "list")) {
        if (arg1) {
          if (strstr(arg1, "verbose")) {
            rc = list(OPT_VERBOSE);
          } else {
            usage();
          }
        } else {
          rc = list(OPT_NONE);
        }
      }

      /*
       * show
       */
      if (strstr(cmd, "show")) {
        if (arg1) {
          rc = show(arg1);
        } else {
          usage();
        }
      }

    } else {
      // No command passed, so display usage screen
      usage();
    }
  }

  // Clean up and exit
  if (rd != NULL) {
    IDOS->FreeArgs(rd);
    rd = NULL;
  }
  return RETURN_OK;
}
