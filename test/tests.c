#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "b64decode.h"
#include "pofix-stack.h"
#include "pofix-calc.h"
#include "infix-calc.h"

START_TEST(test_b64decode_0)
{
    const char *enc = "MiAqICgyMy8oMyozKSktIDIzICogKDIqMyk=";
    const char *sol = "2 * (23/(3*3))- 23 * (2*3)";
    char* dec;
    size_t dec_len;

    dec_len = b64_sizeof_decode(enc) + 1;
    dec = malloc (dec_len);

    ck_assert_int_eq(base64_decode(enc, (unsigned char*) dec, dec_len), 1);
    ck_assert_str_eq(dec, sol);

    free (dec);
}
END_TEST

START_TEST(test_stack_0)
{
    pofix_stack_t *s;
    s = pofix_stack_new();
    ck_assert_ptr_ne(s, NULL);
    ck_assert_int_ne(pofix_stack_empty(s), 0);
    pofix_stack_del(s);
}
END_TEST

START_TEST(test_stack_1)
{
    pofix_stack_t *s;
    char *a = "foo";

    s = pofix_stack_new();
    ck_assert_ptr_ne(s, NULL);
    pofix_stack_push(s, a);
    ck_assert_int_eq(pofix_stack_empty(s), 0);

    for (int i = 0; i < 3; i++) {
        char *p = pofix_stack_peek(s);
        ck_assert_str_eq(a, p);
    }

    char *p = pofix_stack_pop(s);
    ck_assert_str_eq(a, p);
    ck_assert_int_ne(pofix_stack_empty(s), 0);

    pofix_stack_del(s);
}
END_TEST

START_TEST(test_stack_2)
{
    char *a[] = { "nice", "hello", "world", "out", "there", NULL };
    int i;
    pofix_stack_t *s;

    s = pofix_stack_new();
    ck_assert_ptr_ne(s, NULL);

    for (i = 0; a[i]; i++) {
        pofix_stack_push(s, a[i]);
        ck_assert_int_eq(pofix_stack_empty(s), 0);
    }

    for (i--; i >= 0; i--) {
        char *p = pofix_stack_pop(s);
        ck_assert_str_eq(a[i], p);
    }

    ck_assert_int_ne(pofix_stack_empty(s), 0);

    pofix_stack_del(s);
}
END_TEST

START_TEST(test_stack_3)
{
    char *a = "bar";
    int i;
    pofix_stack_t *s;

    s = pofix_stack_new();
    ck_assert_ptr_ne(s, NULL);

    for (i = 0; i < 10000; i++) {
        pofix_stack_push(s, a);
        ck_assert_int_eq(pofix_stack_empty(s), 0);
    }

    for (i--; i >= 0; i--) {
        char *p = pofix_stack_pop(s);
        ck_assert_str_eq(a, p);
    }

    ck_assert_int_ne(pofix_stack_empty(s), 0);
    pofix_stack_del(s);
}
END_TEST

START_TEST(test_stack_4)
{
    char *p;
    pofix_stack_t *s;
    s = pofix_stack_new();
    ck_assert_ptr_ne(s, NULL);
    p = pofix_stack_peek(s);
    ck_assert_ptr_eq(p, NULL);
    p = pofix_stack_pop(s);
    ck_assert_ptr_eq(p, NULL);
    ck_assert_int_ne(pofix_stack_empty(s), 0);
    pofix_stack_del(s);
}
END_TEST

typedef struct {
    int rc;
    char *result;
    char **token;
} token_test_t;

typedef struct {
    int rc;
    char *result;
    char *expr;
} expr_test_t;

START_TEST(test_pofix_eval_token_0)
{
    char *result;
    int rc;

    static char *t1[] = {"3", "4", "+", "4", "*", "2", "+", NULL};
    static char *t2[] = {"foo", NULL};
    static char *t3[] = {"+", NULL};
    static char *t4[] = {"1", "2", NULL};
    static char *t5[] = {"1", "0", "/", NULL};
    static char *t6[] = {"1", "0", "%", NULL};

    token_test_t tests[] = {
        {POFIX_OK, "30", t1},
        {POFIX_INVALID_TOKEN, NULL, t2},
        {POFIX_MISSING_OPERAND, NULL, t3},
        {POFIX_MISSING_OPERATOR, NULL, t4},
        {POFIX_ARITHMETIC_ERROR, NULL, t5},
        {POFIX_ARITHMETIC_ERROR, NULL, t6},
    };

    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        rc = pofix_eval_token(tests[i].token, &result);
        ck_assert_int_eq(rc, tests[i].rc);
        if (tests[i].result) {
           ck_assert_str_eq(result, tests[i].result);
        }
    }
}
END_TEST

