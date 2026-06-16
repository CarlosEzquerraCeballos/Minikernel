/*
 *  kernel/include/clock.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _CLOCK_H
#define _CLOCK_H

/* frecuencia de reloj usada (tics/segundo) */
#define TICK 100 // 100 tics por segundo -> 1 tic cada 10 ms.

/* rodaja usada en implementacion de round robin: número de tics */
#ifndef SLICE
#define SLICE 10  // 100 ms.
#endif

void init_clock_module(void); // función de iniciación del módulo

#endif /* _CLOCK_H */

