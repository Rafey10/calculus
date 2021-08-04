#ifndef BASE64_H
#define BASE64_H

#include <stdlib.h>
#include <string.h>


/**
 *  \brief determines size of utf8 string from a b64 string
 * 
 *  \param b64str the base64 string to be decoded
 *  \return the calculated size of the output utf8 string, 0 on failure 
 */

size_t b64_sizeof_decode(const char* b64str);

/**
 *  \brief determines if a character is a valid b64 character
 * 
 *  \param c the input character
 *  \return 1 on success, 0 on failure 
 */

int is_valid_b64_char(char c);

/**
 *  \brief decodes to utf8 from base64
 * 
 *  \param b64str the base64 string to be decoded
 *  \param utfstr string to contain the decoded output
 *  \param utflen calculated length of the decoded string
 *  \return 1 on success, 0 on failure
 */

int base64_decode(const char *b64str, unsigned char* utfstr, size_t utflen);

#endif