/*
 * user/locker1_int.c
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
    int d2, d3;
    printf("locker1_int comienza\n");

    if ((d2=mutex_open("m2"))<0)
        printf("Error abriendo m2\n");

    if ((d3=mutex_open("m3"))<0)
        printf("Error abriendo m3\n");

    if (mutex_lock(d2)<0)
        printf("Error lock de m2\n");

    if (create_process("locker2_int", 30) <0)
        printf("Error creando locker2_int\n");

    if (mutex_lock(d3)<0)
        printf("Error lock de m3\n");

    printf("locker1_int termina\n");

    return 0; 
}
