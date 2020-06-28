DEST	= ..
CC	= gcc
INCDIR	= $(DEST)/include
LIBDIR 	= $(DEST)/lib
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I.
LDLIBS  = -lm
LIBOBJS	= mlp.o
LIB	= libneuralnetwork.a

.SUFFIXES: .c .o

.PHONY: all builtests test clean

all: $(LIB) aggregate buildtests

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

buildtests: aggregate
	cd tests/ && make

test: builtests
	cd tests/ && ./xor_test

aggregate: $(LIB)
	install -m 644 $(LIB) $(LIBDIR)
	install -m 644 mlp.h $(INCDIR)

clean:
	rm -f $(wildcard *.0)
	rm -f $(LIB)
	rm $(LIBDIR)/$(LIB)
	rm $(INCDIR)/mlp.h
	cd tests/ && make clean
