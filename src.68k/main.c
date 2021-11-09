#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "list.h"
#include "add_cmd.h"
#include "add_version.h"
#include "delete_version.h"

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
  rd = ReadArgs(template, args, NULL);
  if (rd == NULL) {
    // No args could be passed, bomb out`
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
        printf("%s\n", SETCMD_VERSION);
        rc = RETURN_OK;
      }
      
      // list
      else if (strcmp(cmd, "list") == 0) {
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


      // add-cmd
      else if (strcmp(cmd, "add-cmd") == 0) {
        if (arg1) {
          rc = add_cmd(arg1);
        } else {
          usage();
        }
      }

      // add-version
      else if (strcmp(cmd, "add-version") == 0) {
        if (arg3) {
          rc = add_version(arg1, arg2, arg3);
        } else {
          usage();
        }
      }

      // delete-version
      else if (strcmp(cmd, "delete-version") == 0) {
        if (arg2) {
          rc = delete_version(arg1, arg2);
        } else {
          usage();
        }
      }

    } else {
      // No command given, just display usage
      usage();
    }
  }

  // Clean up and exit
  if (rd != NULL) {
    FreeArgs(rd);
    rd = NULL;
  }

  return RETURN_OK;
}
