DEST 	= ..
CC	    = gcc
LIBDIR 	= $(DEST)/lib
INCDIR	= $(DEST)/include
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I$(INCDIR)
LIB	    = libtestutils.a
LIBOBJS = testutils.o

.SUFFIXES: .c .o

.PHONY: all clean aggregate

all: $(LIB) aggregate

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

aggregate: $(LIB)
	install -m 644 $(LIB) $(LIBDIR)
	install -m 644 testutils.h $(INCDIR)


clean: 	
	rm -f $(LIB) *.o core
	rm $(LIBDIR)/$(LIB)
	rm $(INCDIR)/testutils.h
