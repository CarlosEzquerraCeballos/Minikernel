/*
 * user/simplon.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#include "services.h"

#define TOT_ITER 10

int main(){
    int i;

    printf("simplon: inicia\n");

    for (i=0; i<TOT_ITER; i++)
        printf("simplon: i %d\n", i);

    printf("simplon: termina\n");
    return 0;  // final implícito
}
