/*
 *  kernel/include/terminal.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H

// tamaño del buffer del terminal
#define TERM_BUF_SIZE   8

void init_terminal_module(void);

// Implementación de la llamada al sistema
int do_print(char * buf, int size);

#endif /* _TERMINAL_H */

