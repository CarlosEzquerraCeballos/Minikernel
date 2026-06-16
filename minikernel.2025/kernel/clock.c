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

/* Manejador de la interrupción de reloj (vector CLOCK_INT).
   En esta fase inicial solo notifica la llegada del tic. */
static void clock_handler(void) {
    printk("-> INT. RELOJ\n");
}

void init_clock_module(void) {
    register_irq_handler(CLOCK_INT, clock_handler);
    init_clock_controller(TICK); // TICK interrupciones de reloj por segundo
}
