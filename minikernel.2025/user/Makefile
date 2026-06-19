#
# usuario/Makefile
#	Makefile de programas de usuario
#

MAKEFLAGS=-k
INCLUDEDIR=include
LIBDIR=lib

CC=gcc
CFLAGS=-Wall -fPIC -Werror -g -I$(INCLUDEDIR)

PROGRAMAS=init lanza simplon yosoy dormilon yosoy_largo mudo creador_mut1 creador_mut2 creador_mut3 creador_mut4 creador_mut_error locker1 locker2 locker3 # locker1_int locker2_int lector lector2

all: biblioteca $(PROGRAMAS)

biblioteca:
	cd lib; make

init.o: $(INCLUDEDIR)/services.h
init: init.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

lanza.o: $(INCLUDEDIR)/services.h
lanza: lanza.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

simplon.o: $(INCLUDEDIR)/services.h
simplon: simplon.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

yosoy.o: $(INCLUDEDIR)/services.h
yosoy: yosoy.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

yosoy_largo.o: $(INCLUDEDIR)/services.h
yosoy_largo: yosoy_largo.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

mudo.o: $(INCLUDEDIR)/services.h
mudo: mudo.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

dormilon.o: $(INCLUDEDIR)/services.h
dormilon: dormilon.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

creador_mut1.o: $(INCLUDEDIR)/services.h
creador_mut1: creador_mut1.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

creador_mut2.o: $(INCLUDEDIR)/services.h
creador_mut2: creador_mut2.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

creador_mut3.o: $(INCLUDEDIR)/services.h
creador_mut3: creador_mut3.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

creador_mut4.o: $(INCLUDEDIR)/services.h
creador_mut4: creador_mut4.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

creador_mut_error.o: $(INCLUDEDIR)/services.h
creador_mut_error: creador_mut_error.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

locker1.o: $(INCLUDEDIR)/services.h
locker1: locker1.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

locker2.o: $(INCLUDEDIR)/services.h
locker2: locker2.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

locker3.o: $(INCLUDEDIR)/services.h
locker3: locker3.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

locker1_int.o: $(INCLUDEDIR)/services.h
locker1_int: locker1_int.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

locker2_int.o: $(INCLUDEDIR)/services.h
locker2_int: locker2_int.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

lector.o: $(INCLUDEDIR)/services.h
lector: lector.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

lector2.o: $(INCLUDEDIR)/services.h
lector2: lector2.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o
	$(CC) -shared -o $@ $@.o $(LIBDIR)/services.o $(LIBDIR)/.crt.o

clean:
	rm -f *.o $(PROGRAMAS)
	cd lib; make clean
