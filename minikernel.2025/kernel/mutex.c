/*
 *  kernel/mutex.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */


/* Funciones relacionadas con la gestión de los mutex */

#include "common.h"
#include "mutex.h"
#include "process.h"
#include "syscall.h"
#include "list.h"
#include "sched.h"
#include "HAL.h"

/* Estados de una entrada de la tabla global de mutex */
#define MUTEX_ENTRY_FREE	0  // entrada de la tabla sin usar
#define MUTEX_ENTRY_USED	1  // entrada en uso (mutex existente)

/* Estado del cerrojo (se usará a partir de la Fase 6) */
#define MUTEX_UNLOCKED	0
#define MUTEX_LOCKED	1

/* Descriptor de un mutex en la tabla global del sistema */
typedef struct {
    int in_use;				// MUTEX_ENTRY_FREE | MUTEX_ENTRY_USED
    char name[MAX_MUTEX_NAMELENGTH];	// nombre del mutex
    int ref_count;			// nº de aperturas vivas (apertura/cierre)
    int locked;				// MUTEX_UNLOCKED | MUTEX_LOCKED (Fase 6)
    PCB *owner;				// proceso poseedor del cerrojo (Fase 6)
    list blocked_list;			// procesos bloqueados en el mutex (Fase 6)
} mutex_t;

/* Tabla global de mutex del sistema */
static mutex_t mutex_table[MAX_NR_MUTEX];

/* --- Funciones auxiliares de cadena (no se puede usar <string.h>) --- */

/* Compara dos cadenas terminadas en '\0'. Devuelve 1 si son iguales. */
static int str_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i]; // ambos deben terminar a la vez
}

