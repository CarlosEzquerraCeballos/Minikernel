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

/* Libera la posesión de un mutex (ya identificado por su índice global). Si
   hay procesos esperando, transfiere el cerrojo directamente al primero de la
   lista de bloqueados y lo pasa a listos; si no hay nadie esperando, deja el
   mutex libre. Debe invocarse con las interrupciones de reloj ya inhibidas
   (nivel 3) por quien manipule listas. */
static void mutex_release(int global) {
    PCB *p_next;

    if (list_is_empty(&mutex_table[global].blocked_list)) {
        mutex_table[global].locked = MUTEX_UNLOCKED;
        mutex_table[global].owner = NULL;
    } else {
        /* Transferencia directa del cerrojo al siguiente en espera. */
        p_next = mutex_table[global].blocked_list.first;
        remove_first(&mutex_table[global].blocked_list);
        mutex_table[global].owner = p_next; // el cerrojo sigue locked
        add_ready_queue(p_next);            // lo despierta
    }
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

/* Adquiere el cerrojo del mutex mutid. Si está libre, lo toma. Si está
   ocupado por otro, el proceso se bloquea hasta que se le transfiera. Si el
   propio proceso ya lo posee, devuelve -1 (auto-interbloqueo). */
int do_mutex_lock(int mutid) {
    int global, prev_level;

    if (mutid < 0 || mutid >= MAX_NR_MUTEX_PER_PROC) return -1;
    global = current->open_mutexes[mutid];
    if (global == -1) return -1;

    if (mutex_table[global].locked == MUTEX_UNLOCKED) {
        mutex_table[global].locked = MUTEX_LOCKED;
        mutex_table[global].owner = current;
        return 0;
    }

    /* Ocupado. Si ya lo posee este mismo proceso, es auto-interbloqueo. */
    if (mutex_table[global].owner == current) return -1;

    /* Ocupado por otro: el proceso se bloquea en la lista del mutex. La
       manipulación de listas se hace con el reloj inhibido (nivel 3); la
       cesión de CPU se realiza con el nivel ya elevado (context_switch
       preserva el nivel de cada proceso) y se restaura al despertar. */
    prev_level = set_int_priority_level(LEVEL_3);

    current->state = BLOCKED;
    insert_last(&mutex_table[global].blocked_list, current);
    remove_ready_queue();

    pick_and_activate_next_task(1); // cede la CPU

    /* Al despertar, el cerrojo ya se le ha transferido (es el owner). */
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
