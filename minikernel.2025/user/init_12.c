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

// Para probar que en round-robin proceso desbloqueado recupera su rodaja

#include "services.h"

#define TOT_ITER 80000000

int main(){
    printf("init comienza\n");

    if (create_process("yosoy_largo", get_priority())<0) // misma prioridad
        printf("Error creando yosoy_largo\n");

    long x = 0;
    for (long i = 0; i < TOT_ITER; i++) x += i; // init gasta un poco de tiempo

    printf("init se duerme\n");
    proc_sleep(1);

    for (long i = 0; i < TOT_ITER; i++) x += i; // init gasta más tiempo

    printf("init termina\n");

    return 0; 
}
