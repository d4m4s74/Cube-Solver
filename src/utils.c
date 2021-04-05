#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



//Because % isn't really mod, a real mod function
int mod(int a, int b)
{
    return (a % b + b) % b;
}


//A function to make it easier to make a pair_int_int
pair_int_int make_pair_int_int(int first, int second)
{
    pair_int_int output;
    output.first = first;
    output.second = second;
    return output;
}

/*
A function that cycles four ints in place;
moves first to second, second to third, ... fourth to first
*/
void cycle(int *first, int *second, int *third, int *fourth)
{
    int temp = *first;
    *first = *fourth;
    *fourth = *third;
    *third = *second;
    *second = temp;
}

//Adds string b to the end of string a. Reallocationg more memory if necessary. Returns new pointer to a
char *append(char *a, char *b)
{
    //If A or B is null, free a and return NULL.
    //Usually I wouldn't free A if B is NULL,
    //but in this appplication if appending fails, the whole algorithm is invalid and the application should return nothing
    if (a == NULL || b == NULL)
    {
        free(a);
        return NULL;
    }
    //Attempt to realloc
    char *c = (char *) realloc(a, strlen(a) + strlen(b) + 1);
    //If it works, set the new location to A, and concatenate the strings
    if (c != NULL)
    {
        a = c;
        strncat(a, b, strlen(b) + 1);
        return a;
    }
    //If it fails, free A and return NULL for aforementioned reason
    else
    {
        free(a);
        return NULL;
    }
}

//checks if a string is a number
bool isNumber(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

//checks if a string only contains ones and zeroes
bool isBinary(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] != '0' && s[i] != '1')
        {
            return false;
        }
    }
    return true;
}