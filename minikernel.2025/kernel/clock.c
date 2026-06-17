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

/* Manejador de la interrupción de reloj (vector CLOCK_INT).
   En cada tic actualiza la cuenta de los procesos dormidos, despertando a los
   que hayan agotado su tiempo de bloqueo. Se ejecuta en nivel 3, por lo que
   update_sleeping_processes opera sobre sleep_list sin riesgo de carrera. */
static void clock_handler(void) {
    update_sleeping_processes();
}

void init_clock_module(void) {
    register_irq_handler(CLOCK_INT, clock_handler);
    init_clock_controller(TICK); // TICK interrupciones de reloj por segundo
}
