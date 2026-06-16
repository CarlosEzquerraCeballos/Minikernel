/*
 *  user/include/services.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Fichero de cabecera que contiene los prototipos de funciones de
 * biblioteca que proporcionan la interfaz de llamadas al sistema,
 * así como de la función printf
 */

#ifndef _SERVICES_H
#define _SERVICES_H

/* Función de biblioteca */
#define printf print_f

int print_f(const char *formato, ...);

/* Llamadas al sistema proporcionadas */
int create_process(char *prog,  int prio);
int exit_process(void);
int print(char *buf, unsigned int size);
int get_pid(void);
int get_priority(void);

#endif /* _SERVICES_H */

