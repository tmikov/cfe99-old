
CC=cl
CFLAGS=-W3 -D_DEBUG -DYYDEBUG=1 -MDd -D__STDC__ -Zi
#CFLAGS=-W3 -MD -D__STDC__ -O1
BISON=bison
FLEX=flex
#FFLAGS=-F -b
FFLAGS= -b

COCOM_FILES = arithm.obj ieee.obj bits.obj

FILES= scan.obj strings.obj heap.obj cord.obj options.obj tconf.obj \
       hexdbl.obj comperr.obj c.tab.obj \
       prepr.tab.obj scanp.obj main.obj \
       $(COCOM_FILES)
       

c99.exe: $(FILES)
  $(CC) $(CFLAGS) -Fec99.exe $** gc\debug\gc.lib

c.tab.h c.tab.c: c.y
  $(BISON) -v -d c.y

scan.c: c.tab.h scan.l
  $(FLEX) $(FFLAGS) scan.l

prepr.tab.c prepr.tab.h: prepr.y
  $(BISON) -d -ppp prepr.y

scanp.c: prepr.tab.h scanp.l
  $(FLEX) $(FFLAGS) scanp.l

#
# COCOM
#

arithm.obj: cocom\ammunition\arithm.c
  $(CC) -c $(CFLAGS) $**  

ieee.obj: cocom\ammunition\ieee.c
  $(CC) -c $(CFLAGS) $**  

bits.obj: cocom\ammunition\bits.c
  $(CC) -c $(CFLAGS) $**  


