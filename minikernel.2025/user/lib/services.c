/*
 *  user/lib/services.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 *
 * Fichero que contiene las definiciones de las funciones de interfaz
 * a las llamadas al sistema. Usa la funcion de apoyo sys_call.
 *
 */

#include "syscall_nr.h"
#include "services.h"

/* Función interna que prepara el código de la llamada
   (en el registro 0), los parámetros (en registros 1, 2, ...), realiza la
   instruccion de llamada al sistema y devuelve el resultado 
   (que obtiene del registro 0) */

int sys_call(int nr_syscall, int nr_args, ... /* args */);

/* Funciones interfaz a las llamadas al sistema */

int create_process(char *prog, int prio){
    return sys_call(CREATE_PROCESS, 2, (long)prog, (long)prio);
}
int exit_process(void){
    return sys_call(EXIT_PROCESS, 0);
}
int print(char *texto, unsigned int longi){
    return sys_call(PRINT, 2, (long)texto, (long)longi);
}
int get_pid(void){
    return sys_call(GET_PID, 0);
}
int get_priority(void){
    return sys_call(GET_PRIORITY, 0);
}
