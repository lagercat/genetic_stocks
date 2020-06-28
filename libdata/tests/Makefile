DEST 	= ../..
CC      = gcc
INCDIR	= $(DEST)/include
LIBDIR 	= $(DEST)/lib
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -I$(INCDIR)
LDLIBS	= -L$(LIBDIR) -ltestutils -ldata -lneuralnetwork -lgenetic -lm

.SUFFIXES: .c .o

.PHONY: all clean

all: testload testdataops

clean: 
	rm -f $(BUILD) *.o
	rm -f *.csv
	rm testload
	rm testdataops
