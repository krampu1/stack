#include <stdio.h>

void int_fprintf(FILE *file, void *data) {
    fprintf(file, "%d", *(int *)data);
}

typedef int Type_t;
#define CANARY_PROT
#define HASH_PROT
#include "stack/stack.h"

int main() {
    //setvbuf
    Stack stack = {};
    stack_create(&stack);
    
    stack_change_out_funk(&stack, (void (*)(FILE *, void *))int_fprintf);

    //stack_dump((&stack));
    
    for (int i = 0; i < 10; i++) {
        stack_push(&stack, i);
    }

    printf("%d\n", stack_pop(&stack));

    //stack_dump((&stack));

    for (int i = 0; i < 15; i++) {
        stack_push(&stack, i);
    }

    for (int i = 0; stack.size; i++) {
        printf("%d %d\n", stack_pop(&stack), stack.capacity);
    }

    stack_del(&stack);

    //stack_dump((&stack));
}