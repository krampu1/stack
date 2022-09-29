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
    POISON_CHAR       = 228,
};

#define min(a, b) (a < b ? a : b)

#define stack_create(stack) Stack stack = {};                                                             \
                            stack.canary_left  = (unsigned int)2283001337;                                \
                            stack.data         = (Type_t *)recalloc((void *)4, DEFOLT_STACK_SIZE, sizeof(Type_t));\
                            stack.size         = 0;                                                       \
                            stack.capacity     = DEFOLT_STACK_SIZE;                                       \
                            stack.name         = (char *)#stack;                                          \
                            stack.line         = (size_t)__LINE__;                                        \
                            stack.file         = (char *)__FILE__;                                        \
                            stack.func         = (char *)__FUNCTION__;                                    \
                            stack.hash         = 0;                                                       \
                            stack.data_hash    = 0;                                                       \
                            stack.data_fprintf = (void (*)(FILE *, void *))defoult_data_fprintf;          \
                            stack.canary_right = (unsigned int)2283001337;

#define stack_error(stack) stack->size > stack->capacity || stack->data == (Type_t *)STACK_DEL_PTR || stack->capacity == 0 || \
                           stack->canary_left != 2283001337 || stack->canary_right != 2283001337 || \
                           *((unsigned int *)((char *)(stack->data) - 4)) != 2283001337 || \
                           *((unsigned int *)((char *)(stack->data) + stack->capacity * sizeof(Type_t))) != 2283001337

#define stack_dump(stack) {FILE *ptr_log_file = fopen("log.out", "a");                                              \
                           fprintf(ptr_log_file, "error in %s() at %s(%ld):\n", __FUNCTION__, __FILE__, __LINE__);  \
                           fprintf(ptr_log_file, "  Stack(%p) \"%s\"\n", stack, #stack);                            \
                           fprintf(ptr_log_file, "  \"%s\" at %s() at %s\n", stack->name, stack->func, stack->file);\
                           fprintf(ptr_log_file, "  {\n    size = %ld\n", stack->size);                             \
                           fprintf(ptr_log_file, "    capacity = %ld\n",  stack->capacity);                         \
                           fprintf(ptr_log_file, "    data[%p]\n",        stack->data);                             \
                                                                                                                    \
                           for (size_t i = 0; i < stack->capacity; i++) {                                           \
                               if (i < stack->size) {                                                               \
                                   fprintf(ptr_log_file, "    *[%ld]=", i);                                         \
                                   stack->data_fprintf(ptr_log_file, (void *) (stack->data + i));                   \
                                   fprintf(ptr_log_file, "\n");                                                     \
                               }                                                                                    \
                               else {                                                                               \
                                   if (*((char *)(stack->data + i)) == (char)POISON_CHAR) {                         \
                                       fprintf(ptr_log_file, "     [%ld]=", i);                                     \
                                       stack->data_fprintf(ptr_log_file, (void *) (stack->data + i));               \
                                       fprintf(ptr_log_file, "(poison)\n");                                         \
                                   }                                                                                \
                                   else {                                                                           \
                                       fprintf(ptr_log_file, "     [%ld]=", i);                                     \
                                       stack->data_fprintf(ptr_log_file, (void *) (stack->data + i));               \
                                       fprintf(ptr_log_file, "\n");                                                 \
                                   }                                                                                \
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

void *recalloc(void *data, size_t data_size, size_t elem_size) {
    static size_t alloc = 0;

    data = (void *)realloc((void *)((char *)data - 4), data_size * elem_size + 8);

    *((unsigned int *)data) = (unsigned int)2283001337;

    data = (void *)(((char *)data) + 4);

    *((unsigned int *)((char *)data + data_size * elem_size)) = (unsigned int)2283001337;

    for (int i = alloc; i < data_size * elem_size; i++) {
        *((char *)data + i) = (char)POISON_CHAR;
    }

    alloc = data_size * elem_size;

    return data;
}

void defoult_data_fprintf(FILE *file, void *data) {
    fprintf(file, "%d", data);
}

struct Stack{
    unsigned int canary_left;
    Type_t       *data;
    size_t       size;
    size_t       capacity;
    char         *name;
    size_t       line;
    char         *file;
    char         *func;
    void         (* data_fprintf)(FILE *, void *);
    long long    hash;
    long long    data_hash;
    unsigned int canary_right;
};

size_t stack_resize(Stack *stack) {
    stack_assert(stack);

    if (stack->capacity < DEFOLT_STACK_SIZE) {
        stack->data = (Type_t *)recalloc(stack->data, DEFOLT_STACK_SIZE ,sizeof(Type_t));
        
        if (stack->data == nullptr) {
             memory_alloc_error();
             return 0;
        }

        stack->capacity = DEFOLT_STACK_SIZE;
    }

    if (stack->size == stack->capacity) {
        stack->data = (Type_t *)recalloc(stack->data, stack->size * 2, sizeof(Type_t));

        if (stack->data == nullptr) {
             memory_alloc_error();
             return 0;
        }

        stack->capacity = stack->size * 2;
    }

    if (stack->size * 4 <= stack->capacity && stack->size * 2 >= DEFOLT_STACK_SIZE) {
        stack->data = (Type_t *)recalloc(stack->data, stack->size * 2, sizeof(Type_t));

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
    
    *((char *)(&(stack->data[stack->size]))) = POISON_CHAR;

    stack_resize(stack);

    stack_assert(stack);

    return elem;
}

void stack_del(Stack* stack) {
    stack_assert(stack);

    recalloc(stack->data, 0, sizeof(Type_t));

    stack->data = (Type_t *)STACK_DEL_PTR;

    stack->size = 1;

    stack->capacity = 0;
}