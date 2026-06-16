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

// Para comprobar la llamada proc_sleep

#include "services.h"

int main(){
    printf("init comienza\n");

    if (create_process("dormilon", 10)<0)
        printf("Error creando dormilon\n");

    if (create_process("dormilon", 12)<0)
        printf("Error creando dormilon\n");

    printf("init termina\n");
    return 0; 
}
