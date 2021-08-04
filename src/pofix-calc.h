#ifndef _POFIX_CALC_H
#define _POFIX_CALC_H

/** successful evaluation */
#define POFIX_OK                  0
/** failed due to invalid token */
#define POFIX_INVALID_TOKEN       -1
/** failed due to missing operand. */
#define POFIX_MISSING_OPERAND     -2
/** failed due to missing operator. */
#define POFIX_MISSING_OPERATOR    -3
/** failed due to arithmetic error. */
#define POFIX_ARITHMETIC_ERROR    -4

/**
 * \brief Return an error message for the input error number
 *
 * \param errnum The error number
 * \result A string describing the error
 */

char* pofix_strerror(int errnum);

/**
 * \brief Evaluates a poftfix expression
 *
 * The method will take a null terminated array of strings, containing
 * tokens of expressions in postfix notation. It will output the result
 * as a string via the second argument to the method.
 *
 * \param token Null-terminated array, containing tokens of the expression
 * \param result malloced ptr to string containing the result.
 *               Must be freed by caller
 * \result an error code from the ones defined above, may return NULL if nothing
 *         was calculated
 */

int pofix_eval_token(char *token[], char **result);

/**
 * \brief evaluates a postfix expression
 *
 * This method will split the input expression into a null terminated array 
 * of strings to contain the tokens of the postfix expression. It will output 
 * the result of the calculation as a string via the second argument to the 
 * method. 
 * 
 * \param expr the input expression
 * \param result malloced ptr to a string holding the result of the calculation
 *               must be freed by caller
 * \result an error code from the ones defined above, may return NULL if nothing
 *         was calculated
 */ 

int pofix_eval_expr(const char *expr, char **result);

#endif