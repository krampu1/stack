#include <stdio.h>
#include "stack/stack.h"

int main() {
    Stack stack = {};
    if (stack_push(&stack, 1) == 0) {
        printf("error");
    }
}