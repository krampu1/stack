#include <stdio.h>

#define Type_t int
#include "stack/stack.h"

int main() {
    Stack stack = {};
    add_info_where_created(stack);
    
    for (int i = 0; i < 20; i++) {
        stack_push(&stack, i);
    }
    
    printf("%d\n", stack_pop(&stack));

    for (int i = 0; i < 20 ; i++) {
        stack_push(&stack, i);
    }

    stack_dump((&stack));

    for (int i = 0; stack.size; i++) {
        printf("%d ", stack_pop(&stack));
    }

    stack_del(&stack);
    
    stack_pop(&stack);
}