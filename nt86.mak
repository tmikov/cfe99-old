
MFLAGS=

!ifdef debug
MFLAGS=$(MFLAGS) debug=1
!endif

all: repl_ prebis_


repl_:
  cd utils\repl
  $(MAKE) -f nt86.mak $(MFLAGS)
  cd ..\..

prebis_:
  cd prebis
  $(MAKE) -f nt86.mak $(MFLAGS)
  cd ..

