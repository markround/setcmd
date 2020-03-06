#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "init.h"
#include "list.h"
#include "show.h"
#include "add_cmd.h"
#include "delete_cmd.h"
#include "add_version.h"

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
    arg2  = (const char *)args[ARG_2];
    arg3  = (const char *)args[ARG_3];
    arg4  = (const char *)args[ARG_4];
    if (cmd) {

      // version
      if (strcmp(cmd, "version") == 0) {
        IDOS->Printf("%s\n", SETCMD_VERSION);
        rc = RETURN_OK;
      }

      // init
      if (strcmp(cmd, "init") == 0) {
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

      // list
      if (strcmp(cmd, "list") == 0) {
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

      // show
      if (strcmp(cmd, "show") == 0) {
        if (arg1) {
          rc = show(arg1);
        } else {
          usage();
        }
      }

      // add-cmd
      if (strcmp(cmd, "add-cmd") == 0) {
        if (arg1) {
          rc = add_cmd(arg1);
        } else {
          usage();
        }
      }

      // delete-cmd
      if (strcmp(cmd, "delete-cmd") == 0) {
        if (arg1) {
          rc = delete_cmd(arg1);
        } else {
          usage();
        }
      }

      // add-version
      if (strcmp(cmd, "add-version") == 0) {
        if (arg3) {
          rc = add_version(arg1, arg2, arg3);
        } else {
          usage();
        }
      }

      // delete-version
      if (strcmp(cmd, "delete-version") == 0) {
        if (arg2) {
          rc = add_version(arg1, arg2);
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
