/*
 *  kernel/syscall.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene el tratamiento inicial de las llamadas */

#include "HAL.h"    
#include "interrupt.h"
#include "syscall.h"
#include "syscall_nr.h"
#include "process.h"
#include "terminal.h"
#include "clock.h"
#include "mutex.h"
#include "common.h"

// Prototipos de las rutinas que realizan cada llamada al sistema
static int sys_create_process(void);
static int sys_exit_process(void);
static int sys_print(void);
static int sys_get_pid(void);
static int sys_get_priority(void);

// tabla de llamadas al sistema
int (*syscalls_table[NR_SYSCALLS])() = {sys_create_process,
                                        sys_exit_process,
                                        sys_print,
                                        sys_get_pid,
                                        sys_get_priority,
                                       };

// Manejador de llamadas al sistema: descoméntelo en cuanto esté registrado
static void syscall_handler(void){
    int nr_syscall, res;

    nr_syscall = read_register(0);
    if (nr_syscall < NR_SYSCALLS)
        res = syscalls_table[nr_syscall]();
    else
        res = -1;        // servicio no existente
    write_register(0,res);
    return;
}

void init_syscall_module(void) {
    register_irq_handler(SYS_CALL, syscall_handler);
}

// comprueba los argumentos de tipo puntero de las llamadas

int check_arg = 0;

// verifica que es un string correcto accesible para lectura
int check_syscall_arg_string_read(char *str, int max_length) {
    int i, x=0;
    if (!str) return -1; // puntero nulo
    check_arg = 1;
    // comprueba accesibilidad de lectura
    for (i=0; str[i] && i<max_length; i++) x+= str[i];
    check_arg = 0;
    if (i == max_length) return -1; // string demasiado largo
    return 0;
}
// verifica que es un puntero que referencia una zona accesible para lectura
int check_syscall_arg_pointer_read(char *addr, int length) {
    int x=0;
    if (!addr) return -1; // puntero nulo
    check_arg = 1;
    // comprueba accesibilidad de lectura
    for (int i=0; i<length; i++) x+= addr[i];
    check_arg = 0;
    return 0;
}
// verifica que es un puntero que referencia una zona accesible para escritura
int check_syscall_arg_pointer_write(char *addr, int length) {
    if (!addr) return -1; // puntero nulo
    check_arg = 1;
    // comprueba accesibilidad de escritura
    for (int i=0; i<length; i++) addr[i]+=0;
    check_arg = 0;
    return 0;
}

// Llamadas al sistema

static int sys_create_process(void){
    char *prog;
    unsigned int prio;
    prog=(char *)read_register(1);
    prio=(unsigned int)read_register(2);
    return do_create_process(prog, prio);
}
static int sys_print(void) {
    char *texto;
    unsigned int longi;
    texto=(char *)read_register(1);
    longi=(unsigned int)read_register(2);
    return do_print(texto, longi);
}
static int sys_exit_process(void) {
    return do_exit_process();
}

static int sys_get_pid(void) {
    return do_get_pid();
}
static int sys_get_priority(void) {
    return do_get_priority();
}
