/*
 * user/yosoy.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#include "services.h"

int main(){
    int segs, pid;

    pid=get_pid();
    printf("dormilon (%d): comienza\n", pid);

    /* primero duerme 1 segundo */
    printf("dormilon (%d) duerme 1 segundo\n", pid);
    proc_sleep(1);

    /* después duerme número de segundos dependiendo de su pid */
    segs=pid+1;
    printf("dormilon (%d) duerme %d segundos\n", pid, segs);
    proc_sleep(segs);

    printf("dormilon (%d): termina\n", pid);
    return 0;
}
