enum Enum1
{
  a = sizeof(enum Enum1)  // @err: Enum1 is not a complete type
};


struct Incomplete;

void func ( struct Incomplete a[] ); // @err: Array of incomplete type is not allowed

