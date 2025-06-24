#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

// struct definitions

#if !defined(__amigaos4__)
/* Couldn't find this documented anywhere but there are several references to
   it from various sources, e.g. AROS
*/
struct PathNode {
  BPTR next;
  BPTR lock;
};
#endif

// SetCmd version string
#if defined(__amigaos4__)
  #define ARCH_TAG "-amigaos4-ppc"
#else
  #define ARCH_TAG "-amigaos3-68k"
#endif
#define SETCMD_SEMVER  "2.0.0"
#define SETCMD_VERSION SETCMD_SEMVER ARCH_TAG

#define SETCMD_BANNER   "(c) Mark Dastmalchi-Round [markround.com/amiga]"

// Text formatting
#define FG_BLACK    "\x1b[31m"
#define FG_WHITE    "\x1b[32m"
#define FG_BLUE     "\x1b[33m"
#define FG_RED      "\x1b[34m"
#define BG_BLUE     "\x1b[43m"
#define BG_DEFAULT  "\x1b[40m"
#define BOLD_ON     "\x1b[1m"
#define BOLD_OFF    "\x1b[22m"
#define ITALIC_ON   "\x1b[3m"
#define ITALIC_OFF  "\x1b[23m"
#define SELECTED    BG_BLUE FG_WHITE
#define NORMAL      BG_DEFAULT FG_BLACK BOLD_OFF ITALIC_OFF

// Constants
#ifndef DEBUG
#define DEBUG         FALSE         // Set to TRUE for Debug builds
#endif

#define MAX_PATH_BUF  2048          // Max length of an AmigaDOS path
#define SETCMD_ASSIGN "SETCMD:"     // Location of main assign
#define SETCMD_PATH   "SETCMD:path" // Location of cmd symlinks
#define SETCMD_CMDS   "SETCMD:cmds" // Location of cmd versions
#define SETCMD_STUB   "SETCMD:stub" // Location of stub binary
#define SETCMD_ERROR  0             // Internal return value of functions that fail
#define SETCMD_OK     1             // Internal return value of functions that succeed

#define SETCMD_NOFORMAT_VAR "SETCMD_NOFORMAT" // Env var that controls formatting

// Used for option parsing flags
#define OPT_NONE    0
#define OPT_VERBOSE 1
#define OPT_QUIET   2

// Helper function definitions

void utility_test();
#if !defined(__amigaos4__)
BOOL is_directory(BPTR lock);
#endif
BOOL path_is_directory(char *path);
char *fmt(char *fmt_string);
BOOL can_lock(const char *path); 
int get_target(const char *cmd, const char *version, char *target);
int current_version(const char *cmd, char *version);

#endif
