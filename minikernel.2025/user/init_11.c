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

// Para probar que en round-robin proceso expulsado mantiene su rodaja

#include "services.h"

#define TOT_ITER 20000000

int main(){
    printf("init comienza\n");
    if (create_process("dormilon", 20)<0)
        printf("Error creando dormilon\n");

    long x = 0;
    for (long i = 0; i < TOT_ITER; i++) x += i; // init gasta un poco de tiempo

    for (int i=1; i<=2; i++)
        if (create_process("yosoy_largo", 10)<0)
            printf("Error creando yosoy_largo\n");
    printf("init termina\n");

    return 0; 
}
