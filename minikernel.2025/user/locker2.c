/*
 * user/locker2.c
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
    int d1, d2;
    printf("locker2 comienza\n");

    if (((d1=mutex_open("m1"))<0) || (d1!=0))
        printf("Error abriendo m1\n");

    if (((d2=mutex_open("m2"))<0) || (d2!=1))
        printf("Error abriendo m2\n");

    if (mutex_lock(d1)<0)
        printf("Error lock m1\n");

    if (mutex_unlock(d1)<0)
        printf("Error unlock m1\n");

    if (mutex_lock(d2)<0)
        printf("Error lock m1\n");

    if (mutex_unlock(d2)<0)
        printf("Error unlock m2\n");

    printf("locker2 termina\n");

    return 0; 
}
