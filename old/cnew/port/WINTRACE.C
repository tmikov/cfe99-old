#include <windows.h>
#include <stdarg.h>

void debug_trace ( const char * fmt, ... )
{
  int nBuf;
  char szBuffer[512];

  va_list args;
  va_start(args, fmt);

  nBuf = wvsprintf(szBuffer, fmt, args);

  OutputDebugString(szBuffer);
  va_end(args);
}
