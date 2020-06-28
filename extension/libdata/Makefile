DEST 	= ..
CC      = gcc
INCDIR	= $(DEST)/include
LIBDIR 	= $(DEST)/lib
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I. -I$(INCDIR)
LDLIBS  = -L$(LIBDIR) -lneuralnetwork -ldata -lgenetic -lm
LIBOBJS = dataops.o csv.o managenn.o
LIB     = libdata.a

.SUFFIXES: .c .o

.PHONY: all buildtests test clean

all: $(LIB) aggregate buildtests

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

buildtests: aggregate
	cd tests/ && make

test: buildtests
	cd tests/ && ./testload
	cd tests/ && ./testdataops
    
aggregate: $(LIB)
	install -m 644 $(LIB) $(LIBDIR)
	install -m 644 dataops.h $(INCDIR)
	install -m 644 csv.h $(INCDIR)
	install -m 644 managenn.h $(INCDIR)

clean:
	rm -f $(wildcard *.o)
	rm -f $(LIB)
	rm $(LIBDIR)/$(LIB)
	rm $(INCDIR)/dataops.h
	rm $(INCDIR)/csv.h
	rm $(INCDIR)/managenn.h
	cd tests && make clean
