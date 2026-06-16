/*
 *  kernel/include/bitmask.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _BITMASK_H
#define _BITMASK_H


// Busca el primer bit (de menor peso) a 1
int find_first_bit_set(unsigned int m);

// Busca el último bit (de mayor peso) a 1
int find_last_bit_set(unsigned int m);

// Pone el bit a 1
void set_bit(unsigned int *m, int bit);

// Pone el bit a 0
void clear_bit(unsigned int *m, int bit);

// Devuelve si el bit está a 1
int is_set_bit(unsigned int m, int bit);

#endif /* _BITMASK_H */
