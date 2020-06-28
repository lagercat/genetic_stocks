DEST     = .
CC       = gcc
INCDIR   = $(DEST)/include
LIBDIR   = $(DEST)/lib
CFLAGS   = -Wall -O3 -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic\
	   -I$(INCDIR) -I.
LDLIBS   = -L$(LIBDIR) -lneuralnetwork -ldata -lgenetic -lm
LIBS     = libtest libneuralnetwork libgenetic libdata
TESTLIBS = libneuralnetwork libdata
OBJS     = train.o predict.o

.SUFFIXES: .c .o

.PHONY: libs all clean cleanlibs

all: libs train predict

libs: 
	for lib in $(LIBS) ; do \
		cd $$lib && make; \
		cd ..; \
	done

test: libs
	for lib in $(TESTLIBS) ; do \
		cd $$lib && make test; \
		cd ..; \
	done

clean: cleanlibs
	rm -f $(wildcard *.o)
	rm -f train
	rm -f predict

cleanlibs:
	for lib in $(LIBS) ; do \
		cd $$lib && make clean; \
		cd ..; \
	done

