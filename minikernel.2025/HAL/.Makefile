#
# boot/Makefile
#       Makefile para el "hardware virtual"
#

CC=gcc
CFLAGS= -g -Wall -fPIC

all: .HAL.o bib

.HAL.o: HAL.h

bib: .HAL.c HAL.h
	@ar r HAL .HAL.o

clean:
	@rm -f .HAL.o HAL

