#include "port\global.h"
#include "c_tab.h"

// Definitions required by "cwords.inc"
#define RESWORDACTION_FUNC static FORCEINLINE int ResWordAction ( int index )
#define RET_ITOK( tok, lex )  return (lex)

typedef struct
{
  const char * text;
  int code;
} TRESWORD;

#include "cwords.inc"

//--------------------------------------------------------------------------
// Name         ScannerCheckResWord
//
// Returns 0 if this is not a C reserved word
// Otherwise returns the code (and executes associated actions)
//--------------------------------------------------------------------------
int ScannerCheckResWord ( const char * szIdent, int len )
{
  if (len >= RES_C_WORD_MIN_LEN && len <= RES_C_WORD_MAX_LEN)
  {
    int hash;

    // we have certain assumptions about C reserved words that decrease
    // table size

    ASSERT( szIdent[0] >= '0' && szIdent[0] <= 'z' );
    ASSERT( szIdent[len - 1] >= '0' &&
            szIdent[len - 1] <= 'z' );

    // calculate perfect hash value

    hash = HashCharValue[ szIdent[0] - '0' ] +
           HashCharValue[ szIdent[len - 1] - '0' ] +
           ((szIdent[1] - '_' + 1) & 7);

    // note: len + 1 is not very big

    if (hash >= 0 && hash < _countof( ResWordsTable ) &&
//        LexIdentHash == ResWordsTable[hash].Hash &&
        memcmp( ResWordsTable[hash].text, szIdent, len + 1 ) == 0)
    {
      int lex;
      return
        (lex = ResWordsTable[hash].code) < 0 ? ResWordAction( -lex ) : lex;
    }
  }

  return 0;
};

