void __attr(wrong) func1 ( void ); // @err: unrecognized __attr
void __attr(cdecl) func2 ( void );
void func2 ( void );               // @err: type is incompatible with previous declaration
void __attr(cdecl) __attr(cdecl) func3 ( void ); // @err: More than one calling convention attribute
void __attr(cdecl) __attr(stdcall) func4 ( void ); // @err: More than one calling convention attribute

int __attr(far)  v1;
int __attr(far)  __attr(far) v2; // @err: More than one pointer size attribute
int __attr(near) __attr(far) v3; // @err: More than one pointer size attribute
int __attr(near)  v4;
int v4;                          // @err: type is incompatible with previous declaration

