/*
 *  kernel/exception.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#include "HAL.h"
#include "exception.h"
#include "process.h"
#include "syscall.h"
#include "common.h"

/* Funciones relacionadas con el tratamiento de excepciones */

/* Manejador de la excepción aritmética (vector ARITM_EXC).
   Si la excepción se produjo en modo usuario, se trata de un error del
   proceso: se aborta su ejecución de forma involuntaria. Si se produjo en
   modo sistema, es un fallo del propio núcleo y se detiene el SO. */
static void exc_aritm_handler(void) {
    if (comes_from_usermode())
        do_exit_process(); // muerte involuntaria del proceso actual
    else
        panic("excepcion aritmetica en modo sistema");
}

/* Manejador de la excepción de memoria (vector MEM_EXC).
   Si se produjo en modo usuario, se aborta el proceso. Si se produjo en modo
   sistema, hay que distinguir: si el núcleo estaba comprobando un puntero
   pasado por el usuario (check_arg activo), el culpable es el proceso y se le
   aborta; en otro caso es un bug real del kernel y se detiene el SO. */
static void exc_mem_handler(void) {
    if (comes_from_usermode())
        do_exit_process(); // muerte involuntaria del proceso actual
    else if (check_arg)
        do_exit_process(); // fallo validando puntero de usuario: culpa del proceso
    else
        panic("excepcion de memoria en modo sistema");
}

void init_exception_module(void) {
    register_irq_handler(ARITM_EXC, exc_aritm_handler);
    register_irq_handler(MEM_EXC, exc_mem_handler);
}
