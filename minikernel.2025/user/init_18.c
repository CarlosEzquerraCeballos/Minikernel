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

// Para comprobar la llamada get_char

#include "services.h"

int main(){
    printf("init comienza\n");

    if (create_process("lector", 20)<0)
        printf("Error creando lector\n");

    if (create_process("lector", 30)<0)
        printf("Error creando lector\n");

    printf("init termina\n");
    return 0; 
}
