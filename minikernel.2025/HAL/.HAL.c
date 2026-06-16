/*
 *  HAL/HAL.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "HAL.h"

/* Señales "en la sombra". Cada una de ellas indica si está realmente
   activa la señal asociada. Nótese que la presencia de una señal en
   la máscara activa puede deberse a qué se ha disparado esa señal
   o a que se ha bloqueado. La señal "en la sombra" permite distinguirlo.
*/
   
#define SIGALRM_ACTIVA SIGPROF
#define SIGIO_ACTIVA SIGXCPU
#define SIGUSR_ACTIVA SIGXFSZ


/* Los registros generales */
long registros[NR_REGS];

/* Apunta al directorio base de la ejecución */
char * dir_base;

/* Tipo que representa una entrada en la tabla de vectores */
typedef struct{
    void (*fmanej)();
} vec_int;

/* Vectores de interrupción */
static vec_int int_vector_table[NR_VECTORS];

/* Llevamos la cuenta de cuántos procesos hay. Cuando no haya ninguno,
   se termina la ejecución del sistema. No es muy realista pero es
   más cómodo para comprobar si la práctica funciona bien */
static int nprocs=0;

static void *stack_to_release = NULL; // pila pendiente de liberar

/* Instala un manejador de interrupciones */
void register_irq_handler(int nvector, void (*manej)()){
    if ((nvector>NR_VECTORS)||(nvector<0))
        panic("usando un vector no existente");

    int_vector_table[nvector].fmanej=manej;
}

/* El printk del sistema que invoca a la funcíón "print_terminal" */
int printk(const char *formato, ...){
    char buffer[1024];
    va_list args;
    int res;

    va_start(args, formato);
    res=vsprintf(buffer, formato, args);
    if (res>0)
        print_terminal(buffer, strlen(buffer));
    return res;
}

/* Si todo va mal es mejor escapar dejando las cosas bien */
void panic(char *mens){
    struct termios term_atrib;

    printk("-> PÁNICO: TERMINA EL SO\n");
    set_int_priority_level(LEVEL_3);
    write(1, mens, strlen(mens));
    write(1, "\n", 1);
    tcflush(0, TCIFLUSH);
    tcgetattr(0, &term_atrib);
    term_atrib.c_lflag |= ICANON;
    term_atrib.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &term_atrib);
    exit(1);
}

/* Función interna que escribe en la pantalla */
void print_terminal(char *texto, unsigned int longi) {
    write(1, texto, longi);
}

/* "Instrucción" que detiene la ejecución del "procesador" */
void halt(void){
    pause();
}

/* Salvaguarda de los registros generales */
static void salvar_registros(long * regs) {
    for (int i=0; i<NR_REGS; i++) regs[i]=registros[i];
}

/* Restauración de los registros generales */
static void restaurar_registros(long * regs) {
    for (int i=0; i<NR_REGS; i++) registros[i]=regs[i];
}

/* Operación de salvaguarda y recuperación de contexto hardware del proceso */
void context_switch(context *contexto_a_salvar, context *contexto_a_restaurar){
    if (contexto_a_salvar!=NULL) {
        if (contexto_a_salvar!=contexto_a_restaurar) {
            salvar_registros(contexto_a_salvar->registros);
            swapcontext(&(contexto_a_salvar->ctxt), &(contexto_a_restaurar->ctxt));
            if (stack_to_release) {
                kfree(stack_to_release); stack_to_release = NULL;
            }

            restaurar_registros(contexto_a_salvar->registros);
        }
    }
    else
        setcontext(&(contexto_a_restaurar->ctxt));
}

/* crea mapa de memoria a partir de ejecutable "prog" devolviendo un
descriptor de dicho mapa y la dirección del punto de arranque del programa */

void * create_image(char *prog, void **dir_ini){
    char ejec[128];
    void *imagen;
    long **regdir;

    /* Busca ejecutable en el directorio usuario */
    sprintf(ejec, "%s../user/%s", dir_base, prog);
    if ((imagen=dlopen(ejec, RTLD_LAZY))!=NULL)
           if ((*dir_ini=dlsym(imagen, "main"))!=NULL) {
            if ((regdir=dlsym(imagen, "reglib"))!=NULL) {
                *regdir = &(registros[0]);
                nprocs++;
                return imagen;
            }
        }
    return NULL;
}

