#ifndef SOLVER_LIBRARY_H
#define SOLVER_LIBRARY_H

/**
 * Rubik's Cube Solver Library for C
 * https://github.com/d4m4s74/Cube-Solver
 *
 * Created as my CS50 final project 
 *
 * Copyright (c) 2021
 * All rights reserved
 *
 * BSD 3-Clause License
 * http://www.opensource.org/licenses/BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of CS50 nor the names of its contributors may be used
 *   to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>

//A struct used to return two ints from one function
typedef struct
{
    int first;
    int second;
} pair_int_int;

//A function to make it easier to make a pair_int_int
pair_int_int make_pair_int_int(int first, int second);

/*
Load all OLLs and PLLs into memory
path is the folder in which the csv files are located
*/
bool setup(char *path);

//Loads the OLLs from a file into memory
bool load_olls(char *filename);

//Loads the PLLs from a file into memory
bool load_plls(char *filename);

//A function that adds the colors from four strings onto the cube.
void color_cube(int cube[6][9], char *f, char *r, char *b, char *l, char *u, char *d);

/*
A function that moves the virtual cube
Each face moves clockwise, assuming you're facing the face.
int move: the move done
0 for F
1 for R
2 for B
3 for L
4 for U
5 for D
6 for M
7 for E
8 for S
int times: the amount of times the same move is repeated.
*/
void move_cube(int cube[6][9], int move, int times);

//Runs a multiple move algorithm on the cube, using standard cube notation
void run_algorithm(int cube[6][9], char *algorithm);

//returns string containing all algorithms used to solve the cube, separated by newlines and the names of the steps (eg. Cross, F2L, OLL: Sune, PLL: Y perm)
char *solve(int cube[6][9]);

//Generates a cube from an algorithm and returns its solution
char *solve_scramble(char *scramble);
/*
A function that prints the sides of the cube in an exploded fashion
    444
    444
    444

333 000 111 222
333 000 111 222
333 000 111 222

    555
    555
    555
*/
void print_cube(int cube[6][9]);

//Validate the colors on the cube for impossible cubies. This does not check if the scramble is solvable.
bool validate(int cube[6][9]);

/*
A function to solve the (yellow) cross.
Returns a string containing the solve algorithm, with each solved edge separated by newlines
*/
char *solve_cross(int cube[6][9]);

/*
A function to solve the first two layers of the cube
Returns a string containing the solve algorithm, with each solved pair separated by newlines
*/
char *solve_f2l(int cube[6][9]);

/*
Analyses the cube and finds the right OLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of U moves to do before
*/
pair_int_int find_oll(int cube[6][9]);

/*
Analyses the cube and finds the right PLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of U moves to do before
*/
pair_int_int find_pll(int cube[6][9]);

/*
Looks up the right OLL algorithm and runs it
Returns the name of the OLL, and the algorithm, separated by a newline
*/
char *solve_oll(int cube[6][9]);

/*
Looks up the right PLL algorithm and runs it
Returns the name of the PLL, and the algorithm, separated by a newline
*/
char *solve_pll(int cube[6][9]);

//frees the memory used to store OLLs and PLLs
void cleanup_last_layer();

#endif