/*
 * user/creador_mut4.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que crea varios mutex y se va a dormir;
   usa cierre implícito
 */

#include "services.h"

int main(){
    int d1, d2, d3, d4;
    printf("creador_mut4 comienza\n");

    if (((d1=mutex_open("m13"))<0) || (d1!=0))
        printf("Error abriendo m13\n");

    if (((d2=mutex_open("m14"))<0) || (d2!=1))
        printf("Error abriendo m14\n");

    if (((d3=mutex_open("m15"))<0) || (d3!=2))
        printf("Error abriendo m15\n");

    if (((d4=mutex_open("m16"))<0) || (d4!=3))
        printf("Error abriendo m16\n");

    proc_sleep(3);

    printf("creador_mut4 termina\n");

    return 0;
}
