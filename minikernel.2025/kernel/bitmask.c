/*
 *  kernel/bitmask.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene funciones que manejan una máscara de bits
   almacenada en un entero de 32 bits */

#include "bitmask.h"
#include "common.h"

// Busca el primer bit (de menor peso) a 1
int find_first_bit_set(unsigned int m) {
    int i;
    for (i=0; !(m & 0x1) && (i<sizeof(m)*8); m>>= 1, i++);
    return (m & 0x1) ? i : -1;
}

// Busca el último bit (de mayor peso) a 1
int find_last_bit_set(unsigned int m) {
    int i;
    for (i=0; !(m & (1 << 31)) && (i<sizeof(m)*8); m<<= 1, i++);
    return (m & (1 << 31)) ? 31 - i : -1;
}

// Pone el bit a 1
void set_bit(unsigned int *m, int bit) {
    if (bit<32) *m |= (1<<bit);
}
// Pone el bit a 0
void clear_bit(unsigned int *m, int bit) {
    if (bit<32) *m &= (~(1<<bit));
}
// Devuelve si el bit está a 1
int is_set_bit(unsigned int m, int bit) {
    return (bit<32) ? m & (1<<bit) : -1;
}
