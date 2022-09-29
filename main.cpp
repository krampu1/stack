#include <stdio.h>

#define Type_t int
#include "stack/stack.h"

int main() {
    stack_create(stack);

    //stack_dump((&stack));
    
    for (int i = 0; i < 20; i++) {
        stack_push(&stack, i);
    }
    
    printf("%d\n", stack_pop(&stack));

    //stack_dump((&stack));

    for (int i = 0; i < 15 ; i++) {
        stack_push(&stack, i);
    }

    for (int i = 0; stack.size; i++) {
        printf("%d %d\n", stack_pop(&stack), stack.capacity);
    }

    stack_del(&stack);

    //stack_dump((&stack));
}