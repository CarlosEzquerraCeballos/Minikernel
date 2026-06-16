/*
 * user/lanza.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

// deben ejecutarse en orden de prioridad

#include "services.h"

int main(){
    printf("lanza: inicia\n");

    if (create_process("simplon", 12) <0) 
        printf("Error creando simplon\n");

    if (create_process("simplon", 14) <0) 
        printf("Error creando simplon\n");

    if (create_process("simplon", 10) <0) 
        printf("Error creando simplon\n");


    printf("lanza: termina\n");
    return 0;  // final implícito
}
