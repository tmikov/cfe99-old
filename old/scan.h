#ifndef SCAN_H
#define SCAN_H

#include "strings.h"

typedef struct yyltype
{
  STRING fileName;
  unsigned short line, col;
} yyltype;

#define YYLTYPE yyltype

#endif
