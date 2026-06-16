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

// Para probar round-robin lanza 8 procesos: 4 con una prio y 4 con otra menor

#include "services.h"

int main(){
    printf("init comienza\n");
    for (int i=1; i<=8; i++)
	// procesos 1, 3, 5 y 7 más prioridad que 2, 4, 6 y 8
        if (create_process("mudo", 4 + (i % 2))<0)
            printf("Error creando mudo\n");
    printf("init termina\n");

    return 0; 
}
