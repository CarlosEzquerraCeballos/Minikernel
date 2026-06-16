#
# usuario/lib/Makefile
#	Makefile de la biblioteca
#

INCLUDEDIR=../include
INCLUDEDIR2=../../kernel/include

CC=gcc
CFLAGS=-Wall -g -fPIC -I$(INCLUDEDIR) -I$(INCLUDEDIR2)

all: services.o .crt.o

services.o: $(INCLUDEDIR)/services.h $(INCLUDEDIR2)/syscall_nr.h
.crt.o: $(INCLUDEDIR)/services.h

clean:
	rm -f services.o .crt.o
