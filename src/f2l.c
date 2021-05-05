#include "cube.h"
#include "utils.h"
#include "f2l.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Corner/edge pair in top layer with corner "after" edge (clockwise) (Pointing left in LUF)
char *f2lCase1(int cube[6][9]);
//Corner/edge pair in top layer with corner "before" edge (anti-clockwise) (Pointing right in RUF)
char *f2lCase2(int cube[6][9]);
//Corner oriented correctly in bottom layer
char *f2lCase3(int cube[6][9]);
//corner/edge pair in wrong hole
char *f2lCase4(int cube[6][9]);
//corner/misoriented edge pair in top layer corner "after" edge (clockwise) (Pointing left in LUF)
char *f2lCase5(int cube[6][9]);
//corner/misoriented edge pair in top layer corner "before" edge (anti clockwise) (Pointing right in RUF)
char *f2lCase6(int cube[6][9]);
//Loose corner in top layer pointing right (assuming in front)
char *f2lCase7(int cube[6][9]);
//Loose corner in top layer pointing left
char *f2lCase8(int cube[6][9]);
//Loose corner in top layer pointing up
char *f2lCase9(int cube[6][9]);
//Corner oriented anti-clockwise in bottom layer (pointing right if in RDF)
char *f2lCase10(int cube[6][9]);
//corner oriented clockwise in bottom layer (pointing left in LDF)
char *f2lCase11(int cube[6][9]);

