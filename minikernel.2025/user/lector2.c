/*
 * user/lector.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

// Para probar get_char

#include "services.h"

int main(){

    printf("lector2 comienza quedando a la espera de un carácter\n");

    printf("lector2 ha leído %c\n", get_char());

    printf("lector2 termina\n");
    return 0;
}
