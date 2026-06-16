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

// genera excepción aritmérica: init no debe terminar pero yosoy sí
#include "services.h"

int main(){
    printf("init comienza\n");

    if (create_process("yosoy", 1) <0) // para comprobar que no se ve afectado
        printf("Error creando yosoy\n");

    int x = 1, y = 0;
    x /= y; // excepción aritmética

    printf("init termina\n");

    return 0; 
}
