/*
 *  kernel/include/fifo.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _FIFO_H
#define _FIFO_H

/* Definición del FIFO */
typedef struct fifo {
    int size_elem;
    int max_nr_elems;
    int nr_elems;
    int in;
    int out;
    char *data;
} fifo;

// inicia un FIFO
int fifo_init(fifo *f, int size_elem, int max_nr_elems, void *data);

// comprueba si está vacío
int fifo_is_empty(const fifo *f);

// comprueba si está lleno
int fifo_is_full(const fifo *f);

// extrae el siguiente elemento del FIFO devolviendo -1 si error
int fifo_out(fifo *f, void *elem);

// inserta un elemento en el FIFO devolviendo -1 si error
int fifo_in(fifo *f, void *elem);

#endif /* _FIFO_H */

