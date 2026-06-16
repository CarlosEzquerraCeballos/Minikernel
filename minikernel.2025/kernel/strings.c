/*
 *  kernel/strings.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene funciones de manejo de strings */

#include "strings.h"
#include "common.h"

// Compara dos strings (con un tamaño máximo, que debe incluir el nulo);
// devuelve 0 si ==; -1 si s1 < s2; 1 si s1 > s2
int str_cmp(const char * s1, const char *s2, int max_length){
    if (!max_length) return 0;
    if (!s1 || !s2) return (s1 == s2) ? 0 : ((s1 == NULL) ? -1 : 1);
    for (; *s1 && *s2 && (*s1 == *s2); s1++, s2++)
        if (--max_length == 0) break;
    return (*s1 > *s2) ? 1 : ((*s1 == *s2) ? 0 : -1);
}
// Copia un string (con un tamaño máximo, que debe incluir el nulo)
char * str_cpy(char *dest, const char *src, int max_length){
    if (!max_length || !src || !dest) return NULL;
    char * res = dest;
    for (; *src && max_length--; *dest++ = *src++);
    if (max_length) *dest = 0;
    return res;
}
// Longitud del string (con un tamaño máximo, que debe incluir el nulo)
int str_len(const char * s, int max_length){
    if (!s) return -1;
    int i = 0;
    for (; *s && max_length--; s++, i++);
    return *s ? -1 : i;
}

