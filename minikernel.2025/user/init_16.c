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

// Para probar el auto-interbloqueo

#include "services.h"


int main(){
    int d;
    printf("init comienza\n");

    if ((d=mutex_open("m"))<0)
        printf("Error abriendo m\n");

    if (mutex_lock(d)<0)
        printf("Error lock de m\n");

    if (mutex_lock(d)<0) {
        printf("Debe aparecer este error en lock porque hay interbloqueo\n");
	return -1;
    }
    else
        printf("No ha detectado el interbloqueo\n");

    printf("init termina\n");

    return 0; 
}