void *kmalloc(int size) {
    int * p = mmap(NULL, size+sizeof(int), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
    *p = size;
    return p + 1;
}

void kfree(void *ptr) {
    int * p = ptr;
    munmap(p + 1, *p);
}

/* Crea la pila del tamaño especificado */
void *create_stack(int size) {
    stack_to_release = NULL;
    return kmalloc(size);
}

/* Libera la pila */
void release_stack(void *stack) {
    stack_to_release = stack;
    return;
}

/* Función de arranque de un proceso que asegura que empieza en modo
 * sistema
 */
static void lanzadera(void (*fini) (void (*)(void)), void (*fmain) (void)) {
    sigset_t mascara;

    if (stack_to_release) {
        kfree(stack_to_release); stack_to_release = NULL;
    }
    sigemptyset(&mascara);
    sigprocmask(SIG_SETMASK, &mascara, NULL);
    fini(fmain);
}

/* función auxiliar para arquitecturas de 64 bits que, dado que makecontext
 * sólo puede pasar argumentos enteros y necesitamos pasar direcciones,
 * ensambla los cuatro enteros en 2 punteros e invoca la lanzadera original.
 */

static void lanzadera_64(int a, int b, int c, int d) {
    void *dir1;
    void *dir2;
    unsigned long int la=(unsigned int)a, lb=(unsigned int)b, lc=(unsigned int)c, ld=(unsigned int)d;

    dir1= (void *)(la | lb << sizeof(long)*4);
    dir2= (void *)(lc | ld << sizeof(long)*4);

    lanzadera(dir1, dir2);
}

/* Crea el contexto inicial del proceso */
void set_initial_context(void *mem, void *pila, int tam_pila, void * dir_ini, context *contexto_inicial){
    void (*dir_start)();
    void (*dir_main)();
    ucontext_t *contexto_ini;

    contexto_ini=&(contexto_inicial->ctxt);

    /* Punto de arranque del programa: "start" */
    if ((dir_start=(void (*)())dlsym(mem, "start"))==NULL)
        return;

    dir_main= (void (*)())dir_ini;

    getcontext(contexto_ini);
    contexto_ini->uc_link=0;
    contexto_ini->uc_stack.ss_sp=pila;
    contexto_ini->uc_stack.ss_size=tam_pila;

    /* el proceso arrancará en modo sistema con int. prohibidas*/
    sigemptyset(&(contexto_ini->uc_sigmask));
    sigaddset(&(contexto_ini->uc_sigmask), SIGALRM);
    sigaddset(&(contexto_ini->uc_sigmask), SIGIO);
    sigaddset(&(contexto_ini->uc_sigmask), SIGUSR1);
    sigaddset(&(contexto_ini->uc_sigmask), SIGUSR2);

    /* lanzadera: función de arranque en modo sistema */
    if (sizeof(void *) == sizeof(int))
        makecontext(contexto_ini, (void (*)())lanzadera, 2, //32 bits
            dir_start, dir_main); 
    else {
        int i, j, k, l;
        i=(unsigned long)dir_start;
        j=(unsigned long)dir_start>>sizeof(long)*4;
        k=(unsigned long)dir_main;
        l=(unsigned long)dir_main>>sizeof(long)*4;

        makecontext(contexto_ini, (void (*)())lanzadera_64, 4, //64 bits
            i, j, k, l); 
    }
}

/* Libera la imagen del proceso */
void release_image(void * info_mem){
    struct termios term_atrib;

    dlclose(info_mem);
    /* si ya no quedan procesos nos vamos...*/
    nprocs--;
    if (nprocs==0) {
        printk("-> NO HAY PROCESOS EN EL SISTEMA: TERMINA EL SO\n");
	tcflush(0, TCIFLUSH);
        tcgetattr(0, &term_atrib);
        term_atrib.c_lflag |= ICANON;
        term_atrib.c_lflag |= ECHO;
        tcsetattr(0, TCSANOW, &term_atrib);
        exit(0);
    }
}

/* Genera una interrupción software */
void activate_soft_int(void) {
        kill(getpid(), SIGUSR2);
}

/* Permite establecer un determinado nivel de interrupción del procesador
   devolviendo el previo */
int set_int_priority_level(int nivel){
    sigset_t mascara, mascara_previa;
    sigset_t mascara_actual;

    /* Obtiene mascara actual */
    sigprocmask(SIG_SETMASK, NULL, &mascara_actual);

    /* Prepara la máscara que representa el nuevo nivel */
    sigemptyset(&mascara);

    /* Hay que mantener las señales "en la sombra" */

    if (sigismember(&mascara_actual, SIGALRM_ACTIVA))
        sigaddset(&mascara, SIGALRM_ACTIVA);

    if (sigismember(&mascara_actual, SIGIO_ACTIVA))
        sigaddset(&mascara, SIGIO_ACTIVA);

    if (sigismember(&mascara_actual, SIGUSR_ACTIVA))
        sigaddset(&mascara, SIGUSR_ACTIVA);

    /* Fijando máscara adecuada al nivel */
    switch (nivel) {
        case LEVEL_3:
            sigaddset(&mascara, SIGALRM);
        case LEVEL_2:
            sigaddset(&mascara, SIGIO);
        case LEVEL_1:
            sigaddset(&mascara, SIGUSR1);
            sigaddset(&mascara, SIGUSR2);
    }

    if (sigprocmask(SIG_SETMASK, &mascara, &mascara_previa)){
        perror("sigprocmask");
    }
    /* Analizando nivel previo a partir de la máscara */
    if (sigismember(&mascara_previa, SIGALRM))
        return LEVEL_3;
    else if (sigismember(&mascara_previa, SIGIO))
        return LEVEL_2;
    else
        return LEVEL_1;
}

/* Devuelve verdadero si el modo previo de ejecución del procesador
   era usuario */
int comes_from_usermode(void){
    sigset_t mascara_actual;
    int anidamiento=0;

    sigprocmask(SIG_SETMASK, NULL, &mascara_actual);
    if (sigismember(&mascara_actual, SIGALRM_ACTIVA)) anidamiento++;
    if (sigismember(&mascara_actual, SIGIO_ACTIVA)) anidamiento++;
    if (sigismember(&mascara_actual, SIGUSR_ACTIVA)) anidamiento++;
    if (sigismember(&mascara_actual, SIGILL)) anidamiento++;
    if (sigismember(&mascara_actual, SIGBUS)) anidamiento++;
    if (sigismember(&mascara_actual, SIGFPE)) anidamiento++;
    if (sigismember(&mascara_actual, SIGSEGV)) anidamiento++;

    return (anidamiento<=1);
}

/* función que trata una excepción invocando la rutina especificada por el SO */
static void man_exc_preludio(int sennal){

        if (sennal==SIGFPE)
                (int_vector_table[ARITM_EXC].fmanej)();
        else
                (int_vector_table[MEM_EXC].fmanej)();
        return;
}

/* función que trata una interrupción invocando la rutina especificada
   por el SO */
static void man_int_preludio(int sennal){
    fd_set mascara;
    struct timeval plazo_zero={0, 0};
    int nivel_previo;

    if (sennal==SIGIO) {
        FD_ZERO(&mascara);
        FD_SET(0, &mascara);
        nivel_previo=set_int_priority_level(LEVEL_3);
        if (select(1, &mascara, NULL, NULL, &plazo_zero)>0) {
                set_int_priority_level(nivel_previo);
                (int_vector_table[KEYBOARD_INT].fmanej)();
               }
            else
                set_int_priority_level(nivel_previo);
    }
    else if (sennal==SIGALRM) {
            (int_vector_table[CLOCK_INT].fmanej)();
    }
    else
        panic("INTERRUPCION INESPERADA");
    return;
}

/* función que trata una llamada invocando la rutina especificada por el SO */
static void llam_sis_preludio(int x){

        (int_vector_table[SYS_CALL].fmanej)();
        return;
}

/* función que trata una interrupción software invocando la rutina
   especificada por el SO */
static void int_sw_preludio(int x){

        (int_vector_table[SW_INT].fmanej)();
        return;
}

/* Inicia el terminal sin eco y no orientado a líneas */
static void init_terminal(void) {
    struct termios term_atrib;

    if (tcgetattr(0, &term_atrib)<0)
    {
        perror("Error obteniendo atributos del terminal");
        exit(1);
    }

    term_atrib.c_lflag &= ~ICANON;
    term_atrib.c_lflag &= ~ECHO;

    term_atrib.c_cc[VMIN]=1;
    term_atrib.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &term_atrib);

}
/* Establece las funciones de tratamiento de señales y sus máscaras
   de bloqueo */
