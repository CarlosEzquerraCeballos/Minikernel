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

// muestra el uso de las tres llamadas inicialmente implementadas
// y de la función de biblioteca printf, que, a su vez, llama a print;
// ilustra también la jerarquía de procesos y el uso de prioridades

#include "services.h"

int main(){
    print("Hola ", 5); // llamada al sistema
    printf("%s empieza\n", __FILE__); // función de biblioteca: llama a "print"

    if (create_process("noexiste", 1) <0) // debe imprir el error
        printf("Error creando noexiste\n");

    if (create_process("lanza", 8) <0) 
        printf("Error creando lanza\n");

    printf("init termina\n");

    exit_process(); // llamada explícita; si no, se hace al terminar "main"
    print("adios", 5); 	// esto no se ejecuta
    return 0; 
}
