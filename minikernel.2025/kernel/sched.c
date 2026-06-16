/*
 *  kernel/sched.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la funcionalidad de la planificación */

#include "HAL.h"
#include "sched.h"
#include "common.h"

/* Variable que representa la cola de procesos listos */
list ready_list;

/* Espera a que se produzca una interrupcion */
static void wait_for_int(void){
    int level;

    printk("-> NO HAY LISTOS. ESPERA INTERRUPCIÓN\n");

    /* Baja al mínimo el nivel de interrupción mientras espera */
    level=set_int_priority_level(LEVEL_1);
    halt();
    set_int_priority_level(level);
}

// añade un proceso a la cola de listos
void add_ready_queue(PCB *p) {
    p->state=READY;
    insert_last(&ready_list, p);
}
// elimina el proceso actual de la cola de listos
void remove_ready_queue(void) {
    remove_elem(&ready_list, current);
}

/* Función de planificacion  */
/* complejidad O(n) */
PCB * scheduler(void) {
    while (list_is_empty(&ready_list)) wait_for_int(); // nada que hacer

    iterator it;
    PCB *p, *p_sel = NULL;
    int prio = 1<<31; // mínimo valor negativo
    for (iterator_init(&ready_list, &it); iterator_has_next(&it); ) {
        p = iterator_next(&it);
        if (p->priority > prio) {
            p_sel = p; prio = p->priority;
        }
    }
    return p_sel;
}
        
void init_sched_module(void) {
    list_init(&ready_list);
}
