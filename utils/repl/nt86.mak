repl.exe: repl.c
  $(CC) -O1 -MD $**

clean:
  del *.obj

cleanall: clean
  del *.exe
