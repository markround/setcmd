#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"

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
    printf("Hello, world.\n");
    return RETURN_OK;
}