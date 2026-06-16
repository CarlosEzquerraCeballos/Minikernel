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

// Para probar el open y close de los mutex

#include "services.h"


int main(){
    int d, d0, d1;
    printf("init comienza\n");

    if (((d0=mutex_open("m1"))<0) || (d0!=0))
        printf("Error abriendo m1\n");

    if (((d1=mutex_open("m1"))<0) || (d1!=1)) // lo vuelve a abrir
        printf("Error volviendo a abrir m1\n");

    if (create_process("creador_mut_error", 20)<0)
        printf("Error creando creador_mut_error\n");

    if (create_process("creador_mut1", 20)<0) // crea 4 mutex
        printf("Error creando creador_mut1\n");

    if (create_process("creador_mut2", 20)<0) // crea 4 mutex
        printf("Error creando creador_mut2\n");

    if (create_process("creador_mut3", 20)<0) // crea 4 mutex
        printf("Error creando creador_mut3\n");

    if (create_process("creador_mut4", 20)<0) // crea 4 mutex
        printf("Error creando creador_mut4\n");

    if (mutex_open("m17")>=0)
        printf("Error: Debería haber dado un error al abrir m17 por estar la tabla de mutex llena\n");

    proc_sleep(2);

    // creador_mut1 ha cerrado "m1" pero no se ha liberado
    // porque yo lo tengo abierto dos veces

    if (mutex_open("m17")>=0)
        printf("Error: Debería haber dado un error al abrir m17 por estar la tabla de mutex llena\n");

    if (mutex_close(d0)<0) //lo cierro una vez
        printf("Error cerrando m1\n");

    if (mutex_open("m17")>=0) // no es suficiente
        printf("Error: Debería haber dado un error al abrir m17 por estar la tabla de mutex llena\n");

    if (mutex_close(d1)<0) // lo cierro otra vez
        printf("Error cerrando m1\n");

    if ((d=mutex_open("m17"))<0) // ya hay hueco en la tabla para crearlo
        printf("Error abriendo m17\n");

    if (mutex_close(d)<0)
        printf("Error cerrando m1\n");

    proc_sleep(3); // vuelve a dormir

    // todos los demás procesos han terminado: la tabla debe estar vacía
    // se deben poder crear 16 mutex
    char mut[] = "mut_a";
    for (int i = 0; i < 4; i++, mut[4]++)
        if ((d=mutex_open(mut))<0)
            printf("Error abriendo %s %d\n", mut, d);

    // debe fallar porque no hay descriptores locales
    if (mutex_open(mut)>=0)
        printf("Error: Debería haber dado un error al abrir %s porque no hay descriptores localesa\n", mut);

    printf("init termina\n");

    return 0; 
}
