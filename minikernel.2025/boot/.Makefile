#
# boot/Makefile
#       Makefile para el boot
#

CC=gcc
CFLAGS= -g -Wall

all: .boot

.boot: .boot.c
	@$(CC) -o $@ .boot.c -ldl
	@mv .boot boot

clean:
	@rm -f .boot.o .boot boot
