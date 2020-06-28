DEST 	= ..
CC      = gcc
INCDIR	= $(DEST)/include
LIBDIR 	= $(DEST)/lib
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I. -I$(INCDIR)
LIBOBJS = createstructures.o crossover.o geneticutils.o selection.o
LIB     = libgenetic.a

.SUFFIXES: .c .o

.PHONY: all clean

all: $(LIB) aggregate

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

aggregate: $(LIB)
	install -m 644 $(LIB) $(LIBDIR)
	install -m 644 createstructures.h $(INCDIR)
	install -m 644 crossover.h $(INCDIR)
	install -m 644 geneticutils.h $(INCDIR)
	install -m 644 selection.h $(INCDIR)
	install -m 644 structures.h $(INCDIR)

clean:
	rm -f $(wildcard *.o)
	rm -f $(LIB)
	rm $(LIBDIR)/$(LIB)
	rm $(INCDIR)/createstructures.h
	rm $(INCDIR)/crossover.h
	rm $(INCDIR)/geneticutils.h
	rm $(INCDIR)/selection.h
	rm $(INCDIR)/structures.h
