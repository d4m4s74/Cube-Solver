#ifndef CUBE_H
#define CUBE_H
#include <stdbool.h>

//The moves changed into letters for each of use
enum moves
{
    F,
    R,
    B,
    L,
    U,
    D,
    M,
    E,
    S
};

//A function that adds the colors from four strings onto the cube. Might later be changed to a single string.
void color_cube(int cube[6][9], char *f, char *r, char *b, char *l, char *u, char *d);

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

/*
A function that moves the virtual cube
Each face moves clockwise, assuming you're facing the face.
int move: the move done
0 for F
1 for R
2 for B
...
4 for U
5 for D
6 for M
7 for E
8 for S
int times: the amount of times the same move is repeated.
The function does this recursively.
*/
void move_cube(int cube[6][9], int move, int times);
//Runs a multiple move algorithm on the cube, using standard cube notation
void run_algorithm(int cube[6][9], char *algorithm);
//Validate the colors on the cube. This does not check if the scramble is solvable
bool validate(int cube[6][9]);
#endif