/* Copia una cadena (con su '\0') en destino, sin exceder el tamaño máximo. */
static void str_copy(char *dst, const char *src, int max) {
    int i = 0;
    while (src[i] != '\0' && i < max - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* --- Búsquedas auxiliares --- */

/* Devuelve el índice global del mutex con ese nombre, o -1 si no existe. */
static int find_mutex_by_name(char *name) {
    for (int i = 0; i < MAX_NR_MUTEX; i++)
        if (mutex_table[i].in_use == MUTEX_ENTRY_USED &&
            str_equal(mutex_table[i].name, name))
            return i;
    return -1;
}

/* Devuelve el índice de una entrada global libre, o -1 si la tabla está llena. */
static int find_free_global_slot(void) {
    for (int i = 0; i < MAX_NR_MUTEX; i++)
        if (mutex_table[i].in_use == MUTEX_ENTRY_FREE)
            return i;
    return -1;
}

/* Devuelve el primer descriptor local libre del proceso actual, o -1. */
static int find_free_local_slot(void) {
    for (int i = 0; i < MAX_NR_MUTEX_PER_PROC; i++)
        if (current->open_mutexes[i] == -1)
            return i;
    return -1;
}

/* --- Llamadas al sistema --- */

/* Abre (o crea, si no existe) un mutex con el nombre dado. Devuelve el
   descriptor local (índice en open_mutexes del proceso) o -1 en caso de error. */
int do_mutex_open(char *name) {
    int local, global;

    /* Valida el puntero/cadena de usuario. */
    if (check_syscall_arg_string_read(name, MAX_MUTEX_NAMELENGTH) == -1)
        return -1;

    /* Necesita un descriptor local libre en el proceso. */
    local = find_free_local_slot();
    if (local == -1) return -1; // se superó MAX_NR_MUTEX_PER_PROC

    /* ¿Ya existe un mutex con ese nombre? */
    global = find_mutex_by_name(name);
    if (global != -1) {
        mutex_table[global].ref_count++; // otra apertura del mismo mutex
    } else {
        /* No existe: hay que crearlo. Necesita una entrada global libre. */
        global = find_free_global_slot();
        if (global == -1) return -1; // tabla global llena

        mutex_table[global].in_use = MUTEX_ENTRY_USED;
        str_copy(mutex_table[global].name, name, MAX_MUTEX_NAMELENGTH);
        mutex_table[global].ref_count = 1;
        mutex_table[global].locked = MUTEX_UNLOCKED;
        mutex_table[global].owner = NULL;
        list_init(&mutex_table[global].blocked_list);
    }

    /* Asocia el descriptor local con el mutex global y lo devuelve. */
    current->open_mutexes[local] = global;
    return local;
}

/* Extrae de una lista de bloqueados el proceso de MAYOR prioridad y lo pasa a
   la cola de listos. Devuelve dicho proceso, o NULL si la lista está vacía.
   Es O(n) en el tamaño de la lista de bloqueo (que es pequeña: una por evento),
   como indica el enunciado. Se encapsula porque la lectura de terminal (Fase 8)
   necesita el mismo comportamiento. Debe invocarse con el reloj inhibido. */
static PCB *unblock_most_priority(list *blocked) {
    iterator it;
    PCB *p, *best = NULL;

    if (list_is_empty(blocked)) return NULL;

    for (iterator_init(blocked, &it); iterator_has_next(&it); ) {
        p = iterator_next(&it);
        if (best == NULL || p->priority > best->priority)
            best = p;
    }
    remove_elem(blocked, best);
    add_ready_queue(best); // lo despierta (puede disparar expulsión si procede)
    return best;
}

static void mutex_release(int global) {
    /* Esta función manipula la lista de bloqueados del mutex y la cola de
       listos, así que debe ejecutarse con el reloj y el terminal inhibidos
       (nivel 3). Se eleva aquí, dentro de la propia función, para cubrir a
       TODOS sus invocadores (unlock explícito, y unlock implícito al cerrar o
       al terminar un proceso) en un único punto. La elevación es idempotente:
       si el llamador ya estaba en nivel 3, subir y restaurar a 3 no tiene
       efecto. */
    int prev_level = set_int_priority_level(LEVEL_3);

    mutex_table[global].locked = MUTEX_UNLOCKED;
    mutex_table[global].owner = NULL;
    unblock_most_priority(&mutex_table[global].blocked_list);

    set_int_priority_level(prev_level);
}

/* Cierra el descriptor local mutid del proceso actual. Si era la última
   apertura del mutex, lo libera de la tabla global. Devuelve 0 o -1. */
int do_mutex_close(int mutid) {
    int global;

    /* Valida que el descriptor local esté en rango y abierto. */
    if (mutid < 0 || mutid >= MAX_NR_MUTEX_PER_PROC) return -1;
    global = current->open_mutexes[mutid];
    if (global == -1) return -1; // no estaba abierto

    /* Si el proceso que cierra es el poseedor del cerrojo, hay que liberar la
       posesión (unlock implícito): despierta al siguiente en espera o lo deja
       libre. Cubre el cierre explícito y, vía do_exit_process, el implícito. */
    if (mutex_table[global].locked == MUTEX_LOCKED &&
        mutex_table[global].owner == current)
        mutex_release(global);

    /* Una apertura menos; si era la última, se libera la entrada global. */
    mutex_table[global].ref_count--;
    if (mutex_table[global].ref_count == 0)
        mutex_table[global].in_use = MUTEX_ENTRY_FREE;

    /* Libera el descriptor local. */
    current->open_mutexes[mutid] = -1;
    return 0;
}

int do_mutex_lock(int mutid) {
    int global, prev_level;

    if (mutid < 0 || mutid >= MAX_NR_MUTEX_PER_PROC) return -1;
    global = current->open_mutexes[mutid];
    if (global == -1) return -1;

    /* Prevención de auto-interbloqueo: si el proceso ya posee el cerrojo, un
       segundo lock lo bloquearía para siempre. Se detecta y se devuelve error. */
    if (mutex_table[global].locked == MUTEX_LOCKED &&
        mutex_table[global].owner == current)
        return -1;

    /* Toda la operación se realiza con el reloj inhibido (nivel 3) para que las
       comprobaciones del estado del cerrojo y las manipulaciones de listas sean
       atómicas. La cesión de CPU se hace con el nivel ya elevado (context_switch
       preserva el nivel de cada proceso) y se restaura al final. */
    prev_level = set_int_priority_level(LEVEL_3);

    /* Re-validación con while (no if): al desbloquearse, el cerrojo pudo haber
       sido tomado por otro proceso entre medias, así que hay que volver a
       comprobar la condición y, si sigue ocupado, bloquearse de nuevo. */
    while (mutex_table[global].locked == MUTEX_LOCKED) {
        /* Detección de interbloqueo (grafo de espera). Como cada proceso solo
           puede esperar por un mutex a la vez, el grafo es una simple cadena:
           se sigue "dueño del mutex que espero -> mutex que ese dueño espera ->
           su dueño -> ..." Si la cadena vuelve a current, bloquearse cerraría
           un ciclo: hay interbloqueo y se devuelve error.
           El chivato pending_mutex de current se marca ANTES de recorrer la
           cadena: así, si el ciclo se cierra sobre el propio current, el
           recorrido llega hasta él (su pending_mutex ya no es -1) y se detecta. */
        current->pending_mutex = global;

        PCB *aux = mutex_table[global].owner;
        while (aux != NULL) {
            if (aux == current) {       // la cadena vuelve a mí: ciclo
                current->pending_mutex = -1;
                set_int_priority_level(prev_level);
                return -1;
            }
            if (aux->pending_mutex == -1) break; // aux no espera nada: sin ciclo
            aux = mutex_table[aux->pending_mutex].owner;
        }

        current->state = BLOCKED;
        insert_last(&mutex_table[global].blocked_list, current);
        remove_ready_queue();
        pick_and_activate_next_task(1); // cede la CPU; al despertar reevalúa
        current->pending_mutex = -1;
    }

    /* Cerrojo libre: lo adquiere. */
    mutex_table[global].locked = MUTEX_LOCKED;
    mutex_table[global].owner = current;

    set_int_priority_level(prev_level);
    return 0;
}

/* Libera el cerrojo del mutex mutid. Solo puede hacerlo su poseedor. */
int do_mutex_unlock(int mutid) {
    int global, prev_level;

    if (mutid < 0 || mutid >= MAX_NR_MUTEX_PER_PROC) return -1;
    global = current->open_mutexes[mutid];
    if (global == -1) return -1;

    if (mutex_table[global].locked != MUTEX_LOCKED ||
        mutex_table[global].owner != current)
        return -1;

    /* Sección crítica: mutex_release manipula la lista de bloqueados y la cola
       de listos, que también toca el reloj. */
    prev_level = set_int_priority_level(LEVEL_3);
    mutex_release(global);
    set_int_priority_level(prev_level);

    return 0;
}

void init_mutex_module(void) {
    for (int i = 0; i < MAX_NR_MUTEX; i++)
        mutex_table[i].in_use = MUTEX_ENTRY_FREE;
}