void init_interrupt_controller(void){
    struct sigaction accion;
    
    accion.sa_handler=int_sw_preludio;
    sigemptyset(&accion.sa_mask);
    sigaddset(&accion.sa_mask, SIGUSR_ACTIVA);
    accion.sa_flags=0;
    sigaction(SIGUSR2, &accion, NULL);

    accion.sa_handler=llam_sis_preludio;
    sigemptyset(&accion.sa_mask);
    sigaddset(&accion.sa_mask, SIGUSR2);
    sigaddset(&accion.sa_mask, SIGUSR_ACTIVA);
    accion.sa_flags=0;
    sigaction(SIGUSR1, &accion, NULL);

    accion.sa_handler=man_int_preludio;
    sigemptyset(&accion.sa_mask);
    sigaddset(&accion.sa_mask, SIGIO);
    sigaddset(&accion.sa_mask, SIGUSR1);
    sigaddset(&accion.sa_mask, SIGUSR2);
    sigaddset(&accion.sa_mask, SIGALRM_ACTIVA);
    accion.sa_flags=SA_RESTART;
    sigaction(SIGALRM, &accion, NULL);

    accion.sa_handler=man_int_preludio;
    sigemptyset(&accion.sa_mask);
    sigaddset(&accion.sa_mask, SIGUSR1);
    sigaddset(&accion.sa_mask, SIGUSR2);
    sigaddset(&accion.sa_mask, SIGIO_ACTIVA);
    accion.sa_flags=SA_RESTART;
    sigaction(SIGIO, &accion, NULL);

    accion.sa_handler=man_exc_preludio;
    sigemptyset(&accion.sa_mask);
    accion.sa_flags=0;
    sigaddset(&accion.sa_mask, SIGUSR2);
    sigaction(SIGILL, &accion, NULL);
    sigaction(SIGBUS, &accion, NULL);
    sigaction(SIGFPE, &accion, NULL);
    sigaction(SIGSEGV, &accion, NULL);

    init_terminal(); // estado inicial del terminal
}

