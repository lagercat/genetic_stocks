DEST 	= ../..
CC      = gcc
INCDIR	= $(DEST)/include
LIBDIR 	= $(DEST)/lib
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I$(INCDIR)
LDLIBS	= -L$(LIBDIR) -lneuralnetwork -lm

.SUFFIXES: .c .o

.PHONY: all clean

all: xor_test

clean: 
	rm -f $(BUILD) *.o core
	rm xor_test
