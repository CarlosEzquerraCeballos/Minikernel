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

// Lanza 4 procesos que escriben su PID y su prioridad
//
#include "services.h"

int main(){
    printf("init comienza\n");
    for (int i=1; i<=4; i++)
        if (create_process("yosoy", i * 2)<0) // cada vez más prioridad
            printf("Error creando yosoy\n");
    printf("init termina\n");

    return 0; 
}
