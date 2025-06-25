#include <exec/types.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/exec.h>
#include <dos/dosextens.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"

int delete_cmd(const char *cmd) 
{
  BPTR lock;
  char cmd_dir[MAX_PATH_BUF];
  char path_entry[MAX_PATH_BUF];
  char path[MAX_PATH_BUF];
  char version[MAX_PATH_BUF];
  char version_path[MAX_PATH_BUF];
  struct FileInfoBlock path_data;
  int rc = 0;
  int cmd_rc = RETURN_OK;
#if defined(__amigaos4__)
  APTR path_context;
  struct ExamineData *path_data;
#endif

  // Sanity check, make sure we can access the SETCMD:cmds directory
  if (!can_lock(SETCMD_PATH)) {
    printf("%sERROR %s: Failed to lock the %s directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    printf("Check your installation and make sure the SETCMD: assign is correctly setup.\n");
    printf("For more information see the SetCmd manual.\n");
    return SETCMD_ERROR;
  }
  
  // Now try and lock the requested cmd directory to see if it already exists
  strcpy(cmd_dir, SETCMD_CMDS);
  AddPart(cmd_dir, cmd, MAX_PATH_BUF);

  if (DEBUG) {
    printf("Locking %s\n", cmd_dir);
  }

  // First get the data for the cmd dir
  // Test it is a dir first
  if (!path_is_directory(cmd_dir)) {
    printf("%sERROR %s: cmd dir %s does not appear to be a directory\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    cmd_rc = RETURN_FAIL;
    goto cleanup;  
  }

  lock = Lock(cmd_dir, ACCESS_READ);
  if (!lock) {
    printf("%sERROR %s: The command %s does not exist.\n", fmt(FG_RED), fmt(NORMAL), cmd);
    return RETURN_FAIL;
  } 

  // Iterate over all the installed versions and delete them

  // AmigaOS4-style iteration
#if defined(__amigaos4__)
  path_context = ObtainDirContextTags(EX_LockInput, lock, TAG_END);
  while (path_data = ExamineDir(path_context)) {
    strcpy(path, SETCMD_CMDS);
    AddPart(path, cmd, MAX_PATH_BUF);
    AddPart(path, path_data->Name, MAX_PATH_BUF);
    if (!Delete(path)) {
      printf("%sERROR %s: Unexpected error when deleting %s.\n", fmt(FG_RED), fmt(NORMAL), path);
      ReleaseDirContext(path_context);
      UnLock(lock);
      return RETURN_FAIL;
    }
  }
  ReleaseDirContext(path_context);

  // AmigaOS3-style iteration
#else
  rc = Examine(lock, &path_data);
  if (!rc) {
    printf("%sERROR %s: Unexpected error when examining cmd dir %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

  // Now loop over all directory entries
  while (ExNext(lock, &path_data)) {
    // Extract the cmd name from path_data struct
    strcpy (version, path_data.fib_FileName);
    
    // Get the full path to the version
    strcpy(version_path, SETCMD_CMDS);
    AddPart(version_path, cmd, MAX_PATH_BUF);
    AddPart(version_path, version, MAX_PATH_BUF);

    if (DEBUG) {
      printf("Deleting %s\n", version_path);
    }

    // Delete the version link
    if (path_is_directory(version_path)) {
      printf("%sERROR %s: %s is a directory.\n", fmt(FG_RED), fmt(NORMAL), version_path);
      cmd_rc = RETURN_FAIL;
      goto cleanup;
    }

    rc = DeleteFile((char *)version_path);
    if (!rc) {
      printf("%sERROR %s: unexpected error deleting link %s.\n", fmt(FG_RED), fmt(NORMAL), version_path);  
    }
  }
#endif

  UnLock(lock);
  lock = (BPTR)NULL;

  if (DEBUG) {
    printf("Finished iterating over versions\n");
  }

  // Now delete the cmd directory itself
  if (DEBUG) {
    printf("Deleting %s\n", cmd_dir);
  }
  
#if defined(__amigaos4__)
  rc = Delete((char *) cmd_dir);
#else
  rc = DeleteFile((char *) cmd_dir);
#endif

  if (!rc) {
     printf("%sERROR %s: Unexpected error when deleting cmd dir %s.\n", fmt(FG_RED), fmt(NORMAL), cmd_dir);
     return RETURN_FAIL;
  }

  // And delete the link in the path dir
  strcpy(path_entry, SETCMD_PATH);
  AddPart(path_entry, cmd, MAX_PATH_BUF);
  if (DEBUG) {
    printf("Deleting %s\n", path_entry);
  }

  // Test it's not a directory
  if (path_is_directory(path_entry)) {
    printf("%sERROR %s: %s is a directory.\n", fmt(FG_RED), fmt(NORMAL), path_entry);
    cmd_rc = RETURN_FAIL;
    goto cleanup;
  }

#if defined(__amigaos4__)
  rc = Delete((char *) path_entry);
#else
  rc = DeleteFile((char *) path_entry);
#endif

  if (!rc) {
     printf("%sERROR %s: Unexpected error when deleting path link %s.\n", fmt(FG_RED), fmt(NORMAL), path_entry);
     return RETURN_FAIL;
  }

  if (DEBUG) {
    printf("Finished deleting files\n");
  }

cleanup:
  if (lock)  { UnLock(lock); }

  return cmd_rc;
}
