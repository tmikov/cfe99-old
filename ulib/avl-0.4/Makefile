V_MAJOR = 0
V_MINOR = 4
VERSION = $(V_MAJOR).$(V_MINOR)
CFLAGS = -O2
CC = gcc $(CFLAGS)
CPP = g++ $(CFLAGS)
LN = ln -sf
PREFIX = /usr/local
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
INSTALL = install -m 644
srcdir = .

default: libavl.so

test: avl_test
	./avl_test

testpp: avl_testpp
	./avl_testpp

libavl.so: avl.c avl.h
	$(CC) -c -shared $(srcdir)/avl.c -o libavl.so

progs: libavl.a

libavl.a: avl.c avl.h
	$(CC) -c $(srcdir)/avl.c -o libavl.a

avl_test: avl_test.c libavl.a
	$(CC) $(srcdir)/avl_test.c libavl.a -o avl_test

avl_testpp: avl_test.c avl.c avl.h
	$(CPP) -c $(srcdir)/avl.c -o libavl.a
	$(CPP) $(srcdir)/avl_test.cpp libavl.a -o avl_testpp

install: libavl.so libavl.a avl.h
	$(INSTALL) libavl.so $(LIBDIR)/libavl.so.$(VERSION)
	$(INSTALL) libavl.a $(LIBDIR)/libavl.a.$(VERSION)
	$(INSTALL) avl.h $(INCDIR)/avl.h
	$(LN) libavl.so.$(VERSION) $(LIBDIR)/libavl.so.$(MAJOR)
	$(LN) libavl.so.$(MAJOR) $(LIBDIR)/libavl.so
	$(LN) libavl.a.$(VERSION) $(LIBDIR)/libavl.a.$(MAJOR)
	$(LN) libavl.a.$(MAJOR) $(LIBDIR)/libavl.a

uninstall: 
	rm $(LIBDIR)/libavl.so*
	rm $(LIBDIR)/libavl.a*
	rm $(INCDIR)/avl.h

clean:
	rm -rfv *.o *.a *.so avl_test
