/*
 * user/init.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Programa inicial que arranca el S.O.
   Sólo se ejecutarán los programas que se incluyan aquí, que, evidentemente,
   pueden ejecutar otros programas... */

// solo hace cálculos; permite apreciar la llegada de interrupciones

#include "services.h"

#define TOT_ITER 1000000000
int main(){
    long x = 0;
    printf("init comienza\n");

    for (long i = 0; i < TOT_ITER; i++) x += i;

    printf("init termina\n");
    return 0; 
}
