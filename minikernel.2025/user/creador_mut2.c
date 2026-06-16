/*
 * user/creador_mut2.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que crea varios mutex y se va a dormir;
 * usa cierre implícito
 */

#include "services.h"

int main(){
    int d1, d2, d3, d4;
    printf("creador_mut2 comienza\n");

    if (((d1=mutex_open("m5"))<0) || (d1!=0))
        printf("Error abriendo m5\n");

    if (((d2=mutex_open("m6"))<0) || (d2!=1))
        printf("Error abriendo m6\n");

    if (((d3=mutex_open("m7"))<0) || (d3!=2))
        printf("Error abriendo m7\n");

    if (((d4=mutex_open("m8"))<0) || (d4!=3))
        printf("Error abriendo m8\n");

    proc_sleep(3);

    printf("creador_mut2 termina\n");

    return 0;
}
