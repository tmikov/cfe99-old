/*
  Host configuration
*/
#ifndef HCONF_H
#define HCONF_H

typedef union AlignAll
{
  long double ld;
  long x;
  void * p;
  void (*pFunc)(void);
} AlignAll;

#endif
