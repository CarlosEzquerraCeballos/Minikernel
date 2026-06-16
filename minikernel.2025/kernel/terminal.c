/*
 *  kernel/terminal.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */


#include "HAL.h"
#include "terminal.h"
#include "syscall.h"
#include "common.h"

/* Funciones relacionadas con la gestión del terminal */

// implementación de la llamada al sistema
int do_print(char * buf, int size) {
    if (size <= 0) return -1;
    if (check_syscall_arg_pointer_read(buf, size) == -1)
        return -1;
    print_terminal(buf, size); // operación del HAL
    return 0;
}
void init_terminal_module(void) {
}
