/*
 *  kernel/main.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la función main del sistema operativo */

#include "interrupt.h"
#include "exception.h"
#include "syscall.h"
#include "clock.h"
#include "process.h"
#include "sched.h"
#include "mutex.h"
#include "terminal.h"
#include "common.h"

/* Rutina de inicialización del SO invocada en arranque */

int main(){
    printk("-> ARRANQUE DEL KERNEL\n");

    // se inician todos los módulos
    init_interrupt_module();
    init_exception_module();
    init_syscall_module();
    init_process_module();
    init_sched_module();
    init_clock_module();
    init_mutex_module();
    init_terminal_module();
    
    printk("-> CREA Y ACTIVA EL PROCESO INICIAL\n");
    if (do_create_process(INIT_PROCESS, DEF_PRIO)<0)
        panic("error activando el proceso inicial");

    pick_and_activate_next_task(0); // activa el proceso inicial

    panic("unexpected execution of OS main function");
    return 0;
}
