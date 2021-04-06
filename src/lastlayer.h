#ifndef LASTLAYER_H
#define LASTLAYER_H
#include "utils.h"
#include "cube.h"
#include <stdbool.h>


//Loads the OLLs from a file into memory
bool load_olls(char *filename);
//Loads the PLLs from a file into memory
bool load_plls(char *filename);
/*
Analyses the cube and finds the right OLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of y moves to do before
*/
pair_int_int find_oll(int cube[6][9]);
/*
Analyses the cube and finds the right PLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of y moves to do before
*/
pair_int_int find_pll(int cube[6][9]);
//Looks up the right OLL algorithm and runs it
char *solve_oll(int cube[6][9]);
//Looks up the right PLL algorithm and runs it
char *solve_pll(int cube[6][9]);
//frees the memory used to store OLLs and PLLs
void cleanup_last_layer();

#endif