#ifndef _INFIX_H
#define _INFIX_H

/** successful evaluation */
#define INFIX_OK                   0
/** failed due to invalid token */
#define INFIX_INVALID_TOKEN       -1
/** failed due to missing operand. */
#define INFIX_MISSING_OPERAND     -2
/** failed due to missing operator. */
#define INFIX_MISSING_OPERATOR    -3
/** failed due to arithmetic error. */
#define INFIX_ARITHMETIC_ERROR    -4
/** failed due to missing closing parenthesis. */
#define INFIX_MISSING_CLOSING_PAREN    -5
/** failed due to missing closing parenthesis. */
#define INFIX_MISSING_OPENING_PAREN    -6

/**
 * \brief Return an error message for the input error number
 *
 * \param errnum The error number
 * \result A string describing the error
 */

char* infix_strerror(int errnum);

/**
 * \brief Evaluates an infix expression.
 *
 * The method will take a null terminated array of strings, containing
 * tokens of expressions in infix notation. It will output the result
 * as a string via the second argument to the method.
 *
 *\param token Null-terminated array, containing tokens of the expression.
 *\param result Malloced ptr to string containing the result.
 *              Must be freed by the caller.
 * \result An error code from the ones defined above, may return NULL if 
 *         nothing was calculated.
 */

int infix_eval_token(char *token[], char **result);

/**
 * \brief Evaluates an infix expression.
 *
 * This method will split the input expression into a null terminated array 
 * of strings to contain the tokens of the infix expression. It will output 
 * the result of the calculation as a string via the second argument to the 
 * method. 
 *
 * \param expr The input expression.
 * \param result Malloced ptr to a string holding the result of the calculation
 *               must be freed by caller.
 * \result An error code from the ones defined above, may return NULL if nothing 
 *         was calculated.
 */ 

int infix_eval_expr(const char *expr, char **result);

#endif