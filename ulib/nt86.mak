OUT=out

CC=cl
BSC=bscmake
CDEFINES=-D_DEBUG -DUSE_GC
CFLAGS=$(CDEFINES) -W3 -I.. -I. -Fr$(OUT)\\

!IFDEF debug 
CFLAGS=$(CFLAGS) -MDd -Zi 
!ELSE
CFLAGS=$(CFLAGS) -MD
!ENDIF

TGT=i386

LIB=lib
LIBFLAGS=

FILES=

##############################################################

all: $(OUT)\ulib.bsc $(OUT)\ulib.lib

clean:
  @del *.pdb;*.pch;*.ilk;*.obj;*.exe;*.sbr;*.bsc;*.lib /s

##############################################################
#
# c
#
##############################################################
FILES=$(FILES)\
  $(OUT)\debug.obj        \
  $(OUT)\cord.obj         \
  $(OUT)\clist.obj        \
  $(OUT)\simgc.obj        \
  $(OUT)\strings.obj

{c}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Ih -Fo$(OUT)\ -c $<

##############################################################
#
# avl-0.4
#
##############################################################
FILES=$(FILES)\
  $(OUT)\avl.obj

{avl-0.4}.c{$(OUT)}.obj::
  $(CC) $(CFLAGS) -Fo$(OUT)\ -c $<



##############################################################
#
# ulib.lib
#
##############################################################
$(OUT)\ulib.lib: $(FILES)
  $(LIB) /OUT:$@ $**

$(OUT)\ulib.bsc: $(FILES)
  $(BSC) -o $@ $(OUT)\*.sbr
