#ifdef _DEBUG

#include "ulibpriv.h"
#include "debug.h"

#if defined(WIN32) || defined(_WIN32)

#include <windows.h>

void debug_trace ( const char * fmt, ... )
{
  int nBuf;
  char szBuffer[512];

  va_list args;
  va_start(args, fmt);

  nBuf = vsprintf(szBuffer, fmt, args);

  OutputDebugString(szBuffer);
  va_end(args);
}

#endif // WIN32

#endif // _DEBUG
