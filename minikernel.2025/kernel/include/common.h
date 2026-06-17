/*
 *  kernel/include/common.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _COMMON_H
#define _COMMON_H

// para incluir definiciones comunes para todos los módulos
#define NULL ((void *)0)

// Quantum de CPU (en tics de reloj) para el turno rotatorio (round-robin)
#define TICKS_POR_RODAJA 10

#endif /* _COMMON_H */

