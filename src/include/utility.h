#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <proto/exec.h>

// SetCmd version string
#define SC_VERSION  "1.0.0"

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


// Helper functions
STRPTR btos(BSTR bstr);

#endif
