/*
 *  kernel/fifo.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la implementación de una lista genérica. */

#include "common.h"
#include "fifo.h"

// inicia un FIFO
int fifo_init(fifo *f, int size_elem, int max_nr_elems, void *data) {
    if (!f || (size_elem<=0) || (max_nr_elems<=0) || !data) return -1;
    f->size_elem = size_elem;
    f->max_nr_elems = max_nr_elems;
    f->nr_elems = 0;
    f->data = data;
    f->in = 0;
    f->out = 0;
    return 0;
}
// comprueba si está vacío
int fifo_is_empty(const fifo *f) {
    return f && (f->nr_elems == 0);
}
// comprueba si está lleno
int fifo_is_full(const fifo *f) {
    return f && (f->nr_elems == f->max_nr_elems);
}
// extrae el siguiente elemento del FIFO devolviendo -1 si error
int fifo_out(fifo *f, void *elem) {
    if (!elem || !f || fifo_is_empty(f)) return -1;
    for (int i = 0; i < f->size_elem; i++)
        ((char *)(elem))[i] = f->data[i + f->out * f->size_elem];
    f->out = (f->out + 1) % f->max_nr_elems;
    --f->nr_elems;
    return 0;
}
// inserta un elemento en el FIFO devolviendo -1 si error
int fifo_in(fifo *f, void *elem) {
    if (!elem || !f || fifo_is_full(f)) return -1;
    for (int i = 0; i < f->size_elem; i++)
        f->data[i + f->in * f->size_elem] = ((char *)(elem))[i];
    f->in = (f->in + 1) % f->max_nr_elems;
    ++f->nr_elems;
    return 0;
}

/* Ejemplo para un FIFO de chars

    // buf es una variable estática o dinámica (kmalloc) con sitio para  n chars
    fifo f;
    fifo_init(&f, sizeof(char), n, buf);
    if (fifo_is_empty(&f))	// está vacío
    if (fifo_is_full(&f))	// está lleno
    fifo_in(&f, &c); 		// inserta un carácter
    fifo_out(&f, &c); 		// extrae un carácter
*/

