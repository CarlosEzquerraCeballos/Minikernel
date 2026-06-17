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
#include "bitmask.h"
#include "common.h"

/* Planificador O(1) con prioridades expulsivas.
 *
 * Hay una cola de listos por cada nivel de prioridad (ready_queue) y una
 * máscara de bits (ready_mask) que indica, en O(1), qué colas no están
 * vacías. Las prioridades válidas van de MIN_PRIO a MAX_PRIO; se proyectan
 * sobre los bits 0..NR_PRIO-1 de la máscara mediante prio_to_bit(). Como una
 * prioridad mayor se corresponde con un bit de mayor peso, el bit de mayor
 * peso a 1 identifica directamente la cola más prioritaria no vacía.
 *
 * Invariante (heredado del diseño original O(n) que usa process.c): el
 * proceso en ejecución PERMANECE en su cola de listos mientras corre; se
 * extrae de ella únicamente cuando deja de estar listo (remove_ready_queue).
 * Por tanto scheduler() SELECCIONA sin extraer.
 */
static list ready_queue[NR_PRIO];
static unsigned int ready_mask;

/* Proyecta una prioridad (MIN_PRIO..MAX_PRIO) sobre el bit de la máscara y,
   a la vez, sobre el índice del vector de colas (0..NR_PRIO-1). */
static inline int prio_to_bit(int prio) {
    return prio - MIN_PRIO;
}

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
    int bit = prio_to_bit(p->priority);

    p->state = READY;
    insert_last(&ready_queue[bit], p);
    set_bit(&ready_mask, bit);

    /* Disparo estratégico de la expulsión: solo si el proceso recién insertado
       es ESTRICTAMENTE más prioritario que el proceso en ejecución. No se
       dispara si no hay proceso actual (current==NULL, primer proceso del
       sistema) o si current no está realmente ejecutando (está terminado o
       bloqueado haciendo de "proceso nulo" a la espera de interrupciones en
       wait_for_int); en esos casos la replanificación ya está en marcha por
       otra vía y una interrupción software sería innecesaria. */
    if (current != NULL && current->state == RUNNING &&
        p->priority > current->priority)
        activate_soft_int();
}

// elimina el proceso actual de la cola de listos
void remove_ready_queue(void) {
    int bit = prio_to_bit(current->priority);

    remove_elem(&ready_queue[bit], current);
    if (list_is_empty(&ready_queue[bit]))
        clear_bit(&ready_mask, bit);
}

/* Función de planificacion */
/* complejidad O(1): localiza con la máscara la cola no vacía de mayor
   prioridad y devuelve su primer elemento (FIFO dentro del nivel) SIN
   extraerlo: el proceso en ejecución permanece en la cola. */
PCB * scheduler(void) {
    int bit;

    while (ready_mask == 0) wait_for_int(); // nada que hacer

    bit = find_last_bit_set(ready_mask); // cola no vacía más prioritaria
    return ready_queue[bit].first;       // primero de esa cola (FIFO)
}

/* Manejador de la interrupción software (vector SW_INT).
   Provoca la replanificación expulsiva salvando el contexto del proceso
   expulsado (que sigue listo en su cola y podrá reanudarse después). */
static void sw_int_handler(void) {
    pick_and_activate_next_task(1); // salva el contexto del proceso expulsado
}

/* Planificador original O(n). NO se utiliza en esta fase; se conserva intacto
   porque será necesario en la Fase 6. Operaba sobre una única cola de listos
   (ready_list) que ya no existe en esta versión, por lo que se deja inactivo
   entre #if 0 para que no se compile ni interfiera. */
#if 0
PCB * scheduler_O_n_original(void) {
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
#endif

void init_sched_module(void) {
    for (int i = 0; i < NR_PRIO; i++)
        list_init(&ready_queue[i]);
    ready_mask = 0;
    register_irq_handler(SW_INT, sw_int_handler);
}
