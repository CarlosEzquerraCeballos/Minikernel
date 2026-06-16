/*
 *  kernel/list.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la implementación de una lista genérica. */

#include "list.h"
#include "common.h"

/*
 * RESTRICCIÓN: los 2 primeros campos de los elementos que se inserten en la
 * lista debe de ser un puntero que se usará para almacenar una
 * referencia al siguiente elemento y otro al previo, respectivamente.
 */
typedef struct node {
    struct node *next;
    struct node *prev;
} node;

// Inicia una lista
int list_init(list * lst){
    if (!lst) return 0;
    lst->first = lst->last = NULL;
    return 0;
}
// Lista vacía?
int list_is_empty(list * lst) {
    return lst && !lst->first;
}
// Inserta un elemento al final de la lista.
int insert_last(list * lst, void * elem){
    if (!lst || !elem) return 0;
    if (lst->first == NULL)
        lst->first = elem;
    else
        ((node *)lst->last)->next = elem;
    ((node *)elem)->prev = lst->last;
    ((node *)elem)->next = NULL;
    lst->last = elem;
    return 1;
}
// Inserta un elemento al principio de la lista.
int insert_first(list * lst, void * elem){
    if (!lst || !elem) return 0;
    ((node *)elem)->next = lst->first;
    ((node *)elem)->prev = NULL;
    lst->first = elem;
    if (lst->last == NULL)
        lst->last = elem;
    return 1;
}
// Elimina el primer elemento de la lista.
int remove_first(list *lst){
    if (!lst || !lst->first) return 0;
    if (lst->last == lst->first)
        lst->last = NULL;
    lst->first = ((node *)lst->first)->next;
    if (lst->first)
        ((node *)lst->first)->prev = NULL;
    return 1;
}
// Elimina un determinado elemento de la lista.
int remove_elem(list *lst, void * elem){
    int res;
    if (!lst || !lst->first || !elem) return 0;
    if (lst->first == elem)
        res = remove_first(lst);
    else {
        node * prev = ((node *)elem)->prev;
        prev->next = ((node *)elem)->next;
        if (prev->next)
            prev->next->prev = prev;
        if (lst->last == elem)
            lst->last = prev;
    }
    return res;
}

// Mueve un elemento de una lista al principio de la misma.
int move_elem_first(list *lst, void *elem) {
    int res = 0;
    if (remove_elem(lst, elem) && insert_first(lst, elem)) res = 1;
    return res;
}
// Mueve un elemento de una lista al final de la misma.
int move_elem_last(list *lst, void *elem) {
    int res = 0;
    if (remove_elem(lst, elem) && insert_last(lst, elem)) res = 1;
    return res;
}

// Inicia un iterador
int iterator_init(list * lst, iterator *it){
    if (!lst || !it) return 0;
    it->lst = lst;
    it->next = lst->first;
    return 0;
}
int iterator_has_next(iterator *it){
    return it && it->next;
}
void * iterator_next(iterator *it){
    if (!it) return NULL;
    void *res = it->next;
    it->next = ((node *)it->next)->next;
    return res;
}
