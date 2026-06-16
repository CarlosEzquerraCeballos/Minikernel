/*
 *  kernel/include/HAL.h
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Fichero de cabecera que contiene los prototipos de las funciones
 * proporcionadas por el "hardware virtual" (módulo HAL).
 */

#ifndef _HAL_H
#define _HAL_H

/* Registros generales del procesador */
#define NR_REGS 	8

/* Niveles de interrupción del procesador  */
#define NR_LEVELS	3
#define LEVEL_1 	1 /* Int. SW */
#define LEVEL_2 	2 /* Int. Terminal */
#define LEVEL_3 	3 /* Int. Reloj */

/* Número de vectores de interrupcion disponibles */
#define NR_VECTORS	6

/* Números de vector */
#define ARITM_EXC	0  // excepción aritmetica
#define MEM_EXC		1  // excepción en acceso a memoria
#define CLOCK_INT	2  // interrupción de reloj
#define KEYBOARD_INT	3  // interrupción de entrada de terminal
#define SYS_CALL 	4  // vector usado para llamadas
#define SW_INT		5  // vector usado para interrupciones software


#define KEYBOARD_PORT	1 // dirección de puerto de E/S del terminal

// estructura que corresponde al contexto de un procesos
#include <ucontext.h>
typedef struct context {
    ucontext_t ctxt;
    long registros[NR_REGS];
} context;

// Operaciones relacionadas con los dispositivos y las interrupciones.

void init_clock_controller(int ticks_per_sec); // iniciar controlador de reloj

void init_keyboard_controller(void); // iniciar controlador de teclado

void init_interrupt_controller(void); // iniciar controlador de interrupciones

void register_irq_handler(int vector, void (*handler)()); // instala manejador

// fija nivel de interrupción del procesador devolviendo el previo
int set_int_priority_level(int level);

// Devuelve verdadero si el modo previo de ejecución del procesador era usuario
int comes_from_usermode(void);

void activate_soft_int(void); // activa la interrupción software

/* Operación de salvaguarda y recuperación de contexto hardware del proceso.
 * Rutina que realiza el cambio de contexto.
 * Si (prev_context==NULL) no salva contexto, sólo restaura */
void context_switch(context *prev_context, context *new_context);

/* Operaciones relacionadas con mapa de memoria del proceso y pila */

/* crea mapa de memoria a partir de ejecutable "prog" devolviendo un
descriptor de dicho mapa y la dirección del punto de arranque del programa */
void * create_image(char *prog, void **initial_dir); 

void * create_stack(int size); // crea la pila del proceso

// crea el contexto inicial del proceso
void set_initial_context(void *mem, void *stack, int stack_size,
			void * initial_pc, context *initial_context);

void release_image(void *mem);		// libera el mapa de memoria

void release_stack(void *stack);	// libera la pila del proceso

#define malloc(s) kmalloc(s) // por si se prefiere usar el término malloc
void *kmalloc(int size); // reserva memoria dinámica dentro del kernel

#define free(p) kfree(p) // por si se prefiere usar el término free
void kfree(void *ptr); // libera memoria dinámica dentro del kernel

/* Operaciones misceláneas */

long read_register(int reg); // leer un registro del "procesador"

int write_register(int reg, long value); // escribe en un registro del "procesador"

char read_port(int dir_port); // lee un carácter del puerto especificado

void halt(void);  // Ejecuta una instrucción HALT para parar UCP

void panic(char *mess); // muestra mensaje y termina SO

void print_terminal(char *buffer, unsigned int longi); // escribe en pantalla

#define printf printk // por si se prefiere usar el término printf
int printk(const char *, ...); // escribe en pantalla con formato

#endif /* _HAL_H */
