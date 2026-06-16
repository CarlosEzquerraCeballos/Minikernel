#
# kernel/Makefile
#	Makefile del sistema 
#

INCLUDEDIR=include
CC=gcc
CFLAGS=-g -Wall -fPIC -Werror -I$(INCLUDEDIR)

all: boot HAL kernel 

OBJS_KER=main.o exception.o interrupt.o syscall.o process.o sched.o clock.o mutex.o terminal.o list.o strings.o fifo.o bitmask.o
BIB_KER=-ldl

$(OBJS_KER): $(INCLUDEDIR)/clock.h $(INCLUDEDIR)/HAL.h $(INCLUDEDIR)/list.h $(INCLUDEDIR)/syscall.h $(INCLUDEDIR)/syscall_nr.h $(INCLUDEDIR)/process.h $(INCLUDEDIR)/terminal.h $(INCLUDEDIR)/exception.h $(INCLUDEDIR)/interrupt.h $(INCLUDEDIR)/sched.h $(INCLUDEDIR)/mutex.h $(INCLUDEDIR)/strings.h $(INCLUDEDIR)/fifo.h  $(INCLUDEDIR)/bitmask.h  $(INCLUDEDIR)/common.h

kernel: $(OBJS_KER) ../HAL/.HAL.o
	$(CC) -shared -o $@ $(OBJS_KER) ../HAL/HAL $(BIB_KER)

boot:
	@$(MAKE) -s -C ../boot -f .Makefile

HAL:
	@$(MAKE) -s -C ../HAL -f .Makefile

clean:
	rm -f  $(OBJS_KER) kernel
