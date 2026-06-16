/*
 *  user/lib/misc.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "services.h"

/* Da acceso a los registros generales del "procesador" */
long * reglib = 0;

extern int vsprintf(char *s, const char *format, va_list ap);

int print_f(const char *formato, ...){
    char buffer[1024];
    va_list args;
    int res;

    va_start(args, formato);
    res=vsprintf(buffer, formato, args);
    if (res>0) 
        print(buffer, strlen(buffer));    
    return res;
}

static long read_register(int nr_reg) {
    return reglib[nr_reg];
}

static void write_register(int nr_reg, long value) {
    reglib[nr_reg]=value;
}

static void trap(void) {
    kill(getpid(), SIGUSR1);
}

/*
 * Función que prepara los parámetros, realiza la
 * instruccion de llamada al sistema  y devuelve el resultado
*/
int sys_call(int llamada, int nargs, ... /* args */) {
    va_list args;
    int reg;

    write_register(0, llamada);

    va_start(args, nargs);
    for ( reg=1; nargs; nargs--, reg++) 
        write_register(reg, va_arg(args, long));
    va_end(args);
    
    trap();
    return read_register(0);
}

/* Funcion inicial del proceso */ 
void start(void (*fini) (void)) {
    fini();
    exit_process();
}

