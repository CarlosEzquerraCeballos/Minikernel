/*
 * user/lector.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

// Para probar get_char

#include "services.h"

int main(){
    int pid;

    pid=get_pid();
    printf("lector (%d): comienza quedando a la espera de un carácter\n", pid);

    char c = get_char();
    printf("lector (%d) ha leído %c\n", pid, c);

    printf("lector (%d) duerme 3 segundos\n", pid);
    proc_sleep(3);

    for (int i=1; i<=10; i++) {
        c = get_char();
        printf("lector (%d) ha leído %c\n", pid, c);
    }
    printf("lector (%d): termina\n", pid);
    return 0;
}
