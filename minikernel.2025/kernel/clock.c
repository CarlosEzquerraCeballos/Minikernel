/*
 *  kernel/clock.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la funcionalidad relacionada con el tiempo */

#include "common.h"
#include "HAL.h"
#include "clock.h"
#include "process.h"
#include "sched.h"

/* Manejador de la interrupción de reloj (vector CLOCK_INT).
   En cada tic: despierta a los procesos dormidos cuyo plazo ha vencido y
   descuenta tiempo de la rodaja del proceso en ejecución (round-robin). Se
   ejecuta en nivel 3, por lo que ambas operaciones manipulan las listas sin
   riesgo de carrera. */
static void clock_handler(void) {
    update_sleeping_processes();
    round_robin_tick();
}

void init_clock_module(void) {
    register_irq_handler(CLOCK_INT, clock_handler);
    init_clock_controller(TICK); // TICK interrupciones de reloj por segundo
}
