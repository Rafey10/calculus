#include "b64decode.h"

const char table[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_decode_table[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 };

/* *
 * Code which generated the golden values above
 * */
/*
    int* generate_b64_decode_table(){
        int dec_table[80];
        size_t i;
        memset(dec_table, -1, sizeof(dec_table));
        for (i = 0; i < sizeof(table) - 1; i++){
            dec_table[table[i]-43] = i;
        }
    return dec_table;
    }
*/

size_t b64_sizeof_decode(const char* b64str)
{
    size_t length, retval, i;

    if (b64str == NULL){
        return 0;
    }

    length = strlen(b64str);
    retval = length / 4 * 3;

    for (i = length; i-->0; ){
        if(b64str[i] == '='){
            retval--;
        } else {
            break;
        }
    }
return retval;
}

int is_valid_b64_char(char c)
{
    if (c >= 'A' && c <= 'Z'){
        return 1;
    } else if (c >= 'a' && c <= 'z'){
        return 1;
    } else if (c >= '0' && c <= '9'){
        return 1;
    } else if (c == '+' || c == '/' || c == '='){
        return 1;
    }
return 0;
}

int base64_decode(const char* b64str, unsigned char* utfstr, size_t utflen)
{
    size_t length, i, j;
    int piece;

    if (b64str == NULL || utfstr == NULL)
        return 0;

    length = strlen(b64str);

    if (length % 4 != 0 || utflen < b64_sizeof_decode(b64str))
        return 0;

    for (i = 0; i < length; i++){
        if (is_valid_b64_char(b64str[i]) == 0)
            return 0;
    } 

    for (j = 0, i = 0; i < length; i+=4, j+=3){

        piece = b64_decode_table[b64str[i] - 43];
        piece = (piece << 6) | b64_decode_table[b64str[i+1] - 43];
        
        if (b64str[i+2] == '=')
            piece = piece << 6;
        else
            piece = (piece << 6) | b64_decode_table[b64str[i+2] - 43];
        

        if (b64str[i+3] == '=')
            piece = piece << 6;
        else
            piece = (piece << 6) | b64_decode_table[b64str[i+3] - 43];
        

        utfstr[j] = (piece >> 16) & 0xFF;

        if (b64str[i+2] != '=')
            utfstr[j+1] = (piece >> 8) & 0xFF;
        if (b64str[i+3] != '=')
            utfstr[j+2] = piece & 0xFF;
    }
    
return 1;   
}
