/*
 *  kernel/include/sched.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _SCHED_H
#define _SCHED_H

#include "process.h"

#define NR_PRIO		sizeof(int)*8		// cuántos bits ocupa un entero
#define MIN_PRIO	1			// a discreción
#define MAX_PRIO	MIN_PRIO + NR_PRIO -1
#define DEF_PRIO	MIN_PRIO + (NR_PRIO/2)	// en la mitad

void init_sched_module(void);

PCB * scheduler(void); // planificador

// añade un proceso a la cola de listos
void add_ready_queue(PCB *p);

// elimina el proceso actual de la cola de listos
void remove_ready_queue(void);


#endif /* _SCHED_H */

