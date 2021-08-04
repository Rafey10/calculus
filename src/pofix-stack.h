#ifndef _POFIX_STACK_H
#define _POFIX_STACK_H

typedef struct _pofix_stack pofix_stack_t;

/**
 * \brief Creates a new empty stack
 *
 * \return A ptr acting as handle for the created stack
 */

pofix_stack_t* pofix_stack_new();

/**
 * \brief Peeks the top of the stack
 *
 * \param s The stack to be peeked
 * \return The peek value or NULL if stack is empty
 */

void* pofix_stack_peek(pofix_stack_t *s);

/**
 * \brief Push data on the stack. Data is caller allocated.
 *
 * \param s The Stack to push data on
 * \param data Ptr to data to be pushed
 */

void pofix_stack_push(pofix_stack_t *s, void *data);

/**
 * \brief Pop data from stack
 *
 * \param s The stack to pop from
 * \return The pop value or NULL if stack is empty.
 */

void* pofix_stack_pop(pofix_stack_t *s);

/**
 * \brief Determine if stack is empty
 *
 * \param s The stack to empty 
 * \return non-zero value if empty, 0 otherwise.
 */

int pofix_stack_empty(pofix_stack_t *s);

/**
 * \brief Delete stack.
 *
 * \param s The stack to delete
 */

void pofix_stack_del(pofix_stack_t *s);

#endif