OUT=out

CC=cl
BSC=bscmake
CDEFINES=-D_DEBUG -DYYDEBUG=1 -DUSE_GC
CFLAGS=$(CDEFINES) -W3 -I.. -I. -Itarget\i386\cfe -Fr$(OUT)\\

!IFDEF debug 
CFLAGS=$(CFLAGS) -MDd -Zi
!ELSE
CFLAGS=$(CFLAGS) -MD
!ENDIF

PREBIS_LIB=..\prebis\main
PREBIS=..\prebis\out\prebis.exe
PBFLAGS=

BISON=bison
BFLAGS=-v 

FLEX=flex
FFLAGS=

M4=m4
M4FLAGS=

REPL=..\utils\repl\repl.exe

TGT=i386

FILES=

##############################################################

all: $(OUT)\c99c.bsc $(OUT)\c99c.exe

clean:
  @del *.pdb;*.pch;*.ilk;*.obj;*.exe;*.sbr;*.bsc /s
  @del lex.backup /s
  @del c.output /s
  

##############################################################
#
# ulib\c
#
##############################################################
FILES=$(FILES)\
  $(OUT)\debug.obj        \
  $(OUT)\cord.obj         \
  $(OUT)\clist.obj        \
  $(OUT)\simgc.obj        \
  $(OUT)\strings.obj

{..\ulib\c}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -I..\ulib\h -Fo$(OUT)\ -c $<

##############################################################
#
# ulib\avl-0.4
#
##############################################################
FILES=$(FILES)\
  $(OUT)\avl.obj

{..\ulib\avl-0.4}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c $<

##############################################################
#
# Cocom
#
##############################################################
FILES=$(FILES)\
  $(OUT)\arithm.obj\
  $(OUT)\ieee.obj\
  $(OUT)\bits.obj

{..\cocom\ammunition}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -W2 $<

##############################################################
#
# common
#
##############################################################
FILES=$(FILES)

{common\c}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Icommon\h -Fo$(OUT)\ -c $<

##############################################################
#
# target\c
#
##############################################################
FILES=$(FILES)\
  $(OUT)\mach.obj


{target\c}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -Itarget\h -Itarget\$(TGT)\mach $<


##############################################################
#
# target\$(TGT)\mach
#
##############################################################
FILES=$(FILES)\
  $(OUT)\machconf.obj

{target\$(TGT)\mach}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -Itarget\c -Itarget\h  $<


##############################################################
#
# target\$(TGT)\cfe
#
##############################################################
FILES=$(FILES)\
  $(OUT)\cfeconf.obj

{target\$(TGT)\cfe}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -Icfe\c -Icfe\h -Itarget\$(TGT)\mach $<


##############################################################
#
# cfe
#
##############################################################
FILES=$(FILES)\
  $(OUT)\c-types.obj      \
  $(OUT)\c-ast.obj        \
  $(OUT)\idtab.obj        \
  $(OUT)\c-expr.obj       \
  $(OUT)\c-stmt.obj       \
  $(OUT)\c-decl.obj       \
  $(OUT)\c-dspecs.obj     \
  $(OUT)\c-symtab.obj     \
  $(OUT)\pp_parse.tab.obj \
  $(OUT)\pp_scan.obj      \
  $(OUT)\scan.obj         \
  $(OUT)\c.tab.obj        \
  $(OUT)\cfe_err.obj      \
  $(OUT)\cfe_opts.obj     \
  $(OUT)\hexdbl.obj       \
  $(OUT)\cfe.obj          \
  $(OUT)\c-dump.obj


{cfe\c}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -Icfe\h -Itarget\$(TGT)\cfe -Itarget\$(TGT)\mach $<

#
#
cfe\c\pp_parse.tab.c cfe\c\pp_parse.tab.h: cfe\c\pp_parse.y
  $(BISON) $(BFLAGS) -l -d -o cfe/c/pp_parse.tab.c cfe/c/pp_parse.y

cfe\c\pp_scan.c: cfe\c\pp_parse.tab.h cfe\c\pp_scan.l
  $(FLEX) $(FFLAGS) -ocfe/c/pp_scan.c cfe/c/pp_scan.l

#
#
cfe\c\c.tab.inc: cfe\c\c.yp
  $(PREBIS) -t $** > cfe/c/c1.y
  $(BISON) --token-table --no-parser -o cfe/c/c1.tab.c cfe/c/c1.y
  cl -DYYACT=\"c1.act\" -I$(PREBIS_LIB) -Fecfe/c/c1.tab.exe cfe/c/c1.tab.c
  cfe\c\c1.tab.exe > $@

cfe\c\c.y: cfe\c\c.yp
  $(PREBIS) cfe/c/c.yp > cfe/c/c.y

cfe\c\c.tab.h: cfe\c\c.y

cfe\c\c.tab.c: cfe\c\c.y cfe\c\c.tab.inc
  $(BISON) $(BFLAGS) --skeleton=cfe/c/bison.simple.c -d -o cfe/c/c.tab.c cfe/c/c.y
  $(REPL) $@ c.y c.yp

cfe\c\scan.c: cfe\c\c.tab.h cfe\c\srtuni\uni.inc cfe\c\scan.l
  $(FLEX) $(FFLAGS) -ocfe/c/scan.c cfe/c/scan.l

#
#
cfe\c\srtuni\uni.inc: cfe\c\srtuni\srtuni.exe cfe\c\srtuni\uni.txt
  cd cfe\c\srtuni
  srtuni < uni.txt | sort > uni.inc
  cd ..\..\..

cfe\c\srtuni\srtuni.exe: cfe\c\srtuni\srtuni.c
  cd cfe\c\srtuni
  $(CC) srtuni.c
  cd ..\..\..

cfe\c\srtuni\srtuni.c: cfe\c\srtuni\srtuni.l
  cd cfe\c\srtuni
  $(FLEX) srtuni.l
  cd ..\..\..
  
  



##############################################################
#
# exe
#  
FILES=$(FILES)\
  $(OUT)\main.obj

.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c -Itarget\$(TGT)\cfe -Itarget\$(TGT)\mach $<

$(OUT)\c99c.exe: $(FILES)
  cl $(CFLAGS) -Fe$@ $** 

$(OUT)\c99c.bsc: $(FILES)
  $(BSC) -o $@ $(OUT)\*.sbr