/* Lee un carácter del puerto especificado (por ahora, sólo el terminal) */
char read_port(int dir_puerto){
    char dato;

    if (dir_puerto==KEYBOARD_PORT)
        read(0, &dato, sizeof(dato));
    return dato;
}

/* Inicia el controlador de reloj fijando su frecuencia de interrupción */
void init_clock_controller(int ticks_por_seg){
        struct itimerval plazo;
        long us_por_tick;

        us_por_tick=1000000/ticks_por_seg;
        plazo.it_value.tv_sec= plazo.it_interval.tv_sec=us_por_tick/1000000;
        plazo.it_value.tv_usec=plazo.it_interval.tv_usec=us_por_tick%1000000;

        setitimer(ITIMER_REAL, &plazo , NULL);
}


/* Inicia el teclado poniéndolo en modo no bloqueante asíncrono */
void init_keyboard_controller(void) {
    int val;

    if (fcntl(0, F_SETOWN, getpid())<0) {
        perror("fcntl F_SETOWN");
        exit(1);
    }
    if ((val=fcntl(0, F_GETFL, 0))<0) {
        perror("fcntl F_GETFL");
        exit(1);
    }
    val |= O_ASYNC;
    if (fcntl(0, F_SETFL, val)<0) {
        perror("fcntl F_SETFL");
        exit(1);
    }
}

/* Operaciones sobre registros */
long read_register(int nreg) {
    if ((nreg<NR_REGS) && (nreg>=0)) return registros[nreg];
    else return -1; 
}
int write_register(int nreg, long valor) {
    if ((nreg<NR_REGS) && (nreg>=0)) {
        registros[nreg]=valor;
        return 0;
    } else  return -1; 
}
