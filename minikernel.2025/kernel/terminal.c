/*
 * kernel/terminal.c
 *
 * Minikernel (versión 2.0)
 *
 * Fernando Pérez Costoya
 *
 */

#include "HAL.h"
#include "terminal.h"
#include "syscall.h"
#include "common.h"
#include "process.h"
#include "sched.h"
#include "list.h"
#include "fifo.h"

// Variables estáticas para el terminal
static fifo terminal_fifo;
static char term_buffer[TERM_BUF_SIZE];
static list terminal_blocked_list;

int do_print(char * buf, int size) {
    if (size <= 0) return -1;
    if (check_syscall_arg_pointer_read(buf, size) == -1)
        return -1;
    print_terminal(buf, size);
    return 0;
}

int do_get_char(void) {
    int prev_level;
    char c;

    // Protegemos la sección crítica con el nivel del terminal
    prev_level = set_int_priority_level(LEVEL_2);

    // Re-validación con while ante robos de interrupción
    while (fifo_is_empty(&terminal_fifo)) {
        current->state = BLOCKED;
        insert_last(&terminal_blocked_list, current);
        remove_ready_queue();
        pick_and_activate_next_task(1);
    }

    fifo_out(&terminal_fifo, &c);

    set_int_priority_level(prev_level);
    return (int)c;
}

static void keyboard_handler(void) {
    char c;
    
    // 1. Lee del puerto
    c = read_port(KEYBOARD_PORT);
    
    // 2. Eco del carácter recibido
    print_terminal(&c, 1);

    // 3. Productor: si hay hueco en el buffer
    if (!fifo_is_full(&terminal_fifo)) {
        fifo_in(&terminal_fifo, &c);

        // Si hay procesos esperando, despertamos al de mayor prioridad
        if (!list_is_empty(&terminal_blocked_list)) {
            iterator it;
            PCB *p, *best = NULL;

            for (iterator_init(&terminal_blocked_list, &it); iterator_has_next(&it); ) {
                p = iterator_next(&it);
                if (best == NULL || p->priority > best->priority) {
                    best = p;
                }
            }
            
            remove_elem(&terminal_blocked_list, best);
            add_ready_queue(best);
        }
    }
}

void init_terminal_module(void) {
    list_init(&terminal_blocked_list);
    fifo_init(&terminal_fifo, sizeof(char), TERM_BUF_SIZE, term_buffer);
    
    register_irq_handler(KEYBOARD_INT, keyboard_handler);
    init_keyboard_controller();
}
