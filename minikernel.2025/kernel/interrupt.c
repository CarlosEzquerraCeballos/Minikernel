/*
 *  kernel/interrupt.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la gestión inicial de las interrupciones */

#include "HAL.h"
#include "interrupt.h"
#include "common.h"

static void dummy_exc_handler(void) {
    panic("unhandled exception");
}
static void dummy_trap_handler(void) {
    panic("unhandled system call trap");
}
static void dummy_int_handler(void) {
    printk("unhandled interrupt\n"); // pero seguimos
}
void init_interrupt_module(void) {
    // se instalan manejadores "provisionales";
    // más adelante, cada módulo instalará su manejador
    register_irq_handler(ARITM_EXC, dummy_exc_handler);
    register_irq_handler(MEM_EXC, dummy_exc_handler);
    register_irq_handler(SYS_CALL, dummy_trap_handler);
    register_irq_handler(SW_INT, dummy_int_handler);
    register_irq_handler(CLOCK_INT, dummy_int_handler);
    register_irq_handler(KEYBOARD_INT, dummy_int_handler);
    init_interrupt_controller();
}
