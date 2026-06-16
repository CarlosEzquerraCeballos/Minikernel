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

#include "services.h"

int main(){
    printf("init comienza\n");

    if (create_process("yosoy", 12)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 10)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 14)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 12)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 10)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 14)<0)
        printf("Error creando yosoy\n");

    if (create_process("yosoy", 18)<0)
        printf("Error creando yosoy\n");

    printf("init termina\n");

    return 0; 
}
