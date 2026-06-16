/*
 * user/yosoy.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que imprime su identificador y prioridad
 */

#include "services.h"

#define TOT_ITER 600000

int main(){
    int pid=get_pid();
    int prio = 0;
    // comente la siguiente línea si no tiene implementada get_priority
    prio = get_priority();

    printf("yosoy (PID %d: PRIO %d): comienza\n", pid, prio);

    for (long i=0; i<TOT_ITER; i++)
       printf("yosoy (PID %d: PRIO %d): i %d\n", pid, prio, i);
    printf("yosoy (PID %d: PRIO %d): termina\n", pid, prio);
    return 0;
}
