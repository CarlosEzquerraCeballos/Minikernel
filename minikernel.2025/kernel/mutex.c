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

/* Cierra el descriptor local mutid del proceso actual. Si era la última
   apertura del mutex, lo libera de la tabla global. Devuelve 0 o -1. */
int do_mutex_close(int mutid) {
    int global;

    /* Valida que el descriptor local esté en rango y abierto. */
    if (mutid < 0 || mutid >= MAX_NR_MUTEX_PER_PROC) return -1;
    global = current->open_mutexes[mutid];
    if (global == -1) return -1; // no estaba abierto

    /* Una apertura menos; si era la última, se libera la entrada global. */
    mutex_table[global].ref_count--;
    if (mutex_table[global].ref_count == 0)
        mutex_table[global].in_use = MUTEX_ENTRY_FREE;

    /* Libera el descriptor local. */
    current->open_mutexes[mutid] = -1;
    return 0;
}

void init_mutex_module(void) {
    for (int i = 0; i < MAX_NR_MUTEX; i++)
        mutex_table[i].in_use = MUTEX_ENTRY_FREE;
}
