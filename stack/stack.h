#include <stdlib.h>
#include <stdio.h>

#undef DEFOLT_STACK_SIZE

#undef struct
#undef size_t
#undef nullptr
#undef data
#undef size
#undef capacity

enum Stack_defoult_values{
    DEFOLT_STACK_SIZE = 10,
    STACK_DEL_PTR     = 1337,
};

#define add_info_where_created(stack) stack.name = (char *)#stack;\
                                      stack.line = (size_t)__LINE__;\
                                      stack.file = (char *)__FILE__;\
                                      stack.func = (char *)__FUNCTION__;

#define stack_error(stack) stack->size > stack->capacity || stack->data == (Type_t *)STACK_DEL_PTR

#define stack_dump(stack) {FILE *ptr_log_file = fopen("log.out", "a");\
                           fprintf(ptr_log_file, "in %s() at %s(%ld):\n", __FUNCTION__, __FILE__, __LINE__);        \
                           fprintf(ptr_log_file, "  Stack(%p) \"%s\"\n", stack, #stack);                            \
                           fprintf(ptr_log_file, "  \"%s\" at %s() at %s\n", stack->name, stack->func, stack->file);\
                           fprintf(ptr_log_file, "  {\n    size = %ld\n", stack->size);                             \
                           fprintf(ptr_log_file, "    capacity = %ld\n",  stack->capacity);                         \
                           fprintf(ptr_log_file, "    data[%p]\n",        stack->data);                             \
                                                                                                                    \
                           for (size_t i = 0; i < stack->capacity; i++) {                                           \
                               if (i < stack->size) {                                                               \
                                   fprintf(ptr_log_file, "    *[%ld]=%d\n", i, stack->data[i]);                     \
                               }                                                                                    \
                               else {                                                                               \
                                   fprintf(ptr_log_file, "     [%ld]=%d\n", i, stack->data[i]);                     \
                               }                                                                                    \
                           }                                                                                        \
                           fprintf(ptr_log_file, "  }\n");                                                          \
                           fclose(ptr_log_file);                                                                    \
                          }

#define memory_alloc_error() {FILE *ptr_log_file = fopen("log.out", "a");                                                             \
                              fprintf(ptr_log_file, "memory allocation error in %s() at %s(%ld)\n", __FUNCTION__, __FILE__, __LINE__);\
                              fclose(ptr_log_file);                                                                                   \
                             }

#ifndef NDEBUG
#define stack_assert(stack) if (stack_error(stack)) stack_dump(stack)
#else
#define stack_assert(stack)
#endif

struct Stack{
    Type_t *data    = nullptr;
    size_t size     = 0;
    size_t capacity = 0;
    char *name      = nullptr;
    size_t line     = 0;
    char *file      = nullptr;
    char *func      = nullptr;
};

size_t stack_resize(Stack *stack) {
    stack_assert(stack);
    
    if (stack->capacity < DEFOLT_STACK_SIZE) {
        stack->data = (Type_t *)realloc(stack->data, DEFOLT_STACK_SIZE * sizeof(Type_t));
        
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

Type_t stack_pop(Stack* stack) {
    stack_assert(stack);

    if (stack->size == 0) {
        stack_dump(stack);

        return 0;
    }

    Type_t elem = stack->data[stack->size-- - 1];

    stack_resize(stack);

    stack_assert(stack);

    return elem;
}

void stack_del(Stack* stack) {
    stack_assert(stack);

    realloc(stack->data, 0);

    stack->data = (Type_t *)STACK_DEL_PTR;

    stack->size = 1;

    stack->capacity = 0;
}