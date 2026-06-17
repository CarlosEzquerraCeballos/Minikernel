/*
 *  kernel/include/process.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include "HAL.h"
#include "list.h"

#define MAX_NR_PROC	32

#define INIT_PROCESS "init" // nombre del proceso inicial

#define MAX_EXEC_NAMELENGTH	128 // incluido el carácter nulo

// Posibles estados del proceso
#define FINISHED	0
#define READY		1
#define RUNNING		2
#define BLOCKED		3

#define STACK_SIZE	32768

/* Definicion del tipo que corresponde con el PCB.  */
typedef struct PCB {
    struct PCB * next;	// puntero a PCB siguiente; DEBE SER EL PRIMER CAMPO
    struct PCB * prev;	// puntero a PCB previo; DEBE SER EL SEGUNDO CAMPO
    int pid;		// ident. del proceso
    int state;		// FINISHED|READY|RUNNING|BLOCKED
    int priority;	// prioridad del proceso
    int ticks_to_sleep;	// tics de reloj restantes de bloqueo (proc_sleep)
    context context;	// copia de regs. de UCP
    void *stack;	// dir. inicial de la pila
    void *mem;		// descriptor del mapa de memoria
} PCB;

// Variable global que identifica el proceso actual
extern PCB * current;

void init_process_module(void);

// el proceso actual no puede seguir ejecutando;
// hay que salvar su contexto y activar el proceso elegido por planificador;
// si parámetro es 0, no salva el contexto.
void pick_and_activate_next_task(int save_ctx);

// Implementación de las llamadas al sistema de este módulo

int do_create_process(char *prog, int prio);

int do_exit_process(void);

int do_get_pid(void);

int do_get_priority(void);

int do_proc_sleep(unsigned int secs);

// actualiza los procesos dormidos en cada tic de reloj
void update_sleeping_processes(void);

#endif /* _PROCESS_H */

