/*
 *  kernel/include/strings.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _STRINGS_H
#define _STRINGS_H

// Compara dos strings (con un tamaño máximo, que debe incluir el nulo);
// devuelve 0 si ==; -1 si s1 < s2; 1 si s1 > s2
int str_cmp(const char * s1, const char *s2, int max_length);

// Copia un string (con un tamaño máximo, que debe incluir el nulo)
char * str_cpy(char *dest, const char *src, int max_length);

// Longitud del string (con un tamaño máximo, que debe incluir el nulo)
int str_len(const char * s, int max_length);

#endif /* _STRINGS_H */

