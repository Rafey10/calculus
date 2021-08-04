#define _POSIX_C_SOURCE 201112L

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "pofix-stack.h"
#include "infix-calc.h"
#include "pofix-calc.h"


char* infix_strerror(int errnum)
{
    switch (errnum) {
    case INFIX_OK:
        return "no error";
    case INFIX_INVALID_TOKEN:
        return "invalid token";
    case INFIX_MISSING_OPERAND:
        return "missing operand";
    case INFIX_MISSING_OPERATOR:
        return "missing operator";
    case INFIX_ARITHMETIC_ERROR:
        return "arithmetic error";
    case INFIX_MISSING_CLOSING_PAREN:
        return "missing closing parenthesis";
    case INFIX_MISSING_OPENING_PAREN:
        return "missing opening parenthesis";
    default:
        return "unknown error";
    }
}

static int precedence(const char *op)
{
    static struct {
        int prec;
        char *op;
    } precs[] = {
        { .prec = 3, .op = "*" },
        { .prec = 3, .op = "/" },
        { .prec = 3, .op = "%" },
        { .prec = 2, .op = "+" },
        { .prec = 2, .op = "-" },
        { .prec = 1, .op = "(" },
        { .prec = 1, .op = ")" },
        { .prec = 0, .op = NULL },
    };
    
    int i;

    for (i = 0; precs[i].op; i++) {
        if (strcmp(op, precs[i].op) == 0) {
            return precs[i].prec;
        }
    }
    return 0;
}

static void* xmalloc(size_t size)
{
    void *p = malloc(size);
    if (! p) {
        fprintf(stderr, "fatal: memory exhausted (malloc of %zu bytes)\n", size);
        exit(1);
    }
return p;
}
    
int infix_eval_token(char *token[], char **result)
{
    pofix_stack_t *s;
    int i, j, cnt;
    char **potoken;
    char *op;
    int last = -1;
    int rc = INFIX_OK;

    assert(token && result);
    *result = NULL;

    for (cnt = 0; token[cnt]; cnt++) ;
    potoken = xmalloc(sizeof(potoken[0]) * (cnt+1));
    memset(potoken, 0, sizeof(potoken[0]) * (cnt+1));
    s = pofix_stack_new();

    for (i = 0, j = 0; token[i]; i++) {
        int p = precedence(token[i]);
        if (strcmp(token[i], "(") == 0) {
            pofix_stack_push(s, token[i]);
        } else if (strcmp(token[i], ")") == 0) {
            while ((op = pofix_stack_peek(s)) && (strcmp(op, "(") != 0)) {
                if ((op = pofix_stack_pop(s))) {
                    potoken[j++] = op;
                } else {
                    rc = INFIX_MISSING_OPENING_PAREN;
                    goto cleanup;
                }
            }

            if (pofix_stack_pop(s) == NULL) {
                rc = INFIX_MISSING_OPENING_PAREN;
                goto cleanup;
            }
        } else if (p > 1) {     /* short for 'its an operator' */
            if (last > 1 && p > 1) {
                rc = INFIX_MISSING_OPERAND;
                goto cleanup;
            }

            while ((op = pofix_stack_peek(s))) {
                if (precedence(op) < p) {
                    break;
                }
                op = pofix_stack_pop(s);
                if (op) {
                    potoken[j++] = op;
                }
            }
            pofix_stack_push(s, token[i]);
        } else {
            if (last == p) {
                rc = INFIX_MISSING_OPERATOR;
                goto cleanup;
            }
            potoken[j++] = token[i];
        }
        last = p;
    }

    while ((op = pofix_stack_pop(s))) {
        if (strcmp(op, "(") == 0) {
            rc = INFIX_MISSING_CLOSING_PAREN;
            goto cleanup;
        }
        potoken[j++] = op;
    }

    potoken[j] = NULL;
    rc = pofix_eval_token(potoken, result);

cleanup:
    if (! pofix_stack_empty(s)) {
        while (pofix_stack_pop(s)) ;
    }
    pofix_stack_del(s);

    if (potoken) {
        free(potoken);
    }
return rc;
}

int infix_eval_expr(const char *expr, char **result)
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
        return INFIX_OK;
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

    rc = infix_eval_token(token, result);
    
    free(token);
    free(line);
    return rc;
}