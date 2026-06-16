/*
 * user/locker3.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Programa inicial que arranca el S.O.
   Sólo se ejecutarán los programas que se incluyan aquí, que, evidentemente,
   pueden ejecutar otros programas... */

// Para probar el lock y unlock de los mutex

#include "services.h"


int main(){
    int d;
    printf("locker3 comienza\n");

    if (((d=mutex_open("m"))<0) || (d!=0))
        printf("Error abriendo m\n");

    if (mutex_lock(d)<0)
        printf("Error lock m\n");

    printf("locker3 termina\n"); // cierre implícito con unlock

    return 0; 
}
