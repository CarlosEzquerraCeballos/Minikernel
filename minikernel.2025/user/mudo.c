/*
 * user/mudo.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

// realiza cálculos

#include "services.h"

#define TOT_ITER 100000000

int main(){
    long x = 0;

    printf("mudo (PID %d: PRIO %d): comienza\n", get_pid(), get_priority());

    for (long i = 0; i < TOT_ITER; i++) x += i;

    printf("mudo (PID %d: PRIO %d): termina\n", get_pid(), get_priority());
    return 0;  // final implícito
}
