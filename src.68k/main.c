#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "usage.h"
#include "init.h"
#include "list.h"
#include "add_cmd.h"
#include "add_version.h"
#include "delete_version.h"
#include "delete_cmd.h"
#include "set_version.h"
#include "show.h"

// Used by the version DOS command
const char __ver[40] = "$VER: SetCmd " SETCMD_VERSION;

// Command function pointer type
typedef int (*cmd_func_t)(const char**, int);

// Structure for lookup table
struct command_entry {
  const char *name;
  int min_args;
  int max_args;
  cmd_func_t handler;
  const char *usage_hint;
};

// Handler functions
static int handle_version(const char **args, int argc) {
  if (argc > 0 && args[0]) {
    if (strcmp(args[0], "semver") == 0) {
      printf("%s\n", SETCMD_SEMVER);
    } else if (strcmp(args[0], "arch") == 0) {
      printf("%s\n", ARCH_TAG + 1);
    } else {
      printf("Invalid option for version: %s\n", args[0]);
      usage();
      return RETURN_FAIL;
    }
  } else {
    printf("%s\n", SETCMD_VERSION);
  }
  return RETURN_OK;
}


static int handle_init(const char **args, int argc) {
  int opt = OPT_NONE;
  
  if (argc > 0 && args[0]) {
    if (strcmp(args[0], "verbose") == 0) {
      opt = OPT_VERBOSE;
    } else if (strcmp(args[0], "quiet") == 0) {
      opt = OPT_QUIET;
    } else {
      printf("Invalid option for init: %s\n", args[0]);
      usage();
      return RETURN_FAIL;
    }
  }
  
  return init(opt);
}

static int handle_list(const char **args, int argc) {
  int opt = OPT_NONE;
  
  if (argc > 0 && args[0]) {
    if (strcmp(args[0], "verbose") == 0) {
      opt = OPT_VERBOSE;
    } else {
      printf("Invalid option for list: %s\n", args[0]);
      usage();
      return RETURN_FAIL;
    }
  }
  
  return list(opt);
}

static int handle_add_cmd(const char **args, int argc) {
  return add_cmd(args[0]);
}

static int handle_delete_cmd(const char **args, int argc) {
  return delete_cmd(args[0]);
}

static int handle_add_version(const char **args, int argc) {
  return add_version(args[0], args[1], args[2]);
}

static int handle_delete_version(const char **args, int argc) {
  return delete_version(args[0], args[1]);
}

static int handle_set_version(const char **args, int argc) {
  return set_version(args[0], args[1]);
}

static int handle_show(const char **args, int argc) {
  return show(args[0]);
}

// Command lookup table
// command , min args , max args , handler, help hint
static const struct command_entry commands[] = {
  {"version",         0, 1, handle_version,    "[semver|arch]"},
  {"init",            0, 1, handle_init,     "[quiet|verbose]"},
  {"list",            0, 1, handle_list,     "[verbose]"},
  {"add-cmd",         1, 1, handle_add_cmd,    "<cmd>"},
  {"delete-cmd",      1, 1, handle_delete_cmd,   "<cmd>"},
  {"add-version",     3, 3, handle_add_version,  "<cmd> <version> <target>"},
  {"delete-version",  2, 2, handle_delete_version, "<cmd> <version>"},
  {"set-version",     2, 2, handle_set_version,  "<cmd> <version>"},
  {"show",            1, 1, handle_show,     "<cmd>"},
  {NULL,              0, 0, NULL, NULL} // Sentinel
};

// Parameter template
static const char template[] =
  "COMMAND,"
  "ARG1,"
  "ARG2,"
  "ARG3,"
  "ARG4";

enum {
  ARG_COMMAND,
  ARG_1,
  ARG_2,
  ARG_3,
  ARG_4,
  NUM_ARGS
};

// Helper function to find command in lookup table
static const struct command_entry* find_command(const char *cmd_name) {
  const struct command_entry *cmd = commands;
  
  while (cmd->name != NULL) {
    if (strcmp(cmd->name, cmd_name) == 0) {
      return cmd;
    }
    cmd++;
  }
  return NULL;
}

// Helper function to validate argument count
static BOOL validate_args(const struct command_entry *cmd, const char **args, int argc) {
  if (argc < cmd->min_args) {
    printf("Error: '%s' requires at least %d argument(s)\n", 
         cmd->name, cmd->min_args);
    printf("Usage: setcmd %s %s\n", cmd->name, cmd->usage_hint);
    return FALSE;
  }
  
  if (argc > cmd->max_args) {
    printf("Error: '%s' accepts at most %d argument(s)\n", 
         cmd->name, cmd->max_args);
    printf("Usage: setcmd %s %s\n", cmd->name, cmd->usage_hint);
    return FALSE;
  }
  
  return TRUE;
}

int main(int argc, char const *argv[])
{
  struct RDArgs *rd = NULL;
  LONG args[NUM_ARGS];
  const char *cmd_name;
  const char *cmd_args[4]; // Max 4 arguments for any command
  int cmd_argc = 0;
  int rc = RETURN_OK;
  const struct command_entry *cmd_entry;

  memset(args, 0, sizeof(args));
  memset(cmd_args, 0, sizeof(cmd_args));

  // Read arguments using template
  rd = ReadArgs(template, args, NULL);
  if (rd == NULL) {
    PrintFault(IoErr(), "SetCmd");
    return RETURN_FAIL;
  }

  cmd_name = (const char *)args[ARG_COMMAND];
  if (!cmd_name) {
    usage();
    rc = RETURN_FAIL;
    goto cleanup;
  }

  // Build argument array for the command
  if (args[ARG_1]) cmd_args[cmd_argc++] = (const char *)args[ARG_1];
  if (args[ARG_2]) cmd_args[cmd_argc++] = (const char *)args[ARG_2];
  if (args[ARG_3]) cmd_args[cmd_argc++] = (const char *)args[ARG_3];
  if (args[ARG_4]) cmd_args[cmd_argc++] = (const char *)args[ARG_4];

  // Find command in lookup table
  cmd_entry = find_command(cmd_name);
  if (!cmd_entry) {
    printf("Unknown command: %s\n", cmd_name);
    usage();
    rc = RETURN_FAIL;
    goto cleanup;
  }

  // Validate argument count
  if (!validate_args(cmd_entry, cmd_args, cmd_argc)) {
    rc = RETURN_FAIL;
    goto cleanup;
  }

  // Execute the command
  rc = cmd_entry->handler(cmd_args, cmd_argc);

cleanup:
  if (rd != NULL) {
    FreeArgs(rd);
  }

  return rc;
}
