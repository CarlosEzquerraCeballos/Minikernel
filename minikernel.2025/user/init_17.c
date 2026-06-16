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

// Para probar el interbloqueo

#include "services.h"

int main(){
    int d1, d2;
    printf("init comienza\n");

    if ((d1=mutex_open("m1"))<0)
        printf("Error abriendo m1\n");

    if ((d2=mutex_open("m2"))<0)
        printf("Error abriendo m2\n");

    if (mutex_lock(d1)<0)
        printf("Error lock de m1\n");

    if (create_process("locker1_int", 20) <0)
        printf("Error creando locker1_int\n");

    if (mutex_lock(d2)<0) {
        printf("Debe aparecer este error en lock porque hay interbloqueo\n");
	return -1;
    }
    else
        printf("No ha detectado el interbloqueo\n");

    printf("init termina\n");

    return 0; 
}
