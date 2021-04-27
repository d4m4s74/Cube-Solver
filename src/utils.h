#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>


//A struct used to return two variables from one function
typedef struct
{
    int first;
    int second;
} pair_int_int;

//Because % isn't really mod, a real mod function
int mod(int a, int b);
//A function to make it easier to make a pair_int_int
pair_int_int make_pair_int_int(int first, int second);
/*
A function that cycles four ints in place;
moves first to second, second to third, ... fourth to first
*/
void cycle(int *first, int *second, int *third, int *fourth);
//Adds string b to the end of string a. Reallocationg more memory if necessary. Returns new pointer to a
char *append(char *a, char *b);
//checks if a string is a number
bool isNumber(char *s);
//checks if a string only contains ones and zeroes
bool isBinary(char *s);
#endif  