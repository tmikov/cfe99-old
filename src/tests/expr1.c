void func ( void );
void (*p)(void);

void test ( void )
{
  p = func;     
  p = &func;    
  p = &*func;
  p = *&func;

  & & func;      // @err
  p = ***func; 

  func();       
  (*func)();
  (*&func)();
  (&*func)();
}
