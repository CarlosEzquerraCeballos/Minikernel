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

// Para probar el lock y unlock de los mutex

#include "services.h"


int main(){
    int d1, d2;
    printf("init comienza\n");

    if (((d1=mutex_open("m1"))<0) || (d1!=0))
        printf("Error abriendo m1\n");

    if (((d2=mutex_open("m2"))<0) || (d2!=1))
        printf("Error abriendo m2\n");

    if (mutex_lock(3)>=0)
        printf("Error: Debería haber dado un error al hacer un lock con descriptor erróneo\n");

    if (mutex_lock(1024)>=0)
        printf("Error: Debería haber dado un error al hacer un lock con descriptor erróneo\n");

    if (mutex_unlock(3)>=0)
        printf("Error: Debería haber dado un error al hacer un unlock con descriptor erróneo\n");

    if (mutex_unlock(1024)>=0)
        printf("Error: Debería haber dado un error al hacer un unlock con descriptor erróneo\n");

    if (mutex_unlock(d1)>=0)
        printf("Error: Debería haber dado un error al hacer un unlock sobre un mutex que no posee\n");

    if (mutex_lock(d1)<0)
        printf("Error lock m1\n");

    if (mutex_lock(d2)<0)
        printf("Error lock m1\n");

    if (create_process("locker1", 20)<0)
        printf("Error creando locker1\n");

    if (create_process("locker2", 30)<0)
        printf("Error creando locker2\n");

    if (mutex_close(d1)<0) // unlock implícito
        printf("Error close m1\n");

    printf("init termina\n"); // cierre implícito con unlock

    return 0; 
}
