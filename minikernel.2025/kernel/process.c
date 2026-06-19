/*
 *  kernel/process.c
 *
 *  Minikernel (versión 2.0)
 *
 *  Fernando Pérez Costoya
 *
 */

/* Fichero que contiene la funcionalidad de la gestión de procesos */

#include "HAL.h"
#include "process.h"
#include "sched.h"
#include "syscall.h"
#include "common.h"
#include "clock.h"

// Variable global que identifica el proceso actual
PCB * current = NULL;

// Variable global que representa la tabla de procesos 
static PCB proc_table[MAX_NR_PROC];

// Lista de procesos bloqueados durmiendo (proc_sleep) 
list sleep_list;

// Función que inicia la tabla de procesos 
static void init_process_table(void){
    for (int i=0; i<MAX_NR_PROC; i++) proc_table[i].state = FINISHED;
}

/* Función que busca una entrada libre en la tabla de procesos.
   Asigna PIDs en orden creciente reciclando cuando hay desbordamiento */
static int search_free_PCB(void){
    static int nproc = MAX_NR_PROC, next = 0;
    for (int i=next; nproc--; i = (i + 1) % MAX_NR_PROC)
        if (proc_table[i].state == FINISHED)  {
            next = (i + 1) % MAX_NR_PROC;
            return i;
        }
    return -1;
}

// el proceso actual no puede seguir ejecutando;
// hay que salvar su contexto y activar el proceso elegido por planificador;
// si parámetro es 0, no salva el contexto.
void pick_and_activate_next_task(int save_ctx) {
    PCB *prev = current;
    current=scheduler();
    current->state = RUNNING;
    if (prev != current) {
        if (prev) printk("-> CAMBIO DE CONTEXTO DE %d A %d\n",
                prev->pid, current->pid);
        context *ctx = (prev && save_ctx) ? &(prev->context) : NULL;
        context_switch(ctx, &(current->context));
    }
}

/* Implementación de la llamada al sistema create_process */
int do_create_process(char *prog, int prio){
    void * image, *initial_pc;
    int error=0;
    int nr_proc;
    PCB *p_new;

    if (check_syscall_arg_string_read(prog, MAX_EXEC_NAMELENGTH) == -1)
        return -1;

    if ((prio > MAX_PRIO) || (prio < MIN_PRIO)) return -1;

    image=create_image(prog, &initial_pc);
    if (image) {
        nr_proc=search_free_PCB();
        if (nr_proc==-1) return -1;    /* no hay entrada libre */
        /* A rellenar el PCB ... */
        p_new=&(proc_table[nr_proc]);
        p_new->mem=image;
        p_new->stack=create_stack(STACK_SIZE);
        set_initial_context(p_new->mem, p_new->stack, STACK_SIZE,
            initial_pc, &(p_new->context));
        p_new->pid=nr_proc;
        p_new->priority=prio;
        p_new->ticks_left=TICKS_POR_RODAJA; // rodaja completa al crearse

        printk("-> NUEVO PROCESO %d\n", p_new->pid);
        
        
        // lo inserta en la cola de listos 
        add_ready_queue(p_new);
        error= 0;
    }
    else
        error= -1; // fallo al crear imagen 
    return error;
}
/* Implementación de la llamada al sistema exit_process */
int do_exit_process(void){
    printk("-> TERMINA PROCESO %d\n", current->pid);
    release_image(current->mem);
    current->state=FINISHED;
    remove_ready_queue();
    release_stack(current->stack);
    pick_and_activate_next_task(0); // no salva estado del previo
    return 0; // no debería llegar aquí ya que el proceso terminó 
}
void init_process_module(void) {
    init_process_table();
    list_init(&sleep_list);
}

/* Implementación de la llamada al sistema get_pid */
int do_get_pid(void){
    return current->pid;
}
/* Implementación de la llamada al sistema get_priority */
int do_get_priority(void){
    return current->priority;
}

/* Implementación de la llamada al sistema proc_sleep: bloquea al proceso
   actual durante secs segundos, cediendo la CPU. */
int do_proc_sleep(unsigned int secs){
    int prev_level;

    current->ticks_to_sleep = secs * TICK; // segundos -> tics de reloj

    /* Sección crítica: la sleep_list y la cola de listos las manipula también
       el reloj (nivel 3). Se eleva el nivel para impedir que una interrupción
       de reloj se cuele mientras se bloquea el proceso. */
    prev_level = set_int_priority_level(LEVEL_3);

    remove_ready_queue();          // deja de estar listo
    current->state = BLOCKED;
    insert_last(&sleep_list, current); // pasa a la lista de dormidos

    /* Cede la CPU con el nivel todavía a 3. context_switch guarda el nivel
       actual en el contexto del proceso que se duerme y restaura el del
       entrante (cada proceso recupera SU nivel), así que no se "tapona" a
       nadie. Bajar el nivel antes de ceder dejaría una ventana en la que el
       reloj podría actuar sobre un estado a medio cambiar. */
    pick_and_activate_next_task(1);

    /* Se reanuda aquí al despertar (el contexto restauró nivel 3); se baja
       al nivel que tenía el proceso antes de dormirse. */
    set_int_priority_level(prev_level);

    return 0;
}

/* Recorre la lista de dormidos decrementando su cuenta de tics. Los que
   llegan a 0 se despiertan: se sacan de sleep_list y se devuelven a la cola
   de listos. Se invoca desde el manejador de reloj (ya en nivel 3), por lo
   que no necesita elevar el nivel de interrupción.
   El borrado durante la iteración es seguro: iterator_next() memoriza el
   elemento siguiente antes de que se modifique el actual al extraerlo. */
void update_sleeping_processes(void){
    iterator it;
    PCB *p;

    for (iterator_init(&sleep_list, &it); iterator_has_next(&it); ) {
        p = iterator_next(&it);
        p->ticks_to_sleep--;
        if (p->ticks_to_sleep <= 0) {
            remove_elem(&sleep_list, p);
            p->ticks_left = TICKS_POR_RODAJA; // al desbloquear, rodaja completa
            add_ready_queue(p); // dispara soft int si p es más prioritario
        }
    }
}