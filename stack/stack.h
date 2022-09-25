#include <stdlib.h>
#include <stdio.h>

#undef DEFOLT_STACK_SIZE
#define DEFOLT_STACK_SIZE 10

#undef struct
#undef size_t
#undef nullptr
#undef data
#undef size
#undef capacity

#ifndef Type_t
#define Type_t int
#endif

///
#define stack_error(stack) true
#define stack_dump(stack) printf("assert\n")
#define memory_alloc_error()
///


#ifndef NDEBUG
#define stack_assert(stack) if (stack_error(stack)) stack_dump(stack)
#else
#define stack_assert(stack)
#endif

struct Stack{
    Type_t *data    = nullptr;
    size_t size     = 0;
    size_t capacity = 0;
};

size_t stack_resize(Stack *stack) {
    stack_assert(stack);

    if (stack->capacity < DEFOLT_STACK_SIZE) {
        stack->data = (Type_t *)realloc(stack->data, 10 * sizeof(Type_t));

        if (stack->data == nullptr) {
             memory_alloc_error();
             return 0;
        }

        stack->capacity = DEFOLT_STACK_SIZE;
    }

    if (stack->size == stack->capacity) {
        stack->data = (Type_t *)realloc(stack->data, stack->size * 2 * sizeof(Type_t));

        if (stack->data == nullptr) {
             memory_alloc_error();
             return 0;
        }

        stack->capacity = stack->size * 2;
    }

    if (stack->size * 4 < stack->capacity && stack->capacity > DEFOLT_STACK_SIZE) {
        stack->data = (Type_t *)realloc(stack->data, stack->size * 2 * sizeof(Type_t));

        if (stack->data == nullptr) {
             memory_alloc_error();
             return 0;
        }

        stack->capacity = stack->size * 2;
    }

    stack_assert(stack);

    return stack->capacity;
}

size_t stack_push(Stack* stack, Type_t a) {
    stack_assert(stack);

    if (stack_resize(stack) == 0) {
        return 0;
    }

    *(stack->data+(stack->size++)) = a;

    stack_assert(stack);

    return stack->size;
}