#ifndef _DEBUG_
#define _DEBUG_

#include "Common.h"
#include <stdarg.h>

class Debug
{
public:
  static void print(const char* format, ...);
  static int disabled;

private:
};

#endif
