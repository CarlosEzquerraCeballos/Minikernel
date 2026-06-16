/*
 * user/locker2_int.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Programa inicial que arranca el S.O.
   Sólo se ejecutarán los programas que se incluyan aquí, que, evidentemente,
   pueden ejecutar otros programas... */

// Para probar el interbloqueo

#include "services.h"

int main(){
    int d1, d3;
    printf("locker2_int comienza\n");

    if ((d1=mutex_open("m1"))<0)
        printf("Error abriendo m1\n");

    if ((d3=mutex_open("m3"))<0)
        printf("Error abriendo m3\n");

    if (mutex_lock(d3)<0)
        printf("Error lock de m3\n");

    if (mutex_lock(d1)<0)
        printf("Error lock de m1\n");

    printf("locker2_int termina\n");

    return 0; 
}
