/*
 *  kernel/include/sysclass_nr.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero de cabecera que contiene el numero asociado a cada llamada:
 * lo usa el kernel y la biblioteca de usuario */

#ifndef _SYSCALL_NR_H
#define _SYSCALL_NR_H

// Número de llamadas disponibles
#define NR_SYSCALLS	8

// Llamadas existentes
#define CREATE_PROCESS	0
#define EXIT_PROCESS	1
#define PRINT		2
#define GET_PID		3
#define GET_PRIORITY	4
#define PROC_SLEEP	5
#define MUTEX_OPEN	6
#define MUTEX_CLOSE	7

#endif /* _SYSCALL_NR_H */

