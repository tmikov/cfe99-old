#define FIRST_TRIGRAPH 33
#define NUM_TRIGRAPHS  30
static char TrigraphTable[NUM_TRIGRAPHS] =
{
  '|',0,0,0,0,0,'^','[',']',0,0,0,'~',0,'\\',0,0,0,0,0,0,0,0,0,0,0,0,'{','#','}'
};

#define FIRST_ESCAPE 97
#define NUM_ESCAPES  22
static char EscapeTable[NUM_ESCAPES] =
{
  '\a','\b',0,0,0,'\f',0,0,0,0,0,0,0,'\n',0,0,0,'\r',0,'\t',0,'\v'
};

