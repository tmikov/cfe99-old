/*
  C99 <stdbool.h> for all platforms
*/
#ifndef STDBOOL_H
#define STDBOOL_H

typedef unsigned char _Bool;

#ifndef __cplusplus

#define bool  _Bool
#define true  1
#define false 0
#define __bool_true_false_are_defined  1

#endif

#endif // STDBOOL
