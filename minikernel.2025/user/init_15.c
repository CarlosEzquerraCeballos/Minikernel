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
    int d;
    printf("init comienza\n");

    if (((d=mutex_open("m"))<0) || (d!=0))
        printf("Error abriendo m\n");

    if (mutex_lock(d)<0)
        printf("Error lock m\n");

    if (create_process("locker3", 10)<0) // menos prioritario
        printf("Error creando locker1\n");

    printf("init duerme 1 segundo\n");
    proc_sleep(1);

    if (mutex_unlock(d)<0)
        printf("Error unlock m\n");

    if (mutex_lock(d)<0)
        printf("Error lock m\n");

    printf("init vuelve a dormir 1 segundo\n");
    proc_sleep(1);

    printf("init termina\n"); // cierre implícito con unlock

    return 0; 
}
