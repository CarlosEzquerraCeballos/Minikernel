/*
 * user/creador_mut_error.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que crea un mutex pasándole un argumento erróneo;
 * el proceso no debe terminar
 */

#include "services.h"

int main(){
    printf("creador_mut_error comienza\n");

    char *n = (char *) 0xfffffffffff00000;
    if (mutex_open(n)<0)
        printf("Error abriendo mutex\n");

    printf("creador_mut_error termina\n");

    return 0;
}
