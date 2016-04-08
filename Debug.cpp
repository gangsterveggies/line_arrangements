#include "Debug.h"

int Debug::disabled = 0;

void Debug::print(const char* format, ...)
{
  if (disabled)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}
