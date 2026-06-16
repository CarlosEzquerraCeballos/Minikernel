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

// genera excepción de memoria: init no debe terminar pero yosoy sí

#include "services.h"

int main(){
    printf("init comienza\n");

    if (create_process("yosoy", 1) <0) // para comprobar que no se ve afectado
        printf("Error creando yosoy\n");

/* BLOQUE 1: acceso a memoria inválido -> debe terminar init pero no el SO

    int *pi = 0;
    *pi = 5; // excepción de memoria
*/

// BLOQUE 2: llamada con puntero no accesible -> debe terminar init pero no el SO
    void *p = (void *) 0xfffffffffff00000;
    print(p, 4); // se le pasa dir no accesible (pasa igual con create_process)


    printf("init termina\n");

    return 0; 
}
