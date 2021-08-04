#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pofix-stack.h"

#define STACK_INCREASE 2

struct _pofix_stack {
    void **content;
    int size;
    int top;
};

static void* xmalloc(size_t size) 
{
    void *p = malloc(size);
    if (! p) {
        fprintf(stderr, "fatal: memory exhausted (malloc of %zu bytes)\n", size);
        exit(1);
    }
    return p;
}

static void* xrealloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);
    if (! p) {
        fprintf(stderr, "fatal: memory exhausted (realloc of %zu bytes)\n", size);
        exit(1);
    }
    return p;
}

pofix_stack_t* pofix_stack_new()
{
    pofix_stack_t *s = xmalloc(sizeof(struct _pofix_stack));
    assert(s);
    s->content = NULL;
    s->size = 0;
    s->top = 0;
    return s;
}

void* pofix_stack_peek(pofix_stack_t *s)
{
    void *data = NULL;
    assert(s);
    if (s->top > 0) {
        data = s->content[s->top-1];
    }
    return data;
}

void pofix_stack_push(pofix_stack_t *s, void *data)
{
    assert(s && data);
    if (s->top == s->size) {
        s->size += STACK_INCREASE;
        s->content = xrealloc(s->content, s->size * sizeof(void*));
        assert(s->content);
    }
    s->content[s->top] = data;
    s->top++;
}

void* pofix_stack_pop(pofix_stack_t *s)
{
    void *data = NULL;
    assert(s);
    if (s->top > 0) {
        data = s->content[s->top-1];
        s->top--;
    }
    return data;
}

int pofix_stack_empty(pofix_stack_t *s)
{
    assert(s);
    return (s->top == 0);
}

void pofix_stack_del(pofix_stack_t *s)
{
    assert(s);
    if (s->content) free(s->content);
    free(s);
}