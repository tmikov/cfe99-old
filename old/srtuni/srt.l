%option noyywrap

XDIGIT  [0-9a-fA-F]
XQUAD   ({XDIGIT}{XDIGIT}{XDIGIT}{XDIGIT})
WS      [ \t]

%%
  unsigned l, h;

{XQUAD}-{XQUAD}  {
                   sscanf( yytext, "%x-%x", &l, &h ); 
                   printf( "  {0x%04x, 0x%04x},\n", l, h );
                 }
{XQUAD}          {
                   sscanf( yytext, "%x", &l ); 
                   printf( "  {0x%04x, 0x%04x},\n", l, l );
                 }

\n
.



%%
