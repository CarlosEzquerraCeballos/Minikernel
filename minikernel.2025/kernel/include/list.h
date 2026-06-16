/*
 *  kernel/include/list.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _LIST_H
#define _LIST_H

// definición de la lista
typedef struct list {
    void *first;
    void *last;
} list;

// definición de un iterador
typedef struct iterator {
    struct list *lst;
    void *next;
} iterator;

// Inicia una lista
int list_init(list * lst);

// Lista vacía?
int list_is_empty(list * lst);

// Inserta un elemento al final de la lista.
int insert_last(list * lst, void * elem);

// Inserta un elemento al principio de la lista.
int insert_first(list * lst, void * elem);

// Elimina el primer elemento de la lista.
int remove_first(list *lst);

// Elimina un determinado elemento de la lista.
int remove_elem(list *lst, void * elem);

// Mueve un elemento de una lista al principio de la misma.
int move_elem_first(list *lst, void *elem);

// Mueve un elemento de una lista al final de la misma.
int move_elem_last(list *lst, void *elem);

// Inicia un iterador
int iterator_init(list * lst, iterator *it);

int iterator_has_next(iterator *it);

void * iterator_next(iterator *it);

#endif /* _LIST_H */

