#ifndef CONSTSTR_H
#define CONSTSTR_H

typedef struct
{
  TListEntry link;
  unsigned refCnt;
  unsigned size;  // length including the zero
} CONST_STR_HEAD;

void ConstStr_Init ( void );
void ConstStr_Done ( void );

const char * AllocConstStrLen ( const char * str, unsigned strLen );
const char * AllocConstString ( const char * str );
void         FreeConstString ( const char * str );
const char * CopyConstString ( const char * str );

/*
//--------------------------------------------------------------------------
// Name         ConstStringSize
//
//
//--------------------------------------------------------------------------
unsigned ConstStringSize ( const char * str )
{
  return ((CONST_STR_HEAD *)str - 1)->size;
};
*/
#define ConstStringSize( str )  (((const CONST_STR_HEAD *)str - 1)->size)

#endif // CONSTSTR_H
