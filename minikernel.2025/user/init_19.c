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

// Para comprobar la llamada get_char dejando a init activo

#include "services.h"

#define TOT_ITER 8000000000

int main(){
    printf("init comienza\n");

    if (create_process("lector", 20)<0)
        printf("Error creando lector\n");

    if (create_process("lector", 30)<0)
        printf("Error creando lector\n");

    printf("teclee caracteres\n");

    long x = 0;

    for (long i = 0; i < TOT_ITER; i++) x += i;

    printf("init termina\n");
    return 0; 
}
