/*
 *  kernel/include/syscall.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _SYSCALL_H
#define _SYSCALL_H

// indica si SO está comprobando argumento de tipo puntero pasado a llamada
extern int check_arg;

// verifica que es un string correcto accesible para lectura
int check_syscall_arg_string_read(char *str, int max_length);

// verifica que es un puntero que referencia una zona accesible para lectura
int check_syscall_arg_pointer_read(char *addr, int length);

// verifica que es un puntero que referencia una zona accesible para escritura
int check_syscall_arg_pointer_write(char *addr, int length);

void init_syscall_module(void); // función de iniciación del módulo

#endif /* _SYSCALL_H */

