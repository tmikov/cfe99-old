#ifndef DEBUG_H
#define DEBUG_H


#ifdef _DEBUG
void debug_trace ( const char * fmt, ... );

#  define VERIFY( x )       ASSERT( x )
#  define TRACE0( x )       debug_trace( x )
#  define TRACE1( x, a1 )   debug_trace( x, a1 )
#  define TRACE2( x, a1, a2 )  debug_trace( x, a1, a2 )
#  define TRACE3( x, a1, a2, a3 )  debug_trace( x, a1, a2, a3 )

#else
#  define VERIFY( x )                 (x)
#  define TRACE0( x )                 ((void)0)
#  define TRACE1( x, a1 )             ((void)0)
#  define TRACE2( x, a1, a2 )         ((void)0)
#  define TRACE3( x, a1, a2, a3 )     ((void)0)
#endif



#endif // DEBUG_H
