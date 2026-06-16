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

// Para comprobar la competencia en la llamada get_char

#include "services.h"

#define TOT_ITER 2000000000

int main(){
    printf("init comienza\n");

    if (create_process("lector2", 10)<0)
        printf("Error creando lector\n");

    proc_sleep(1); // duerme para que arranque lector2

    printf("teclee un solo caracter\n");

    long x = 0;

    for (long i = 0; i < TOT_ITER; i++) x += i;

    printf("init ha leído %c\n", get_char());

    proc_sleep(1);

    printf("teclee un segundo  caracter\n");

    printf("init termina\n");
    return 0; 
}
