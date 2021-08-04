#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "pofix-calc.h"
#include "pofix-stack.h"

char* pofix_strerror(int errnum)
{
    switch (errnum) {
    case POFIX_OK:
        return "no error";
    case POFIX_INVALID_TOKEN:
        return "invalid token";
    case POFIX_MISSING_OPERAND:
        return "missing operand";
    case POFIX_MISSING_OPERATOR:
        return "missing operator";
    case POFIX_ARITHMETIC_ERROR:
        return "arithmetic error";
    default:
        return "unknown error";
    }
}

typedef struct {
    char *token;
    int (*func)(long l1, long l2, long *r);
} ops_t;

static int pofix_mul(long l1, long l2, long *r)
{
    assert(r);
    *r = l1 * l2;
    return 0;
}

static int pofix_div(long l1, long l2, long *r)
{
    assert(r);
    if (l2 == 0) return -1;
    *r = l1 / l2;
    return 0;
}

static int pofix_mod(long l1, long l2, long *r)
{
    assert(r);
    if (l2 == 0) return -1;
    *r = l1 % l2;
    return 0;
}

static int pofix_add(long l1, long l2, long *r)
{
    assert(r);
    *r = l1 + l2;
    return 0;
}

static int pofix_sub(long l1, long l2, long *r)
{
    assert(r);
    *r = l1 - l2;
    return 0;
}

ops_t ops [] = {
    { "*", pofix_mul },
    { "/", pofix_div },
    { "%", pofix_mod },
    { "+", pofix_add },
    { "-", pofix_sub },
    {NULL, NULL},
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

int pofix_eval_token(char *token[], char **result)
{
    int i, j;
    pofix_stack_t *s;
    long *p;
    int rc = POFIX_OK;

    assert(token && result);
    *result = NULL;

    s = pofix_stack_new();

    for (i = 0; token[i]; i++) {
        for (j = 0; ops[j].token; j++) {
            if (strcmp(token[i], ops[j].token) == 0) {
                long *a2 = pofix_stack_pop(s);
                long *a1 = pofix_stack_pop(s);
                if (!a1 || !a2) {
                    rc = POFIX_MISSING_OPERAND;
                    if (a1) free(a1);
                    if (a2) free(a2);
                    goto cleanup;
                }
                if (ops[j].func(*a1, *a2, a1)) {
                    rc = POFIX_ARITHMETIC_ERROR;
                    if (a1) free(a1);
                    if (a2) free(a2);
                    goto cleanup;
                }
                free(a2); pofix_stack_push(s, a1);
                break;
            }
        }
        if (! ops[j].token) {
            char *end;
            long l = strtol(token[i], &end, 10);
            if (*end != 0 || strlen(token[i]) == 0) {
                rc = POFIX_INVALID_TOKEN;
                *result = strdup(token[i]);
                goto cleanup;
            }
            p = xmalloc(sizeof(long));
            *p = l;
            pofix_stack_push(s, p);
        }
    }

    p = pofix_stack_pop(s);
    if (! pofix_stack_empty(s)) {
        rc = POFIX_MISSING_OPERATOR;
        free(p);
        goto cleanup;
    }

    if (p) {
        asprintf(result, "%ld", *p);
        free(p);
    }

cleanup:
    if (! pofix_stack_empty(s)) {
        while ((p = pofix_stack_pop(s))) {
            free(p);
        }
    }
    pofix_stack_del(s);

    return rc;
}

int pofix_eval_expr(const char *expr, char **result)
{
    char *line;
    char **token;
    unsigned cnt = 2;
    unsigned int n = 0;
    int i, rc;
    size_t len;

    assert(expr && result);
    *result = NULL;
    len = strlen(expr);
    if (len == 0) {
        return POFIX_OK;
    }

    line = xmalloc(len+1);
    memcpy(line, expr, len+1);
    for (i = 0; line[i]; i++) {
        cnt += (line[i] == ' ');
    }

    token = xmalloc(sizeof(token[0]) * cnt);
    memset(token, 0, sizeof(token[0]) * cnt);

    token[n] = strtok(line, " ");
    while (token[n]) {
        token[++n] = strtok(NULL, " ");
    }

    rc = pofix_eval_token(token, result);

    free(token);
    free(line);
    return rc;
}