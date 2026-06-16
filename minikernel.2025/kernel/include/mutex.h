/*
 *  kernel/include/mutex.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#ifndef _MUTEX_H
#define _MUTEX_H

#define MAX_MUTEX_NAMELENGTH	32
#define MAX_NR_MUTEX		16
#define MAX_NR_MUTEX_PER_PROC	4

void init_mutex_module(void);


#endif /* _MUTEX_H */

