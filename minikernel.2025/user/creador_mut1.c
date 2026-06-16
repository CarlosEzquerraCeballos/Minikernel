/*
 * user/creador_mut1.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que crea varios mutex y se va a dormir;
 * usa cierre explícito
 */

#include "services.h"

int main(){
    int d0, d1, d2, d3, d4;
    printf("creador_mut1 comienza\n");

    if (((d0=mutex_open("m1"))<0) || (d0!=0))
        printf("Error abriendo m1\n");

    if (((d1=mutex_open("m1"))<0) || (d1!=1)) // lo vuelve a abrir
        printf("Error volviendo a abrir m1\n");

    if (((d2=mutex_open("m2"))<0) || (d2!=2))
        printf("Error abriendo m2 %d\n", d2);

    if (((d3=mutex_open("m3"))<0) || (d3!=3))
        printf("Error abriendo m3\n");

    if (mutex_open("m4")>=0)
        printf("Error: Debería haber dado un error al abrir m4 por superar el número de descriptores por proceso\n");

    if (mutex_close(d1)<0)
        printf("Error cerrando m1\n");

    if (mutex_close(d1)>=0)
        printf("Error: Debería haber dado un error al cerrar m1 por no estar abierto\n");

    if (mutex_close(1024)>=0)
        printf("Error: Debería haber dado un error al cerrar un descriptor inválido\n");

    if (((d4=mutex_open("m4"))<0) || (d4!=1))
        printf("Error abriendo m4\n");

    proc_sleep(1);

    if (mutex_close(d0)<0)
        printf("Error cerrando m1\n");

    proc_sleep(2);

    if (mutex_close(d2)<0)
        printf("Error cerrando m2\n");

    if (mutex_close(d3)<0)
        printf("Error cerrando m3\n");

    if (mutex_close(d4)<0)
        printf("Error cerrando m4\n");

    printf("creador_mut1 termina\n");

    return 0;
}
