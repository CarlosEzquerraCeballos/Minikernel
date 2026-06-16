/*
 * user/creador_mut3.c
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
    printf("creador_mut3 comienza\n");

    if (((d1=mutex_open("m9"))<0) || (d1!=0))
        printf("Error abriendo m9\n");

    if (((d2=mutex_open("m10"))<0) || (d2!=1))
        printf("Error abriendo m10\n");

    if (((d3=mutex_open("m11"))<0) || (d3!=2))
        printf("Error abriendo m11\n");

    if (((d4=mutex_open("m12"))<0) || (d4!=3))
        printf("Error abriendo m12\n");

    proc_sleep(3);

    printf("creador_mut3 termina\n");

    return 0;
}