//Solves the edges of the first two layers of the cube.
char *solve_f2l(int cube[6][9])
{
    //First verify that the cross is solved. If not, return null.
    if (!(cube[D][1] == 5 && cube[F][7] == cube[F][4]) ||
        !(cube[D][3] == 5 && cube[L][7] == cube[L][4]) ||
        !(cube[D][5] == 5 && cube[R][7] == cube[R][4]) ||
        !(cube[D][7] == 5 && cube[B][7] == cube[B][4]))
    {
        return NULL;
    }
    char *algs = malloc(1);
    if (algs == NULL)
    {
        //memory error
        return NULL;
    }
    algs[0] = 0;
    int solved = 0;
    char *alg;
    for (int i = 0; i < 4; i++) //The cross should be able to be solved max 4 rounds. One edge per round.
    {
        //for every corner/edge pair that's solved one is added.
        solved = (cube[D][2] == 5 && cube[F][8] == cube[F][4] && cube[F][5] == cube[F][4] &&
                  cube[R][6] == cube[R][4] && cube[R][3] == cube[R][4]) +
                 (cube[D][8] == 5 && cube[R][8] == cube[R][4] && cube[R][5] == cube[R][4] &&
                  cube[B][6] == cube[B][4] && cube[B][3] == cube[B][4]) +
                 (cube[D][6] == 5 && cube[B][8] == cube[B][4] && cube[B][5] == cube[B][4] &&
                  cube[L][6] == cube[L][4] && cube[L][3] == cube[L][4]) +
                 (cube[D][0] == 5 && cube[L][8] == cube[L][4] && cube[L][5] == cube[L][4] &&
                  cube[F][6] == cube[F][4] && cube[F][3] == cube[F][4]);
        if (solved == 4)
        {
            break;
        }
        //The f2l case names are in the order I created them. the solutions are in the order that's most effficient (least chance it will destroy a shorter option)

        //Corner/edge pair in top layer with corner "after" edge (clockwise) (Pointing left in LUF)
        alg = f2lCase1(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //Corner/edge pair in top layer with corner "before" edge (anti-clockwise) (Pointing right in RUF)
        alg = f2lCase2(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //corner/edge pair in wrong hole
        alg = f2lCase4(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //corner/misoriented edge pair in top layer corner "after" edge (clockwise)
        alg = f2lCase5(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //corner/misoriented edge pair in top layer corner "before" edge (anti clockwise)
        alg = f2lCase6(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }


        //Corner oriented anti-clockwise in bottom layer (pointing right if in RDF)
        alg = f2lCase10(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //corner oriented clockwise in bottom layer (pointing left in LDF)
        alg = f2lCase11(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }
        //Loose corner in top layer pointing right (assuming in front)
        alg = f2lCase7(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //Loose corner in top layer pointing left
        alg = f2lCase8(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //Loose corner in top layer pointing up
        alg = f2lCase9(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }

        //Corner oriented correctly in bottom layer
        alg = f2lCase3(cube);
        if (alg)
        {
            //prints for debugging
            //printf("%s", alg);
            //print_cube(cube);
            algs = append(algs, alg);
            free(alg);
            if (algs == NULL)
            {
                //memory error
                return NULL;
            }
            continue;
        }


        //This point should never be reached unless the cube is unsolvable or the code is faulty. If so, free algs and return null.
        free(algs);
        return NULL;
    }
    //remove trailing newline
    if (strlen(algs) > 1 && algs[strlen(algs) - 1] == '\n')
    {
        algs[strlen(algs) - 1] = 0;
    }
    return algs;
}


/*
Looks for and solves corner/edge pair in top layer with corner "after" edge (clockwise)
*/
char *f2lCase1(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //signifies where the pair is now
    int location = -1;
    //signifies where the pair should go
    int goal = -1;

    //pair on the front with white to the left
    if (cube[L][2] == 5 && cube[F][0] == cube[F][1] && cube[U][6] == cube[U][7])
    {
        location = 0;
        goal = mod(cube[U][6] - cube[F][4], 4);
    }
    //pair on the right with white in front
    else if (cube[F][2] == 5 && cube[R][0] == cube[R][1] && cube[U][8] == cube[U][5])
    {
        location = 1;
        goal = mod(cube[U][8] - cube[R][4], 4);
    }
    //pair on the back with white to the right
    else if (cube[R][2] == 5 && cube[B][0] == cube[B][1] && cube[U][2] == cube[U][1])
    {
        location = 2;
        goal = mod(cube[U][2] - cube[B][4], 4);
    }
    //pair on the left with white to the back
    else if (cube[B][2] == 5 && cube[L][0] == cube[L][1] && cube[U][0] == cube[U][3])
    {
        location = 3;
        goal = mod(cube[U][0] - cube[L][4], 4);
    }
    //If we found a pair
    if (location != -1)
    {
        //If I can move face (location goal + 1) without destroying te the pair
        if (goal == 0 || goal == 1)
        {
            int up = (goal == 0) ? 3 : 2;
            int side = (location + goal + 1) % 4;
            move_cube(cube, side, 1);
            switch (side)
            {
                case 0:
                    alg = append(alg, "F ");
                    break;
                case 1:
                    alg = append(alg, "R ");
                    break;
                case 2:
                    alg = append(alg, "B ");
                    break;
                case 3:
                    alg = append(alg, "L ");
                    break;
            }
            if (goal == 0)
            {
                move_cube(cube, U, 3);
                alg = append(alg, "U' ");
            }
            else
            {
                move_cube(cube, U, 2);
                alg = append(alg, "U2 ");
            }
            move_cube(cube, side, 3);
            switch (side)
            {
                case 0:
                    alg = append(alg, "F'\n");
                    break;
                case 1:
                    alg = append(alg, "R'\n");
                    break;
                case 2:
                    alg = append(alg, "B'\n");
                    break;
                case 3:
                    alg = append(alg, "L'\n");
                    break;
            }
        }
        //if we can't move (location + goal+1)
        else
        {
            int base = (location + goal) % 4;
            if (goal == 2)
            {
                alg = append(alg, "U ");
                move_cube(cube, U, 1);
            }
            else
            {
                alg = append(alg, "");
            }
            switch (base)
            {
                case 0:
                    alg = append(alg, "R' F R F'\n");
                    run_algorithm(cube, "R' F R F'\n");
                    break;
                case 1:
                    alg = append(alg, "B' R B R'\n");
                    run_algorithm(cube, "B' R B R'\n");
                    break;
                case 2:
                    alg = append(alg, "L' B L B'\n");
                    run_algorithm(cube, "L' B L B'\n");
                    break;
                case 3:
                    alg = append(alg, "F' L F L'\n");
                    run_algorithm(cube, "F' L F L'\n");
                    break;
            }

        }
        return alg;
    }
    free(alg);
    return NULL;
}


/*
Finds and solves corner/edge pair in top layer with corner "before" edge (anti-clockwise)
Inverse of f2lCase1()
*/
char *f2lCase2(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //signifies where the pair is now
    int location = -1;
    //signifies where the pair should go
    int goal = -1;
    //Pair in frint with white in the right
    if (cube[R][0] == 5 && cube[F][2] == cube[F][1] && cube[U][8] == cube[U][7])
    {
        location = 0;
        goal = mod(cube[F][4] - cube[U][8], 4);
    }
    //pair on the right with white in the back
    else if (cube[B][0] == 5 && cube[R][2] == cube[R][1] && cube[U][2] == cube[U][5])
    {
        location = 1;
        goal = mod(cube[R][4] - cube[U][2], 4);
    }
    //pair on the back with white to the left
    else if (cube[L][0] == 5 && cube[B][2] == cube[B][1] && cube[U][0] == cube[U][1])
    {
        location = 2;
        goal = mod(cube[B][4] - cube[U][0], 4);
    }
    //pair on the left with white to the front
    else if (cube[F][0] == 5 && cube[L][2] == cube[L][1] && cube[U][6] == cube[U][3])
    {
        location = 3;
        goal = mod(cube[L][4] - cube[U][6], 4);
    }
    //If we found a pair
    if (location != -1)
    {
        //If I can move face (location - goal - 1) without destroying te the pair
        if (goal == 0 || goal == 1)
        {
            int up = (goal == 0) ? 1 : 2;
            int side = mod(location - goal - 1, 4);
            move_cube(cube, side, 3);
            switch (side)
            {
                case 0:
                    alg = append(alg, "F' ");
                    break;
                case 1:
                    alg = append(alg, "R' ");
                    break;
                case 2:
                    alg = append(alg, "B' ");
                    break;
                case 3:
                    alg = append(alg, "L' ");
                    break;
            }
            if (goal == 0)
            {
                move_cube(cube, U, 1);
                alg = append(alg, "U ");
            }
            else
            {
                move_cube(cube, U, 2);
                alg = append(alg, "U2 ");
            }

            move_cube(cube, side, 1);
            switch (side)
            {
                case 0:
                    alg = append(alg, "F\n");
                    break;
                case 1:
                    alg = append(alg, "R\n");
                    break;
                case 2:
                    alg = append(alg, "B\n");
                    break;
                case 3:
                    alg = append(alg, "L\n");
                    break;
            }
        }
        //if we can't move (location - goal-1)
        else
        {
            int base = mod(location - goal, 4);
            if (goal == 2)
            {
                alg = append(alg, "U' ");
                move_cube(cube, U, 3);
            }
            else
            {
                alg = append(alg, "");
            }
            move_cube(cube, mod(base - 1, 4), 1);
            move_cube(cube, base, 3);
            move_cube(cube, mod(base - 1, 4), 3);
            move_cube(cube, base, 1);
            switch (base)
            {
                case 0:
                    alg = append(alg, "L F' L' F\n");
                    break;
                case 1:
                    alg = append(alg, "F R' F' R\n");
                    break;
                case 2:
                    alg = append(alg, "R B' R' B\n");
                    break;
                case 3:
                    alg = append(alg, "B L' B' L\n");
                    break;
            }

        }
        return alg;
    }
    free(alg);
    return NULL;
}

/*
Corner oriented correctly in bottom layer (not necessarily in right location)
*/
char *f2lCase3(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    bool found = false;
    //if the right corner of the front slice is white on the bottom, and the edge above it doesn't fit.
    if (cube[D][2] == 5 && (cube[F][8] != cube[F][5] || cube[R][6] != cube[R][3]))
    {
        found = true;
    }
    //if the right corner of the right slice is white on the bottom
    else if (cube[D][8] == 5 && (cube[R][8] != cube[R][5] || cube[B][6] != cube[B][3]))
    {
        //turn the cube so it's in the front slice
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
        found = true;
    }
    //if the right corner of the back slice is white on the bottom
    else if (cube[D][6] == 5 && (cube[B][8] != cube[B][5] || cube[L][6] != cube[L][3]))
    {
        //turn the cube so it's in the front slice
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
        found = true;
    }
    //if the right corner of the left slice is white on the bottom
    else if (cube[D][0] == 5 && (cube[L][8] != cube[L][5] || cube[F][6] != cube[F][3]))
    {
        //turn the cube so it's in the front slice
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
        found = true;
    }
    if (found)
    {
        int edge = -1;
        //corresponding edge in 2nd layer, oriented correctly
        if (cube[R][5] == cube[F][8] && cube[B][3] == cube[R][6])
        {
            edge = 1;
        }
        else if (cube[B][5] == cube[F][8] && cube[L][3] == cube[R][6])
        {
            edge = 2;
        }
        else if (cube[L][5] == cube[F][8] && cube[F][3] == cube[R][6])
        {
            edge = 3;
        }
        //If we found an edge oriented correctly
        if (edge != -1)
        {
            move_cube(cube, D, edge);
            move_cube(cube, (edge + 1) % 4, 1);
            move_cube(cube, U, 1);
            move_cube(cube, (edge + 1) % 4, 3);
            move_cube(cube, D, 4 - edge);
            switch (edge)
            {
                case 1:
                    alg = append(alg, "D B U B' D' ");
                    break;
                case 2:
                    alg = append(alg, "D2 L U L' D2 ");
                    break;
                case 3:
                    alg = append(alg, "D' F U F' D ");
                    break;
            }
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //corresponding edge in 2nd layer, oriented wrong
        if (cube[F][5] == cube[R][6] && cube[R][3] == cube[F][8])
        {
            edge = 0;
        }
        else if (cube[R][5] == cube[R][6] && cube[B][3] == cube[F][8])
        {
            edge = 1;
        }
        else if (cube[B][5] == cube[R][6] && cube[L][3] == cube[F][8])
        {
            edge = 2;
        }
        else if (cube[L][5] == cube[R][6] && cube[F][3] == cube[F][8])
        {
            edge = 3;
        }
        //If we found an edge oriented wrong
        if (edge != -1)
        {
            switch (edge)
            {
                case 0:
                    alg = append(alg, "R U R' ");
                    run_algorithm(cube, "R U R' ");
                    //ends up with a misoriented pair (edge flipped)
                    alg2 = f2lCase5(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 1:
                    alg = append(alg, "R2 U2 R2 U2 R2 ");
                    run_algorithm(cube, "R2 U2 R2 U2 R2");
                    //check if corner in right location.
                    //We only have to check F8 because if f8 is correct and everything we tested before is correct, the corner is correct
                    if (cube[F][8] == cube[F][4])
                    {
                        alg = append(alg, "\n");
                        return alg;
                    }
                    else
                    {
                        //Ends with a pair in the wrong hole.
                        alg2 = f2lCase4(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    }
                case 2:
                    alg = append(alg, "D2 B' U' B D2 ");
                    run_algorithm(cube, "D2 B' U' B D2");
                    //ends up with a misoriented pair (edge flipped)
                    alg2 = f2lCase6(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 3:
                    alg = append(alg, "F2 U2 F2 U2 F2 ");
                    run_algorithm(cube, "F2 U2 F2 U2 F2");
                    //check if corner in right location
                    if (cube[F][8] == cube[F][4])
                    {
                        alg = append(alg, "\n");
                        return alg;
                    }
                    else
                    {
                        //Ends with a pair in the wrong hole.
                        alg2 = f2lCase4(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    }
            }
        }
        //corresponding edge in top layer
        //oriented counter clockwise from corner (Front of edge == left of corner)
        if (cube[F][1] == cube[F][8] && cube[U][7] == cube[R][6])
        {
            alg = append(alg, "U R U' R' ");
            run_algorithm(cube, "U R U' R' ");
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase2(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[L][1] == cube[F][8] && cube[U][3] == cube[R][6])
        {
            alg = append(alg, "R U' R' ");
            run_algorithm(cube, "R U' R' ");
            alg2 = f2lCase2(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[B][1] == cube[F][8] && cube[U][1] == cube[R][6])
        {
            alg = append(alg, "U' R U' R' ");
            run_algorithm(cube, "U' R U' R' ");
            alg2 = f2lCase2(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[R][1] == cube[F][8] && cube[U][5] == cube[R][6])
        {
            alg = append(alg, "U2 R U' R' ");
            run_algorithm(cube, "U2 R U' R' ");
            alg2 = f2lCase2(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //oriented anti-clockwise from corner (Front of edge == right of corner)
        if (cube[F][1] == cube[R][6] && cube[U][7] == cube[F][8])
        {
            alg = append(alg, "U2 F' U F ");
            run_algorithm(cube, "U2 F' U F ");
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[L][1] == cube[R][6] && cube[U][3] == cube[F][8])
        {
            alg = append(alg, "U F' U F ");
            run_algorithm(cube, "U F' U F ");
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[B][1] == cube[R][6] && cube[U][1] == cube[F][8])
        {
            alg = append(alg, "F' U F ");
            run_algorithm(cube, "F' U F ");
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        else if (cube[R][1] == cube[R][6] && cube[U][5] == cube[F][8])
        {
            alg = append(alg, "U' F' U F ");
            run_algorithm(cube, "U' F' U F ");
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
    }
    free(alg);
    return NULL;
}

//corner/edge pair in wrong hole
char *f2lCase4(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    int location = -1;
    //check if the bottom of the pair is white, the edge is the same color and orientation as the corner, but the edge doesn't fit.
    if (cube[D][2] == 5 && cube[F][5] == cube[F][8] && cube[R][3] == cube[R][6] && cube[F][5] != cube[F][4])
    {
        location = 0;
    }
    else if (cube[D][8] == 5 && cube[R][5] == cube[R][8] && cube[B][3] == cube[B][6] && cube[R][5] != cube[R][4])
    {
        location = 1;
    }
    else if (cube[D][6] == 5 && cube[B][5] == cube[B][8] && cube[L][3] == cube[L][6] && cube[B][5] != cube[B][4])
    {
        location = 2;
    }
    else if (cube[D][0] == 5 && cube[L][5] == cube[L][8] && cube[F][3] == cube[F][6] && cube[L][5] != cube[L][4])
    {
        location = 3;
    }
    if (location != -1)
    {
        switch (location)
        {
            case 0:
                alg = append(alg, "R U R' ");
                run_algorithm(cube, "R U R'");
                //ends with a complete pair in the top layer, ready to be put into the f2l
                alg2 = f2lCase1(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            case 1:
                alg = append(alg, "R' U' R ");
                run_algorithm(cube, "R' U' R");
                //ends with a complete pair in the top layer, ready to be put into the f2l, pointing the other way
                alg2 = f2lCase2(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            case 2:
                alg = append(alg, "L U L' ");
                run_algorithm(cube, "L U L'");
                alg2 = f2lCase1(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            case 3:
                alg = append(alg, "L' U' L ");
                run_algorithm(cube, "L' U' L ");
                alg2 = f2lCase2(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
        }
    }
    free(alg);
    return NULL;
}

//corner/misoriented edge pair in top layer corner "after" edge (clockwise) (Pointing left in LUF)
char *f2lCase5(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //the amount of d moves necessary
    int goal = -1;
    //pair is on front with white in L
    if (cube[L][2] == 5 && cube[F][0] == cube[U][7] && cube[U][6] == cube[F][1])
    {
        //no rotation necessary
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on right with white in F
    else if (cube[F][2] == 5 && cube[R][0] == cube[U][5] && cube[U][8] == cube[R][1])
    {
        //y necessary
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on back with white in R
    else if (cube[R][2] == 5 && cube[B][0] == cube[U][1] && cube[U][2] == cube[B][1])
    {
        //y necessary
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on left with white in B
    else if (cube[B][2] == 5 && cube[L][0] == cube[U][3] && cube[U][0] == cube[L][1])
    {
        //y necessary
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //If we found one
    if (goal != -1)
    {
        //first turn d (not D) into position
        switch (goal)
        {
            case 0:
                alg = append(alg, "");
                break;
            case 1:
                alg = append(alg, "d ");
                run_algorithm(cube, "d ");
                break;
            case 2:
                alg = append(alg, "d2 ");
                run_algorithm(cube, "d2 ");
                break;
            case 3:
                alg = append(alg, "d' ");
                run_algorithm(cube, "d' ");
                break;
        }
        //Then it's one simple algorythm.
        alg = append(alg, "L U2 L2 U' L2 U' L'\n");
        run_algorithm(cube, "L U2 L2 U' L2 U' L'");
        return alg;
    }

    free(alg);
    return NULL;
}

//corner/misoriented edge pair in top layer corner "before" edge (anti clockwise) (Pointing right in LUF)
char *f2lCase6(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //the amount of d moves necessary
    int goal = -1;
    //pair is on front with white in R
    if (cube[R][0] == 5 && cube[F][1] == cube[U][8] && cube[U][7] == cube[F][2])
    {
        //no rotation necessary
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on right with white in B
    else if (cube[B][0] == 5 && cube[R][1] == cube[U][2] && cube[U][5] == cube[R][2])
    {
        //y necessary
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on back with white in L
    else if (cube[L][0] == 5 && cube[B][1] == cube[U][0] && cube[U][1] == cube[B][2])
    {
        //y necessary
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //pair is on left with white in F
    else if (cube[F][0] == 5 && cube[L][1] == cube[U][6] && cube[U][3] == cube[L][2])
    {
        //y necessary
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
        goal = mod(cube[F][4] - cube[U][7], 4);
    }
    //If we found one
    if (goal != -1)
    {
        //first turn d (not D) into position
        switch (goal)
        {
            case 0:
                alg = append(alg, "");
                break;
            case 1:
                alg = append(alg, "d ");
                run_algorithm(cube, "d ");
                break;
            case 2:
                alg = append(alg, "d2 ");
                run_algorithm(cube, "d2 ");
                break;
            case 3:
                alg = append(alg, "d' ");
                run_algorithm(cube, "d' ");
                break;
        }
        //Then it's one simple algorythm.
        alg = append(alg, "R' U2 R2 U R2 U R\n");
        run_algorithm(cube, "R' U2 R2 U R2 U R");
        return alg;
    }

    free(alg);
    return NULL;
}

//Loose corner in top layer pointing right (assuming in front)
char *f2lCase7(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //The location of the edge anti-clockwise. 0 being front right corner, 3 being left front corner
    int location = -1;
    //If there is a white corner pointing right, with  no connected edge (the last check is probably unnecessary because the connect edge cases are done first)
    if (cube[R][0] == 5 && !(cube[F][1] == cube[U][8] && cube[F][2] == cube[U][7]) && !(cube[F][1] == cube[U][7]
            && cube[F][2] == cube[U][8]))
    {
        location = 0;
    }
    else if (cube[B][0] == 5 && !(cube[R][1] == cube[U][2] && cube[R][2] == cube[U][5]) && !(cube[R][1] == cube[U][5]
             && cube[R][2] == cube[U][2]))
    {
        location = 1;
    }
    else if (cube[L][0] == 5 && !(cube[B][1] == cube[U][0] && cube[B][2] == cube[U][1]) && !(cube[B][1] == cube[U][1]
             && cube[B][2] == cube[U][0]))
    {
        location = 2;
    }
    else if (cube[F][0] == 5 && !(cube[L][1] == cube[U][6] && cube[L][2] == cube[U][3]) && !(cube[L][1] == cube[U][3]
             && cube[L][2] == cube[U][6]))
    {
        location = 3;
    }
    //if we found a corner
    if (location != -1)
    {
        //location of edge on top row, amount of U moves to corner.
        int edge = -1;
        //save if the edge is oriented the same as the corner
        bool oriented;
        //Unfortunately because of the numbers in U I can't do this in a simple loop.
        switch (location)
        {
            case 0:
                //3 away, oriented correctly
                if (cube[F][2] == cube[L][1] && cube[U][8] == cube[U][3])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[F][2] == cube[B][1] && cube[U][8] == cube[U][1])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[F][2] == cube[R][1] && cube[U][8] == cube[U][5])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[F][2] == cube[U][3] && cube[U][8] == cube[L][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[F][2] == cube[U][1] && cube[U][8] == cube[B][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[F][2] == cube[U][5] && cube[U][8] == cube[R][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 1:
                //3 away, oriented correctly
                if (cube[R][2] == cube[F][1] && cube[U][2] == cube[U][7])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[R][2] == cube[L][1] && cube[U][2] == cube[U][3])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[R][2] == cube[B][1] && cube[U][2] == cube[U][1])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[R][2] == cube[U][7] && cube[U][2] == cube[F][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[R][2] == cube[U][3] && cube[U][2] == cube[L][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[R][2] == cube[U][1] && cube[U][2] == cube[B][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 2:
                //3 away, oriented correctly
                if (cube[B][2] == cube[R][1] && cube[U][0] == cube[U][5])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[B][2] == cube[F][1] && cube[U][0] == cube[U][7])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[B][2] == cube[L][1] && cube[U][0] == cube[U][3])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[B][2] == cube[U][5] && cube[U][0] == cube[R][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[B][2] == cube[U][7] && cube[U][0] == cube[F][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[B][2] == cube[U][3] && cube[U][0] == cube[L][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 3:
                //3 away, oriented correctly
                if (cube[L][2] == cube[B][1] && cube[U][6] == cube[U][1])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[L][2] == cube[R][1] && cube[U][6] == cube[U][5])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[L][2] == cube[F][1] && cube[U][6] == cube[U][7])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[L][2] == cube[U][1] && cube[U][6] == cube[B][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[L][2] == cube[U][5] && cube[U][6] == cube[R][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[L][2] == cube[U][7] && cube[U][6] == cube[F][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
        }
        //if corresponding edge found correctly oriented in top layer, not next to the white part of the edge.
        if ((edge == 2 || edge == 3) && oriented == true)
        {
            //make sure there open space under R/B so we don't remove an existing pair
            switch (location)
            {
                case 0:
                    if (cube[D][8] == 5)
                    {
                        if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 3)
                    {
                        alg = append(alg, "R' U' R ");
                    }
                    else
                    {
                        alg = append(alg, "R' U2 R ");
                    }
                    move_cube(cube, R, 3);
                    move_cube(cube, U, edge);
                    move_cube(cube, R, 1);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 1:
                    if (cube[D][6] == 5)
                    {
                        if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 3)
                    {
                        alg = append(alg, "B' U' B ");
                    }
                    else
                    {
                        alg = append(alg, "B' U2 B ");
                    }
                    move_cube(cube, B, 3);
                    move_cube(cube, U, edge);
                    move_cube(cube, B, 1);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 2:
                    if (cube[D][0] == 5)
                    {
                        if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 3)
                    {
                        alg = append(alg, "L' U' L ");
                    }
                    else
                    {
                        alg = append(alg, "L' U2 L ");
                    }
                    move_cube(cube, L, 3);
                    move_cube(cube, U, edge);
                    move_cube(cube, L, 1);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 3:
                    if (cube[D][2] == 5)
                    {
                        if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                        else if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 3)
                    {
                        alg = append(alg, "F' U' F ");
                    }
                    else
                    {
                        alg = append(alg, "F' U2 F ");
                    }
                    move_cube(cube, F, 3);
                    move_cube(cube, U, edge);
                    move_cube(cube, F, 1);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
            }

        }
        if (edge == 1 && oriented == true)
        {
            switch (location)
            {
                case 0:
                    if (cube[D][0] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "F U' F'");
                    alg = append(alg, "F U' F' ");
                    if (cube[D][8] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "R' U2 R ");
                    run_algorithm(cube, "R' U2 R");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 1:
                    if (cube[D][2] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "R U' R'");
                    alg = append(alg, "R U' R' ");
                    if (cube[D][6] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "B' U2 B ");
                    run_algorithm(cube, "B' U2 B");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 2:
                    if (cube[D][8] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "B U' B'");
                    alg = append(alg, "B U' B' ");
                    if (cube[D][0] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "L' U2 L ");
                    run_algorithm(cube, "L' U2 L");
                    //print_cube(cube);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 3:
                    if (cube[D][6] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "L U' L'");
                    alg = append(alg, "L U' L' ");
                    if (cube[D][2] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "F' U2 F ");
                    run_algorithm(cube, "F' U2 F");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase2(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
            }
        }

        //if corresponding edge found incorrectly oriented in top layer
        if (edge != -1 && oriented == false)
        {
            int goal = mod(cube[location][2] - cube[location][4], 4);
            if (edge == 1)
            {
                switch (goal)
                {
                    case 2:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                    case 3:
                        alg = append(alg, "");
                        break;
                    case 0:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                    case 1:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                }
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F U' F' U F U F'");
                        run_algorithm(cube, "F U' F' U F U F'");
                        break;
                    case 1:
                        alg = append(alg, "R U' R' U R U R'");
                        run_algorithm(cube, "R U' R' U R U R'");
                        break;
                    case 2:
                        alg = append(alg, "B U' B' U B U B'");
                        run_algorithm(cube, "B U' B' U B U B'");
                        break;
                    case 3:
                        alg = append(alg, "L U' L' U L U L'");
                        run_algorithm(cube, "L U' L' U L U L'");
                        break;
                }
                if (cube[location][3] == cube[location][4])
                {
                    alg = append(alg, "\n");
                    return alg;
                }
                else
                {
                    alg = append(alg, " ");
                    //Ends with a pair in the wrong hole.
                    alg2 = f2lCase4(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                }
            }
            else if (edge == 2)
            {
                switch (goal)
                {
                    case 0:
                        alg = append(alg, "");
                        break;
                    case 3:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                    case 2:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                    case 1:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                }
                switch (location)
                {
                    case 0:
                        alg = append(alg, "R U R'\n");
                        run_algorithm(cube, "R U R'");
                        return alg;
                    case 1:
                        alg = append(alg, "B U B'\n");
                        run_algorithm(cube, "B U B'");
                        return alg;
                    case 2:
                        alg = append(alg, "L U L'\n");
                        run_algorithm(cube, "L U L'");
                        return alg;
                    case 3:
                        alg = append(alg, "F U F'\n");
                        run_algorithm(cube, "F U F'");
                        return alg;
                }
            }
            else if (edge == 3)
            {
                switch (goal)
                {
                    case 1:
                        alg = append(alg, "");
                        break;
                    case 0:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                    case 3:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                    case 2:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                }
                switch (location)
                    //Might not work, might be wrong edge case that gets here.
                    //TODO: REDO
                {
                    case 0:
                        alg = append(alg, "R' U R d' R U R'\n");
                        run_algorithm(cube, "R' U R d' R U R'");
                        return alg;
                    case 1:
                        alg = append(alg, "B' U B d' B U B'\n");
                        run_algorithm(cube, "B' U B d' B U B'");
                        return alg;
                    case 2:
                        alg = append(alg, "L' U L d' L U L'\n");
                        run_algorithm(cube, "L' U L d' L U L'");
                        return alg;
                    case 3:
                        alg = append(alg, "F' U F d' F U F'\n");
                        run_algorithm(cube, "F' U F d' F U F'");
                        return alg;
                }
            }
        }
        //edges in f2l
        //location of the edge
        edge = -1;
        //it's oriented the same as the edge (left color of edge == front color or corner)
        switch (location)
        {
            case 0:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[F][2] && cube[mod(edge + 1, 4)][3] == cube[U][8])
                    {
                        oriented = true;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][8] && cube[mod(edge + 1, 4)][3] == cube[F][2])
                    {
                        oriented = false;
                        break;
                    }
                }
                break;
            case 1:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[R][2] && cube[mod(edge + 1, 4)][3] == cube[U][2])
                    {
                        oriented = true;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][2] && cube[mod(edge + 1, 4)][3] == cube[R][2])
                    {
                        oriented = false;
                        break;
                    }
                }
                break;
            case 2:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[B][2] && cube[mod(edge + 1, 4)][3] == cube[U][0])
                    {
                        oriented = true;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][0] && cube[mod(edge + 1, 4)][3] == cube[B][2])
                    {
                        oriented = false;
                        break;
                    }
                }

                break;
            case 3:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[L][2] && cube[mod(edge + 1, 4)][3] == cube[U][6])
                    {
                        oriented = true;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][6] && cube[mod(edge + 1, 4)][3] == cube[L][2])
                    {
                        oriented = false;
                        break;
                    }
                }
                break;
        }
        if (edge != -1 && oriented == true)
        {
            //turn the cube so the edge is front/right
            switch (edge)
            {
                case 1:
                    alg = append(alg, "y ");
                    run_algorithm(cube, "y");
                    location = mod(location - 1, 4);
                    break;
                case 2:
                    alg = append(alg, "y2 ");
                    run_algorithm(cube, "y2");
                    location = mod(location - 2, 4);
                    break;
                case 3:
                    alg = append(alg, "y' ");
                    run_algorithm(cube, "y'");
                    location = mod(location + 1, 4);
                    break;
            }
            switch (location)
            {
                case 0:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                case 1:
                    alg = append(alg, "");
                    break;
                case 2:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 3:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
            }
            //extra U to make the next step easily find it.
            alg = append(alg, "R U2 R' U ");
            run_algorithm(cube, "R U2 R' U ");
            //ends up with a loose corner in U, spot 0, and the edge in spot 3
            alg2 = f2lCase7(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        if (edge != -1 && oriented == false)
        {
            //turn the cube so the edge is front/right
            switch (edge)
            {
                case 1:
                    alg = append(alg, "y ");
                    run_algorithm(cube, "y");
                    location = mod(location - 1, 4);
                    break;
                case 2:
                    alg = append(alg, "y2 ");
                    run_algorithm(cube, "y2");
                    location = mod(location - 2, 4);
                    break;
                case 3:
                    alg = append(alg, "y' ");
                    run_algorithm(cube, "y'");
                    location = mod(location - 3, 4);
                    break;
            }
            switch (location)
            {
                case 2:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
                case 3:
                    alg = append(alg, "");
                    break;
                case 0:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 1:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
            }
            alg = append(alg, "R U R' U2 "); //extra U2 so this piece is done next
            run_algorithm(cube, "R U R' U2 ");
            //ends up with a loose corner in U, spot 0, and the edge in U
            //print_cube(cube); //testing
            alg2 = f2lCase7(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
    }
    free(alg);
    return NULL;
}

//Loose corner in top layer pointing left (assuming in front)
char *f2lCase8(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //The location of the edge anti-clockwise. 0 being front left corner, 3 being right front corner
    int location = -1;
    //If there is a white corner pointing left, with  no connected edge (the last check is probably unnecessary because the connect edge cases are done first)
    if (cube[L][2] == 5 && !(cube[F][1] == cube[U][7] && cube[F][0] == cube[U][7]) && !(cube[F][1] == cube[U][7]
            && cube[F][0] == cube[U][6]))
    {
        location = 0;
    }
    else if (cube[F][2] == 5 && !(cube[R][1] == cube[U][8] && cube[R][0] == cube[U][5]) && !(cube[R][1] == cube[U][5]
             && cube[R][0] == cube[U][8]))
    {
        location = 1;
    }
    else if (cube[R][2] == 5 && !(cube[B][1] == cube[U][2] && cube[B][0] == cube[U][1]) && !(cube[B][1] == cube[U][1]
             && cube[B][0] == cube[U][2]))
    {
        location = 2;
    }
    else if (cube[B][2] == 5 && !(cube[L][1] == cube[U][0] && cube[L][0] == cube[U][3]) && !(cube[L][1] == cube[U][3]
             && cube[L][0] == cube[U][0]))
    {
        location = 3;
    }
    //if we found a corner
    if (location != -1)
    {
        //location of edge on top row, amount of U'moves to corner.
        int edge = -1;
        //save if the edge is oriented the same as the corner
        bool oriented;
        //Unfortunately because of the numbers in U I can't do this in a simple loop.
        switch (location)
        {
            case 0:
                //3 away, oriented correctly
                if (cube[F][0] == cube[L][1] && cube[U][6] == cube[U][3])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[F][0] == cube[B][1] && cube[U][6] == cube[U][1])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[F][0] == cube[R][1] && cube[U][6] == cube[U][5])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[F][0] == cube[U][3] && cube[U][6] == cube[L][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[F][0] == cube[U][1] && cube[U][6] == cube[B][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[F][0] == cube[U][5] && cube[U][6] == cube[R][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 1:
                //3 away, oriented correctly
                if (cube[R][0] == cube[F][1] && cube[U][8] == cube[U][7])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[R][0] == cube[L][1] && cube[U][8] == cube[U][3])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[R][0] == cube[B][1] && cube[U][8] == cube[U][1])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[R][0] == cube[U][7] && cube[U][8] == cube[F][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[R][0] == cube[U][3] && cube[U][8] == cube[L][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[R][0] == cube[U][1] && cube[U][8] == cube[B][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 2:
                //3 away, oriented correctly
                if (cube[B][0] == cube[R][1] && cube[U][2] == cube[U][5])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[B][0] == cube[F][1] && cube[U][2] == cube[U][7])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[B][0] == cube[L][1] && cube[U][2] == cube[U][3])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[B][0] == cube[U][5] && cube[U][2] == cube[R][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[B][0] == cube[U][7] && cube[U][2] == cube[F][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[B][0] == cube[U][3] && cube[U][2] == cube[L][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
            case 3:
                //3 away, oriented correctly
                if (cube[L][0] == cube[B][1] && cube[U][0] == cube[U][1])
                {
                    edge = 3;
                    oriented = true;
                }
                //2 away oriented correctly
                else if (cube[L][0] == cube[R][1] && cube[U][0] == cube[U][5])
                {
                    edge = 2;
                    oriented = true;
                }
                //1 away oriented correctly
                else if (cube[L][0] == cube[F][1] && cube[U][0] == cube[U][7])
                {
                    edge = 1;
                    oriented = true;
                }
                //3 incorrectly
                else if (cube[L][0] == cube[U][1] && cube[U][0] == cube[B][1])
                {
                    edge = 3;
                    oriented = false;
                }
                //2 incorrectly
                else if (cube[L][0] == cube[U][5] && cube[U][0] == cube[R][1])
                {
                    edge = 2;
                    oriented = false;
                }
                //1 incorrectly
                else if (cube[L][0] == cube[U][7] && cube[U][0] == cube[F][1])
                {
                    edge = 1;
                    oriented = false;
                }
                break;
        }
        //if corresponding edge found correctly oriented in top layer, not next to the white part of the edge.
        if ((edge == 2 || edge == 1) && oriented == true)
        {
            switch (location)
            {
                case 0:
                    if (cube[D][6] == 5)
                    {
                        if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 1)
                    {
                        alg = append(alg, "L U L' ");
                    }
                    else
                    {
                        alg = append(alg, "L U2 L' ");
                    }
                    move_cube(cube, L, 1);
                    move_cube(cube, U, edge);
                    move_cube(cube, L, 3);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 1:
                    if (cube[D][0] == 5)
                    {
                        if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 1)
                    {
                        alg = append(alg, "F U F' ");
                    }
                    else
                    {
                        alg = append(alg, "F U2 F' ");
                    }

                    move_cube(cube, F, 1);
                    move_cube(cube, U, edge);
                    move_cube(cube, F, 3);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 2:
                    if (cube[D][2] == 5)
                    {
                        if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][8] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 1)
                    {
                        alg = append(alg, "R U R' ");
                    }
                    else
                    {
                        alg = append(alg, "R U2 R' ");
                    }
                    move_cube(cube, R, 1);
                    move_cube(cube, U, edge);
                    move_cube(cube, R, 3);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 3:
                    if (cube[D][8] == 5)
                    {
                        if (cube[D][2] != 5)
                        {
                            run_algorithm(cube, "d");
                            alg = append(alg, "d ");
                        }
                        else if (cube[D][0] != 5)
                        {
                            run_algorithm(cube, "d2");
                            alg = append(alg, "d2 ");
                        }
                        else if (cube[D][6] != 5)
                        {
                            run_algorithm(cube, "d'");
                            alg = append(alg, "d' ");
                        }
                    }
                    else
                    {
                        alg = append(alg, "");
                    }
                    if (edge == 1)
                    {
                        alg = append(alg, "B U B' ");
                    }
                    else
                    {
                        alg = append(alg, "B U2 B' ");
                    }
                    move_cube(cube, B, 1);
                    move_cube(cube, U, edge);
                    move_cube(cube, B, 3);
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
            }

        }
        if (edge == 3 && oriented == true)
        {
            switch (location)
            {
                case 0:
                    if (cube[D][2] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "F' U F");
                    alg = append(alg, "F' U F ");
                    if (cube[D][6] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "L U2 L' ");
                    run_algorithm(cube, "L U2 L'");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 1:
                    if (cube[D][8] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "R' U R");
                    alg = append(alg, "R' U R ");
                    if (cube[D][0] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "F U2 F' ");
                    run_algorithm(cube, "F U2 F'");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 2:
                    if (cube[D][6] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][0] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "B' U B");
                    alg = append(alg, "B' U B ");
                    if (cube[D][2] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "R U2 R' ");
                    run_algorithm(cube, "R U2 R'");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
                case 3:
                    if (cube[D][0] != 5)
                    {
                        alg = append(alg, "");
                    }
                    else if (cube[D][6] != 5)
                    {
                        run_algorithm(cube, "d");
                        alg = append(alg, "d ");
                    }
                    else if (cube[D][8] != 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    else if (cube[D][2] != 5)
                    {
                        run_algorithm(cube, "d'");
                        alg = append(alg, "d' ");
                    }
                    run_algorithm(cube, "L' U L");
                    alg = append(alg, "L' U L ");
                    if (cube[D][8] == 5)
                    {
                        run_algorithm(cube, "d2");
                        alg = append(alg, "d2 ");
                    }
                    alg = append(alg, "B U2 B' ");
                    run_algorithm(cube, "B U2 B'");
                    //ends with a complete pair in the top layer, ready to be put into the f2l
                    alg2 = f2lCase1(cube);
                    alg = append(alg, alg2);
                    free(alg2);
                    return alg;
            }
        }

        //if corresponding edge found incorrectly oriented in top layer
        if (edge != -1 && oriented == false)
        {
            int goal = mod(cube[location][0] - cube[location][4], 4);
            if (edge == 3)
            {
                switch (goal)
                {
                    case 2:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                    case 1:
                        alg = append(alg, "");
                        break;
                    case 0:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                    case 3:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                }
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F' U F U' F' U' F\n");
                        run_algorithm(cube, "F' U F U' F' U' F");
                        return alg;
                    case 1:
                        alg = append(alg, "R' U R U' R' U' R\n");
                        run_algorithm(cube, "R' U R U' R' U' R");
                        return alg;
                    case 2:
                        alg = append(alg, "B' U B U' B' U' B\n");
                        run_algorithm(cube, "B' U B U' B' U' B");
                        return alg;
                    case 3:
                        alg = append(alg, "L' U L U' L' U' L\n");
                        run_algorithm(cube, "L' U L U' L' U' L");
                        return alg;
                }
            }
            else if (edge == 2)
            {
                switch (goal)
                {
                    case 0:
                        alg = append(alg, "");
                        break;
                    case 1:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                    case 2:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                    case 3:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                }
                switch (location)
                {
                    case 0:
                        alg = append(alg, "L' U' L\n");
                        run_algorithm(cube, "L' U' L");
                        return alg;
                    case 1:
                        alg = append(alg, "F' U' F\n");
                        run_algorithm(cube, "F' U' F");
                        return alg;
                    case 2:
                        alg = append(alg, "R' U' R\n");
                        run_algorithm(cube, "R' U' R");
                        return alg;
                    case 3:
                        alg = append(alg, "B' U' B\n");
                        run_algorithm(cube, "B' U' B");
                        return alg;
                }
            }
            else if (edge == 1)
            {
                switch (goal)
                {
                    case 3:
                        alg = append(alg, "");
                        break;
                    case 0:
                        alg = append(alg, "d' ");
                        run_algorithm(cube, "d'");
                        break;
                    case 1:
                        alg = append(alg, "d2 ");
                        run_algorithm(cube, "d2");
                        break;
                    case 2:
                        alg = append(alg, "d ");
                        run_algorithm(cube, "d");
                        break;
                }
                switch (location)
                {
                    case 0:
                        alg = append(alg, "L U' L' d L' U' L\n");
                        run_algorithm(cube, "L U' L' d L' U' L");
                        return alg;
                    case 1:
                        alg = append(alg, "F U' F' d F' U' F\n");
                        run_algorithm(cube, "F U' F' d F' U' F");
                        return alg;
                    case 2:
                        alg = append(alg, "R U' R' d R' U' R\n");
                        run_algorithm(cube, "R U' R' d R' U' R");
                        return alg;
                    case 3:
                        alg = append(alg, "B U' B' d B' U' B\n");
                        run_algorithm(cube, "B U' B' d B' U' B");
                        return alg;
                }
            }
        }
        //edges in f2l
        //location of the edge
        edge = -1;
        //it's oriented the same as the edge (left color of edge == front color or corner)
        switch (location)
        {
            case 0:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[F][0] && cube[mod(edge + 1, 4)][3] == cube[U][6])
                    {
                        oriented = false;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][6] && cube[mod(edge + 1, 4)][3] == cube[F][0])
                    {
                        oriented = true;
                        break;
                    }
                }
                break;
            case 1:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[R][0] && cube[mod(edge + 1, 4)][3] == cube[U][8])
                    {
                        oriented = false;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][8] && cube[mod(edge + 1, 4)][3] == cube[R][0])
                    {
                        oriented = true;
                        break;
                    }
                }
                break;
            case 2:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[B][0] && cube[mod(edge + 1, 4)][3] == cube[U][2])
                    {
                        oriented = false;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][2] && cube[mod(edge + 1, 4)][3] == cube[B][0])
                    {
                        oriented = true;
                        break;
                    }
                }

                break;
            case 3:
                for (edge = 0; edge < 4; edge++)
                {
                    if (cube[edge][5] == cube[L][0] && cube[mod(edge + 1, 4)][3] == cube[U][0])
                    {
                        oriented = false;
                        break;
                    }
                    else if (cube[edge][5] == cube[U][0] && cube[mod(edge + 1, 4)][3] == cube[L][0])
                    {
                        oriented = true;
                        break;
                    }
                }
                break;
        }
        if (edge != -1 && edge != 4 && oriented == true)
        {
            //turn the cube so the edge is front/right
            switch (edge)
            {
                case 0:
                    alg = append(alg, "y ");
                    run_algorithm(cube, "y");
                    location = mod(location - 1, 4);
                    break;
                case 1:
                    alg = append(alg, "y2 ");
                    run_algorithm(cube, "y2");
                    location = mod(location + 2, 4);
                    break;
                case 2:
                    alg = append(alg, "y' ");
                    run_algorithm(cube, "y'");
                    location = mod(location + 1, 4);
                    break;
            }
            switch (location)
            {
                case 2:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
                case 3:
                    alg = append(alg, "");
                    break;
                case 0:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 1:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
            }
            //If it's in the right spot, immediately put it in
            if (cube[L][5] == cube[L][4])
            {
                alg = append(alg, "L' U2 L U' L' U' L\n");
                run_algorithm(cube, "L' U2 L U' L' U' L ");
                return alg;
            }
            else
            {
                //the last y is there so the cubie will be seen first
                alg = append(alg, "L' U2 L y' ");
                run_algorithm(cube, "L' U2 L y' ");
                //Ends with a pair in the wrong hole.
                alg2 = f2lCase8(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }


        }
        if (edge != -1 && oriented == false)
        {
            //turn the cube so the edge is front/left
            switch (edge)
            {
                case 0:
                    alg = append(alg, "y ");
                    run_algorithm(cube, "y");
                    location = mod(location - 1, 4);
                    break;
                case 1:
                    alg = append(alg, "y2 ");
                    run_algorithm(cube, "y2");
                    location = mod(location - 2, 4);
                    break;
                case 2:
                    alg = append(alg, "y' ");
                    run_algorithm(cube, "y'");
                    location = mod(location - 3, 4);
                    break;
            }
            switch (location)
            {
                case 3:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
            }
            alg = append(alg, "L' U' L ");
            run_algorithm(cube, "L' U' L ");
            //Ends up with the corner on the bottom layer, and the edge in the top layer.
            alg2 = f2lCase3(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
    }
    free(alg);
    return NULL;
}

//Loose corner in top layer pointing up
char *f2lCase9(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    int location = -1;
    if (cube[U][8] == 5)
    {
        location = 0;
    }
    else if (cube[U][2] == 5)
    {
        location = 1;
    }
    else if (cube[U][0] == 5)
    {
        location = 2;
    }
    else if (cube[U][6] == 5)
    {
        location = 3;
    }
    //If found
    if (location != -1)
    {
        //Check for edges in F2L
        int edge;
        bool oriented;
        for (edge = 0; edge < 4; edge++)
        {
            //if oriented the "easy" way (so they become a pair with just an u move)
            if (cube[edge][5] == cube[location][2] && cube[mod(edge + 1, 4)][3] == cube[mod(location + 1, 4)][0])
            {
                oriented = true;
                break;
            }
            //if oriented the hard way
            else if (cube[mod(edge + 1, 4)][3] == cube[location][2] && cube[edge][5] == cube[mod(location + 1, 4)][0])
            {
                oriented = false;
                break;
            }
        }
        //found an easy one
        if (edge < 4 && oriented == true)
        {
            //place the corner above the edge
            move_cube(cube, U, mod(location - edge, 4));
            switch (mod(location - edge, 4))
            {
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    break;
                case 3:
                    alg = append(alg, "U' ");
                    break;
            }
            //move out the corner edge pair
            move_cube(cube, edge, 3);
            move_cube(cube, U, 1);
            move_cube(cube, edge, 1);
            switch (edge)
            {
                case 0:
                    alg = append(alg, "F' U F ");
                    break;
                case 1:
                    alg = append(alg, "R' U R ");
                    break;
                case 2:
                    alg = append(alg, "B' U B ");
                    break;
                case 3:
                    alg = append(alg, "L' U L ");
                    break;
            }
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //found a slightly more difficult one
        if (edge < 4 && oriented == false)
        {
            if (cube[edge][5] == cube[edge][4])
            {
                //todo, combine both switches to replace y and u with d
                switch (edge)
                {
                    case 1:
                        alg = append(alg, "y ");
                        run_algorithm(cube, "y");
                        break;
                    case 2:
                        alg = append(alg, "y2 ");
                        run_algorithm(cube, "y2");
                        break;
                    case 3:
                        alg = append(alg, "y' ");
                        run_algorithm(cube, "y'");
                        break;
                }
                move_cube(cube, U, mod(location - edge, 4));
                switch (mod(location - edge, 4))
                {
                    case 1:
                        alg = append(alg, "U ");
                        break;
                    case 2:
                        alg = append(alg, "U2 ");
                        break;
                    case 3:
                        alg = append(alg, "U' ");
                        break;
                }

                //todo, multiple variants
                alg = append(alg, "R U R' U' R U R' U' R U R'\n");
                run_algorithm(cube, "R U R' U' R U R' U' R U R'\n");
                return alg;
            }
            else
            {
                //turn the cube so edge is front right
                switch (edge)
                {
                    case 1:
                        alg = append(alg, "y ");
                        run_algorithm(cube, "y");
                        break;
                    case 2:
                        alg = append(alg, "y2 ");
                        run_algorithm(cube, "y2");
                        break;
                    case 3:
                        alg = append(alg, "y' ");
                        run_algorithm(cube, "y'");
                        break;
                }
                switch (mod(location - edge, 4))
                {
                    case 2:
                        alg = append(alg, "U ");
                        run_algorithm(cube, "U ");
                        break;
                    case 3:
                        alg = append(alg, "U2 ");
                        run_algorithm(cube, "U2 ");
                        break;
                    case 0:
                        alg = append(alg, "U' ");
                        run_algorithm(cube, "U' ");
                        break;
                }
                alg = append(alg, "R U R' U R U' R' ");
                run_algorithm(cube, "R U R' U R U' R'");
                //ends up with a loose corner in U, spot 0, and the edge in U
                alg2 = f2lCase7(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
        }
        //edge is not relative to location
        edge = -1;
        //Oriented is no longer a true/false, but a 0, 1 because there are two possible cases;
        //1 means the U color == the location + 1 color.
        oriented = 0;
        if (cube[U][7] == cube[(location + 1) % 4][0] && cube[F][1] == cube[location][2])
        {
            edge = 0;
            oriented = 1;
        }
        else if (cube[U][7] == cube[location][2] && cube[F][1] == cube[(location + 1) % 4][0])
        {
            edge = 0;
            oriented = 0;
        }
        else if (cube[U][5] == cube[(location + 1) % 4][0] && cube[R][1] == cube[location][2])
        {
            edge = 1;
            oriented = 1;
        }
        else if (cube[U][5] == cube[location][2] && cube[R][1] == cube[(location + 1) % 4][0])
        {
            edge = 1;
            oriented = 0;
        }
        else if (cube[U][1] == cube[(location + 1) % 4][0] && cube[B][1] == cube[location][2])
        {
            edge = 2;
            oriented = 1;
        }
        else if (cube[U][1] == cube[location][2] && cube[B][1] == cube[(location + 1) % 4][0])
        {
            edge = 2;
            oriented = 0;
        }
        else if (cube[U][3] == cube[(location + 1) % 4][0] && cube[L][1] == cube[location][2])
        {
            edge = 3;
            oriented = 1;
        }
        else if (cube[U][3] == cube[location][2] && cube[L][1] == cube[(location + 1) % 4][0])
        {
            edge = 3;
            oriented = 0;
        }
        //If we have found one
        if (edge != -1)
        {
            //First make sure there is an opening below the corner, and the corner is in F U R
            //If there is no opening
            if (cube[location][8] == cube[location][4] && cube[(location + 1) % 4][6] == cube[(location + 1) % 4][4])
            {
                //find the opening;
                for (int opening = 0; opening < 4; opening++)
                {
                    //if we found one
                    if (!(cube[opening][8] == cube[opening][4] && cube[(opening + 1) % 4][6] == cube[(opening + 1) % 4][4]))
                    {
                        //u is the amount of u moves you have to do
                        int u = mod(location - opening, 4);
                        //first turn the cube so the opening is front right
                        switch (opening)
                        {
                            case 1:
                                alg = append(alg, "y ");
                                run_algorithm(cube, "y");
                                break;
                            case 2:
                                alg = append(alg, "y2 ");
                                run_algorithm(cube, "y2");
                                break;
                            case 3:
                                alg = append(alg, "y' ");
                                run_algorithm(cube, "y'");
                                break;
                        }
                        //update locations
                        location = mod(location - opening, 4);
                        edge = mod(edge - opening, 4);
                        //then do the aforementioned u move
                        switch (u)
                        {
                            case 0:
                                alg = append(alg, "");
                                break;
                            case 1:
                                alg = append(alg, "U ");
                                run_algorithm(cube, "U ");
                                break;
                            case 2:
                                alg = append(alg, "U2 ");
                                run_algorithm(cube, "U2 ");
                                break;
                            case 3:
                                alg = append(alg, "U' ");
                                run_algorithm(cube, "U' ");
                                break;
                        }
                        location = mod(location - u, 4);
                        edge = mod(edge - u, 4);
                        break;
                    }
                }
            }
            //if there is an opening
            else
            {
                //Just turn the cube so the corner is front right
                switch (location)
                {
                    case 0:
                        //if no move print an empty bracket because we expect one later
                        alg = append(alg, "");
                        break;
                    case 1:
                        alg = append(alg, "y ");
                        run_algorithm(cube, "y");
                        break;
                    case 2:
                        alg = append(alg, "y2 ");
                        run_algorithm(cube, "y2");
                        break;
                    case 3:
                        alg = append(alg, "y' ");
                        run_algorithm(cube, "y'");
                        break;
                }
                edge = mod(edge - location, 4);
                location = 0;
            }
            //We know now that the corner is Front, up, right, and there's space below it.

            if (oriented)
            {
                switch (edge)
                {
                    case 0:
                        alg = append(alg, "F' U2 F U ");
                        run_algorithm(cube, "F' U2 F U ");
                        //ends up with a loose corner in U, spot 8 and the edge in U
                        alg2 = f2lCase8(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 1:
                        alg = append(alg, "R U' R' U ");
                        run_algorithm(cube, "R U' R' U ");
                        //ends up with a loose corner in U, spot 8, and the edge in U
                        alg2 = f2lCase7(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 2:
                        alg = append(alg, "F' U F U ");
                        run_algorithm(cube, "F' U F U ");
                        alg2 = f2lCase8(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 3:
                        alg = append(alg, "R U' R' U ");
                        run_algorithm(cube, "R U' R' U ");
                        alg2 = f2lCase7(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                }
            }
            else
            {
                switch (edge)
                {
                    case 0:
                        alg = append(alg, "F' U2 F U ");
                        run_algorithm(cube, "F' U2 F U ");
                        //ends up with a loose corner in U, spot 8, and the edge in U
                        alg2 = f2lCase8(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 1:
                        alg = append(alg, "R U' R' U ");
                        run_algorithm(cube, "R U' R' U ");
                        //ends up with a loose corner in U, spot 8, and the edge in U
                        alg2 = f2lCase7(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 2:
                        alg = append(alg, "F' U F U ");
                        run_algorithm(cube, "F' U F U ");
                        alg2 = f2lCase8(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                    case 3:
                        alg = append(alg, "R U' R' U ");
                        run_algorithm(cube, "R U' R' U ");
                        alg2 = f2lCase7(cube);
                        alg = append(alg, alg2);
                        free(alg2);
                        return alg;
                }
            }
        }
    }

    free(alg);
    return NULL;
}

char *f2lCase10(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //create int outside of the for loop so it's saved afterwards
    int location;
    for (location = 0; location < 4; location++)
    {
        if (cube[(location + 1) % 4][6] == 5)
        {
            break;
        }
    }
    //If we found a location before the break
    if (location < 4)
    {
        int d = -1;
        //first find the right d location for the corner so we can more easily find edges
        switch (location)
        {
            case 0:
                d = 2;
                break;
            case 1:
                d = 8;
                break;
            case 2:
                d = 6;
                break;
            case 3:
                d = 0;
                break;
        }
        int edge = -1;
        bool oriented;
        //first try to find an edge in the up slice. Oriented checks if [location][8] is equal to the up color.
        if (cube[U][7] == cube[location][8] && cube[F][1] == cube[D][d])
        {
            oriented = true;
            edge = 0;
        }
        else if (cube[U][7] == cube[D][d] && cube[F][1] == cube[location][8])
        {
            oriented = false;
            edge = 0;
        }
        else if (cube[U][5] == cube[location][8] && cube[R][1] == cube[D][d])
        {
            oriented = true;
            edge = 1;
        }
        else if (cube[U][5] == cube[D][d] && cube[R][1] == cube[location][8])
        {
            oriented = false;
            edge = 1;
        }
        else if (cube[U][1] == cube[location][8] && cube[B][1] == cube[D][d])
        {
            oriented = true;
            edge = 2;
        }
        else if (cube[U][1] == cube[D][d] && cube[B][1] == cube[location][8])
        {
            oriented = false;
            edge = 2;
        }
        else if (cube[U][3] == cube[location][8] && cube[L][1] == cube[D][d])
        {
            oriented = true;
            edge = 3;
        }
        else if (cube[U][3] == cube[D][d] && cube[L][1] == cube[location][8])
        {
            oriented = false;
            edge = 3;
        }
        //If we found an edge and it's oriented correctly
        if (edge != -1 && oriented)
        {
            //turn the cube so it's on location.
            switch (mod(edge - location, 4))
            {
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
                case 3:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
            }
            switch (location)
            {
                case 0:
                    alg = append(alg, "R U R' U' ");
                    run_algorithm(cube, "R U R' U'");
                    break;
                case 1:
                    alg = append(alg, "B U B' ");
                    run_algorithm(cube, "B U B'");
                    break;
                case 2:
                    alg = append(alg, "L U L' U ");
                    run_algorithm(cube, "L U L' U");
                    break;
                case 3:
                    alg = append(alg, "F U F' U2 ");
                    run_algorithm(cube, "F U F' U2");
                    break;
            }
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase2(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //If we found an edge and it's oriented incorrectly
        if (edge != -1 && oriented == false)
        {
            //turn U so the the edge is on location + 2
            switch (mod(edge - location + 2, 4))
            {
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
                case 3:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
            }
            switch (location)
            {
                case 0:
                    alg = append(alg, "R U2 R' U2 ");
                    run_algorithm(cube, "R U2 R' U2 ");
                    break;
                case 1:
                    alg = append(alg, "B U2 B' U' ");
                    run_algorithm(cube, "B U2 B' U' ");
                    break;
                case 2:
                    alg = append(alg, "L U2 L' ");
                    run_algorithm(cube, "L U2 L'");
                    break;
                case 3:
                    alg = append(alg, "F U2 F' U ");
                    run_algorithm(cube, "F U2 F' U");
                    break;
            }
            //ends up with a loose corner in U, spot 8, and the edge in U
            alg2 = f2lCase7(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //Now check for edges in the E slice
        for (edge = 0; edge < 4; edge++)
        {
            if (cube[edge][5] == cube[location][8] && cube[(edge + 1) % 4][3] == cube[D][d])
            {
                oriented = true;
                break;
            }
            if (cube[edge][5] == cube[D][d] && cube[(edge + 1) % 4][3] == cube[location][8])
            {
                oriented = false;
                break;
            }
        }
        //If we found one (and the loop didn't finish)
        if (edge < 4)
        {
            if (edge == location)
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F' U F U ");
                        run_algorithm(cube, "F' U F U");
                        break;
                    case 1:
                        alg = append(alg, "R' U R U ");
                        run_algorithm(cube, "R' U R U");
                        break;
                    case 2:
                        alg = append(alg, "B' U B U ");
                        run_algorithm(cube, "B' U B U");
                        break;
                    case 3:
                        alg = append(alg, "L' U L U ");
                        run_algorithm(cube, "L' U L U");
                        break;
                }
                //ends up with a loose corner in U, spot 8, and the edge in U
                alg2 = f2lCase7(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
            else if (oriented)
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F' U' F ");
                        run_algorithm(cube, "F' U' F");
                        break;
                    case 1:
                        alg = append(alg, "R' U' R ");
                        run_algorithm(cube, "R' U' R");
                        break;
                    case 2:
                        alg = append(alg, "B' U' B ");
                        run_algorithm(cube, "B' U' B");
                        break;
                    case 3:
                        alg = append(alg, "L' U' L ");
                        run_algorithm(cube, "L' U' L");
                        break;
                }
                //ends up with a corner pointing up, and an edge in E
                alg2 = f2lCase9(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
            else
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "R U R' U' R U R' ");
                        run_algorithm(cube, "R U R' U' R U R'");
                        break;
                    case 1:
                        alg = append(alg, "B U B' U' B U B' ");
                        run_algorithm(cube, "B U B' U' B U B'");
                        break;
                    case 2:
                        alg = append(alg, "L U L' U' L U L' ");
                        run_algorithm(cube, "L U L' U' L U L'");
                        break;
                    case 3:
                        alg = append(alg, "F U F' U' F U F' ");
                        run_algorithm(cube, "F U F' U' F U F'");
                        break;
                }
                //Ends up with the corner on the bottom layer, and the edge in the top layer.
                alg2 = f2lCase3(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
        }
    }
    free(alg);
    return NULL;
}

//corner oriented clockwise in bottom layer (pointing left in LDF)
char *f2lCase11(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    char *alg2;
    alg[0] = 0;
    //create int outside of the for loop so it's saved afterwards
    int location;
    for (location = 0; location < 4; location++)
    {
        if (cube[mod(location - 1, 4)][8] == 5)
        {
            break;
        }
    }
    //If we found a location before the break
    if (location < 4)
    {
        int d = -1;
        //first find the right d location for the corner so we can more easily find edges
        switch (location)
        {
            case 0:
                d = 0;
                break;
            case 1:
                d = 2;
                break;
            case 2:
                d = 8;
                break;
            case 3:
                d = 6;
                break;
        }
        int edge = -1;
        bool oriented;
        //first try to find an edge in the up slice. Oriented checks if [location][8] is equal to the up color.
        if (cube[U][7] == cube[location][6] && cube[F][1] == cube[D][d])
        {
            oriented = true;
            edge = 0;
        }
        else if (cube[U][7] == cube[D][d] && cube[F][1] == cube[location][6])
        {
            oriented = false;
            edge = 0;
        }
        else if (cube[U][5] == cube[location][6] && cube[R][1] == cube[D][d])
        {
            oriented = true;
            edge = 1;
        }
        else if (cube[U][5] == cube[D][d] && cube[R][1] == cube[location][6])
        {
            oriented = false;
            edge = 1;
        }
        else if (cube[U][1] == cube[location][6] && cube[B][1] == cube[D][d])
        {
            oriented = true;
            edge = 2;
        }
        else if (cube[U][1] == cube[D][d] && cube[B][1] == cube[location][6])
        {
            oriented = false;
            edge = 2;
        }
        else if (cube[U][3] == cube[location][6] && cube[L][1] == cube[D][d])
        {
            oriented = true;
            edge = 3;
        }
        else if (cube[U][3] == cube[D][d] && cube[L][1] == cube[location][6])
        {
            oriented = false;
            edge = 3;
        }
        //If we found an edge and it's oriented correctly
        if (edge != -1 && oriented)
        {
            //turn the cube so it's on location.
            switch (mod(edge - location, 4))
            {
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
                case 3:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
            }
            switch (location)
            {
                case 0:
                    alg = append(alg, "L' U' L ");
                    run_algorithm(cube, "L' U' L");
                    break;
                case 1:
                    alg = append(alg, "F' U' F ");
                    run_algorithm(cube, "F' U' F");
                    break;
                case 2:
                    alg = append(alg, "R' U' R ");
                    run_algorithm(cube, "R' U' R");
                    break;
                case 3:
                    alg = append(alg, "B' U' B ");
                    run_algorithm(cube, "B' U' B");
                    break;
            }
            //ends with a complete pair in the top layer, ready to be put into the f2l
            alg2 = f2lCase1(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //If we found an edge and it's oriented incorrectly
        if (edge != -1 && oriented == false)
        {
            //turn U so the the edge is on location + 2
            switch (mod(edge - location + 2, 4))
            {
                case 0:
                    alg = append(alg, "");
                    break;
                case 1:
                    alg = append(alg, "U ");
                    run_algorithm(cube, "U");
                    break;
                case 2:
                    alg = append(alg, "U2 ");
                    run_algorithm(cube, "U2");
                    break;
                case 3:
                    alg = append(alg, "U' ");
                    run_algorithm(cube, "U'");
                    break;
            }
            switch (location)
            {
                case 0:
                    alg = append(alg, "L' U2 L ");
                    run_algorithm(cube, "L' U2 L");
                    break;
                case 1:
                    alg = append(alg, "F' U2 F ");
                    run_algorithm(cube, "F' U2 F");
                    break;
                case 2:
                    alg = append(alg, "R' U2 R ");
                    run_algorithm(cube, "R' U2 R");
                    break;
                case 3:
                    alg = append(alg, "B' U2 B ");
                    run_algorithm(cube, "B' U2 B");
                    break;
            }
            //ends up with a loose corner in U, spot 0, and the edge in U
            alg2 = f2lCase8(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //Now check for edges in the E slice
        for (edge = 0; edge < 4; edge++)
        {
            if (cube[edge][3] == cube[location][6] && cube[mod(edge - 1, 4)][5] == cube[D][d])
            {
                oriented = true;
                break;
            }
            if (cube[edge][3] == cube[D][d] && cube[mod(edge - 1, 4)][5] == cube[location][6])
            {
                oriented = false;
                break;
            }
        }
        //If we found one (and the loop didn't finish)
        if (edge < 4)
        {
            if (edge == location)
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F U' F' ");
                        run_algorithm(cube, "F U' F'");
                        break;
                    case 1:
                        alg = append(alg, "R U' R' ");
                        run_algorithm(cube, "R U' R'");
                        break;
                    case 2:
                        alg = append(alg, "B U' B' ");
                        run_algorithm(cube, "B U' B'");
                        break;
                    case 3:
                        alg = append(alg, "L U' L' ");
                        run_algorithm(cube, "L U' L'");
                        break;
                }
                //ends up with a loose corner in U, spot 0, and the edge in U
                alg2 = f2lCase8(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
            else if (oriented)
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "F U F' ");
                        run_algorithm(cube, "F U F'");
                        break;
                    case 1:
                        alg = append(alg, "R U R' ");
                        run_algorithm(cube, "R U R'");
                        break;
                    case 2:
                        alg = append(alg, "B U B' ");
                        run_algorithm(cube, "B U B'");
                        break;
                    case 3:
                        alg = append(alg, "L U L' ");
                        run_algorithm(cube, "L U L'");
                        break;
                }
                //ends up with a corner pointing up, and an edge in E
                alg2 = f2lCase9(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
            else
            {
                switch (location)
                {
                    case 0:
                        alg = append(alg, "L' U' L U L' U' L ");
                        run_algorithm(cube, "L' U' L U L' U' L");
                        break;
                    case 1:
                        alg = append(alg, "F' U' F U F' U' F ");
                        run_algorithm(cube, "F' U' F U F' U' F");
                        break;
                    case 2:
                        alg = append(alg, "R' U' R U R' U' R ");
                        run_algorithm(cube, "R' U' R U R' U' R");
                        break;
                    case 3:
                        alg = append(alg, "B' U' B U B' U' B ");
                        run_algorithm(cube, "B' U' B U B' U' B");
                        break;
                }
                //Ends up with the corner on the bottom layer, and the edge in the top layer.
                alg2 = f2lCase3(cube);
                alg = append(alg, alg2);
                free(alg2);
                return alg;
            }
        }
    }
    free(alg);
    return NULL;
}