START_TEST(test_pofix_eval_expr_0)
{
    char *result;
    int rc;
    
    expr_test_t tests[] = {
        {POFIX_OK, NULL, ""},
        {POFIX_OK, "3", "3"},
        {POFIX_OK, "8", "3 5 +", },
        {POFIX_OK, "20", "3 5 + 2 * 4 +"},

        {POFIX_MISSING_OPERAND, NULL, "3 +"},
        {POFIX_MISSING_OPERAND, NULL, "3 5 + +"},
        {POFIX_MISSING_OPERATOR, NULL, "3 5"},
        {POFIX_MISSING_OPERATOR, NULL, "3 5 + 3"},
        {POFIX_INVALID_TOKEN, NULL, "3 5 &"},
        {POFIX_ARITHMETIC_ERROR, NULL, "3 0 /"},
        {POFIX_ARITHMETIC_ERROR, NULL, "5 0 %"},
    }; 

    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        rc = pofix_eval_expr(tests[i].expr, &result);
        ck_assert_int_eq(rc, tests[i].rc);
        if (tests[i].result) {
            ck_assert_str_eq(result, tests[i].result);
        }
    }
}
END_TEST

START_TEST(test_infix_eval_token_0)
{
    char *result;
    int rc;

    static char *t1[] = {"(", "3", "+", "4", ")", "*", "2", NULL};
    static char *t2[] = {"foo", NULL};
    static char *t3[] = {"+", NULL};
    static char *t4[] = {"1", "2", NULL};
    static char *t5[] = {"1", "/", "0", NULL};
    static char *t6[] = {"1", "%", "0", NULL};

    token_test_t tests[] = {
        {INFIX_OK, "14", t1},
        {INFIX_INVALID_TOKEN, NULL, t2},
        {INFIX_MISSING_OPERAND, NULL, t3},
        {INFIX_MISSING_OPERATOR, NULL, t4},
        {INFIX_ARITHMETIC_ERROR, NULL, t5},
        {INFIX_ARITHMETIC_ERROR, NULL, t6},
    };

    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        rc = infix_eval_token(tests[i].token, &result);
        ck_assert_int_eq(rc, tests[i].rc);
        if (tests[i].result) {
            ck_assert_str_eq(result, tests[i].result);
        }
        if (result) free(result);
    }
}
END_TEST

START_TEST(test_infix_eval_expr_0)
{
    char *result;
    int rc;
    
    expr_test_t tests[] = {
        {INFIX_OK, NULL, ""},
        {INFIX_OK, "3", "3"},
        {INFIX_OK, "8", "3 + 5", },
        {INFIX_OK, "20", "( 3 + 5 ) * 2 + 4"},
        {INFIX_OK, "15", "1 + 2 + ( 3 + 4 + 5 )"},
        {INFIX_OK, "-134", "2 * ( 23 / ( 3 * 3 ) ) - 23 * ( 2 * 3 )"},

        {INFIX_MISSING_OPERAND, NULL, "3 +"},
        {INFIX_MISSING_OPERAND, NULL, "3 + 5 +"},
        {INFIX_MISSING_OPERATOR, NULL, "3 5 +"},
        {INFIX_MISSING_OPERATOR, NULL, "3 + 5 3"},
        {INFIX_INVALID_TOKEN, NULL, "3 + &"},
        {INFIX_ARITHMETIC_ERROR, NULL, "3 / 0"},
        {INFIX_ARITHMETIC_ERROR, NULL, "5 % 0"},
        {INFIX_MISSING_CLOSING_PAREN, NULL, "( 2 + 0"},
        {INFIX_MISSING_OPENING_PAREN, NULL, "2 + 0 )"},
    };

    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        rc = infix_eval_expr(tests[i].expr, &result);
        ck_assert_int_eq(rc, tests[i].rc);
        if (tests[i].result) {
            ck_assert_str_eq(result, tests[i].result);
        }
        if (result) free(result);
    }
}
END_TEST

static Suite*
test_suite(void)
{
    Suite *s;
    TCase *tc;
    s = suite_create("all");
    tc = tcase_create("test");
    tcase_add_test(tc, test_b64decode_0);
    tcase_add_test(tc, test_stack_0);
    tcase_add_test(tc, test_stack_1);
    tcase_add_test(tc, test_stack_2);
    tcase_add_test(tc, test_stack_3);
    tcase_add_test(tc, test_stack_4);
    tcase_add_test(tc, test_pofix_eval_token_0);
    tcase_add_test(tc, test_pofix_eval_expr_0);
    tcase_add_test(tc, test_infix_eval_token_0);
    tcase_add_test(tc, test_infix_eval_expr_0);
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = test_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}