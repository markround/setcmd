#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <proto/exec.h>

// SetCmd version string
#define SC_VERSION      "1.0.0c"
#define SC_BANNER       "(c) 2020 Mark Dastmalchi-Round   [http://markround.com/amiga]"

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

// Constants
#define DEBUG         FALSE         // Set to TRUE for Debug builds
#define MAX_PATH_BUF  1024          // Max length of an AmigaDOS path
#define SETCMD_PATH   "SETCMD:path" // Location of cmd symlinks

#define OPT_NONE    0
#define OPT_VERBOSE 1
#define OPT_QUIET   2


// Helper functions
STRPTR btos(BSTR bstr);
void DumpPathNode(struct PathNode *node);

#endif
