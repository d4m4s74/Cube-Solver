#include "cube.h"
#include "cross.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Yellow facing down in wrong location
char *crossCase1(int cube[6][9]);
//Yellow facing up.
char *crossCase2(int cube[6][9]);
//Next: Yellow on the right side of the face
char *crossCase3(int cube[6][9]);
//Yellow on the left side of the face
char *crossCase4(int cube[6][9]);
//Yellow on the sides of the down layer
char *crossCase5(int cube[6][9]);
//Yellow on the sides of the up layer
char *crossCase6(int cube[6][9]);

/*
A function to solve the cross.
No alorithms this time, so it'll be a huge decision tree.
Based on my own methods
*/
char *solve_cross(int cube[6][9])
{
    char *algs = malloc(1);
    algs[0] = 0;
    //First orient the cube with the yellow center pointing down.
    if (cube[U][4] == 5)
    {
        algs = append(algs, "z2\n");
        run_algorithm(cube, "z2");
    }
    else if (cube[R][4] == 5)
    {
        algs = append(algs, "z\n");
        run_algorithm(cube, "z");
    }
    else if (cube[L][4] == 5)
    {
        algs = append(algs, "z'\n");
        run_algorithm(cube, "z'");
    }
    else if (cube[F][4] == 5)
    {
        algs = append(algs, "x'\n");
        run_algorithm(cube, "x'");
    }
    else if (cube[B][4] == 5)
    {
        algs = append(algs, "x\n");
        run_algorithm(cube, "x");
    }
    int solved = 0;
    char *alg;
    for (int i = 0; i < 4; i++) //The cross should be able to be solved max 4 rounds. One edge per round.
    {
        //print_cube(); //for debugging purposes
        //Reset solved every round.
        solved = (cube[D][1] == 5 && cube[F][7] == cube[F][4]) +
                 (cube[D][3] == 5 && cube[L][7] == cube[L][4]) +
                 (cube[D][5] == 5 && cube[R][7] == cube[R][4]) +
                 (cube[D][7] == 5 && cube[B][7] == cube[B][4]);
        if (solved == 4)
        {
            break;
        }
        //Yellow facing down
        alg = crossCase1(cube);
        if (alg)
        {
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
        //Yellow facing up.
        alg = crossCase2(cube);
        if (alg)
        {
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
        //Yellow on the right side of the face
        alg = crossCase3(cube);
        if (alg)
        {
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
        //Yellow on the left side of the face
        alg = crossCase4(cube);
        if (alg)
        {
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
        //Yellow on the sides of the down layer
        alg = crossCase5(cube);
        if (alg)
        {
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
        //Yellow on the sides of the up layer
        alg = crossCase6(cube);
        if (alg)
        {
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


//Yellow facing down in wrong location.
char *crossCase1(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    int solved = (cube[D][1] == 5 && cube[F][7] == cube[F][4]) +
                 (cube[D][3] == 5 && cube[L][7] == cube[L][4]) +
                 (cube[D][5] == 5 && cube[R][7] == cube[R][4]) +
                 (cube[D][7] == 5 && cube[B][7] == cube[B][4]);
    //If there is a Yellow edge on the bottom facing front but not in place
    if ((cube[D][1] == 5 && cube[F][7] != cube[F][4]))
    {
        //If this is the first solved, just rotate the down face
        int correction = mod(cube[F][7] - cube[F][4], 4);
        if (solved == 0)
        {
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "D\n");
                    break;
                case 2:
                    alg = append(alg, "D2\n");
                    break;
                case 3:
                    alg = append(alg, "D'\n");
                    break;
            }
        }
        //Otherwise, move the piece out of the way, rotate down slice under it, place the piece back and move down back
        else
        {
            move_cube(cube, F, 1);
            move_cube(cube, D, 4 - correction);
            move_cube(cube, F, 3);
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "F D' F' D\n");
                    break;
                case 2:
                    alg = append(alg, "F D2 F' D2\n");
                    break;
                case 3:
                    alg = append(alg, "F D F' D'\n");
                    break;
            }
        }
        //check again from the start
        return alg;
    }
    //If there is a Yellow edge on the bottom facing right but not in place
    if ((cube[D][5] == 5 && cube[R][7] != cube[R][4]))
    {
        //If this is the first solved, just rotate the down face
        int correction = mod(cube[R][7] - cube[R][4], 4);
        if (solved == 0)
        {
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "D\n");
                    break;
                case 2:
                    alg = append(alg, "D2\n");
                    break;
                case 3:
                    alg = append(alg, "D'\n");
                    break;
            }
        }
        //Otherwise, move the piece out of the way, rotate down slice under it, place the piece back and move down back
        else
        {
            move_cube(cube, R, 1);
            move_cube(cube, D, 4 - correction);
            move_cube(cube, R, 3);
            move_cube(cube, D, correction);

            switch (correction)
            {
                case 1:
                    alg = append(alg, "R D' R' D\n");
                    break;
                case 2:
                    alg = append(alg, "R D2 R' D2\n");
                    break;
                case 3:
                    alg = append(alg, "R D R' D'\n");
                    break;
            }
        }
        return alg;
    }
    //If there is a Yellow edge on the bottom facing left but not in place
    if ((cube[D][3] == 5 && cube[L][7] != cube[L][4]))
    {
        //If this is the first solved, just rotate the down face
        int correction = mod(cube[L][7] - cube[L][4], 4);
        if (solved == 0)
        {
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "D\n");
                    break;
                case 2:
                    alg = append(alg, "D2\n");
                    break;
                case 3:
                    alg = append(alg, "D'\n");
                    break;
            }
        }
        //Otherwise, move the piece out of the way, rotate down slice under it, place the piece back and move down back
        else
        {
            move_cube(cube, L, 3);
            move_cube(cube, D, 4 - correction);
            move_cube(cube, L, 1);
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "L' D' L D\n");
                    break;
                case 2:
                    alg = append(alg, "L' D2 L D2\n");
                    break;
                case 3:
                    alg = append(alg, "L' D L D'\n");
                    break;
            }
        }
        return alg;
    }
    //If there is a Yellow edge on the bottom facing back but not in place
    if ((cube[D][7] == 5 && cube[B][7] != cube[B][4]))
    {
        //If this is the first solved, just rotate the down face
        int correction = mod(cube[B][7] - cube[B][4], 4);
        if (solved == 0)
        {
            move_cube(cube, D, correction);
            switch (correction)
            {
                case 1:
                    alg = append(alg, "D\n");
                    break;
                case 2:
                    alg = append(alg, "D2\n");
                    break;
                case 3:
                    alg = append(alg, "D'\n");
                    break;
            }
        }
        //Otherwise, move the piece out of the way, rotate down slice under it, place the piece back and move down back
        else
        {
            move_cube(cube, B, 3);
            move_cube(cube, D, 4 - correction);
            move_cube(cube, B, 1);
            move_cube(cube, D, correction);

            switch (correction)
            {
                case 1:
                    alg = append(alg, "B' D' B D\n");
                    break;
                case 2:
                    alg = append(alg, "B' D2 B D2\n");
                    break;
                case 3:
                    alg = append(alg, "B' D B D'\n");
                    break;
            }
        }
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow facing up.
char *crossCase2(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    //The plan: Find Yellow facing up, find where is it supposed to go, rotate U and rotate the correct face.
    int found = -1;
    int correction = -1;
    //Check for Yellow facing up on the front edge
    if (cube[U][7] == 5)
    {
        //Correction is how far we have to turn the up slice to align the piece with it's place
        correction = mod(cube[F][4] - cube[F][1], 4);
        //Found is where the piece should be.
        found = mod(F - correction, 4);
    }
    //Yellow facing up on left edge
    else if (cube[U][3] == 5)
    {
        correction = mod(cube[L][4] - cube[L][1], 4);
        found = mod(L - correction, 4);
    }
    //Right edge
    else if (cube[U][5] == 5)
    {
        correction = mod(cube[R][4] - cube[R][1], 4);
        found = mod(R - correction, 4);
    }
    //Back edge
    else if (cube[U][1] == 5)
    {
        correction = mod(cube[B][4] - cube[B][1], 4);
        found = mod(B - correction, 4);
    }
    if (found != -1)
    {
        //First move the U to align te piece
        move_cube(cube, U, correction);
        //Then rotate it in.
        move_cube(cube, found, 2);
        switch (correction)
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
        }
        switch (found)
        {
            case 0:
                alg = append(alg, "F2\n");
                break;
            case 1:
                alg = append(alg, "R2\n");
                break;
            case 2:
                alg = append(alg, "B2\n");
                break;
            case 3:
                alg = append(alg, "L2\n");
        }
        return alg;
    }
    free(alg);
    return NULL;
}

//Next: Yellow on the right side of the face
char *crossCase3(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    //The plan: Rotate the cube so the edge to put in is always F[5], Rotate B under it, and put it in.
    int found = -1;
    int correction = -1;
    if (cube[F][5] == 5)
    {
        found = 0;
    }
    else if (cube[R][5] == 5)
    {
        found = 1;
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
    }
    else if (cube[B][5] == 5)
    {
        found = 2;
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
    }
    else if (cube[L][5] == 5)
    {
        found = 3;
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
    }
    if (found != -1)
    {
        correction = mod(cube[R][4] - cube[R][3], 4);
        move_cube(cube, D, correction);
        move_cube(cube, R, 3);
        move_cube(cube, D, 4 - correction);
        switch (correction)
        {
            case 0:
                alg = append(alg, "R'\n");
                break;
            case 1:
                alg = append(alg, "D R' D'\n");
                break;
            case 2:
                alg = append(alg, "D2 R' D2\n");
                break;
            case 3:
                alg = append(alg, "D' R' D\n");
                break;
        }
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the left side of the face
char *crossCase4(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    int found = -1;
    int correction = -1;
    //The plan: Rotate the cube so the edge to put in is always F[5], Rotate B under it, and put it in.
    if (cube[F][3] == 5)
    {
        found = 0;
    }
    else if (cube[R][3] == 5)
    {
        found = 1;
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
    }
    else if (cube[B][3] == 5)
    {
        found = 2;
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
    }
    else if (cube[L][3] == 5)
    {
        found = 3;
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
    }
    if (found != -1)
    {
        correction = mod(cube[L][4] - cube[L][5], 4);
        move_cube(cube, D, correction);
        move_cube(cube, L, 1);
        move_cube(cube, D, 4 - correction);
        switch (correction)
        {
            case 0:
                alg = append(alg, "L\n");
                break;
            case 1:
                alg = append(alg, "D L D'\n");
                break;
            case 2:
                alg = append(alg, "D2 L D2\n");
                break;
            case 3:
                alg = append(alg, "D' L D\n");
                break;
        }
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the sides of the down layer
char *crossCase5(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    int found = -1;
    int correction = -1;
    //Yellow facing out on the down right edge
    if (cube[R][7] == 5)
    {
        found = 0;
    }
    //Yellow facing out on the down back edge
    else if (cube[B][7] == 5)
    {
        found = 1;
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
    }
    //Yellow facing out on the down left edge
    else if (cube[L][7] == 5)
    {
        found = 2;
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
    }
    //Yellow facing out on the down front edge
    else if (cube[F][7] == 5)
    {
        found = 3;
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
    }
    if (found != -1)
    {
        correction = mod(cube[F][4] - cube[D][5], 4);
        move_cube(cube, R, 1);
        move_cube(cube, D, correction);
        move_cube(cube, F, 1);
        move_cube(cube, D, 4 - correction);
        switch (correction)
        {
            case 0:
                alg = append(alg, "R F\n");
                break;
            case 1:
                alg = append(alg, "R D F D'\n");
                break;
            case 2:
                alg = append(alg, "R D2 F D2\n");
                break;
            case 3:
                alg = append(alg, "R D' F D\n");
                break;
        }
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the sides of the up layer
char *crossCase6(int cube[6][9])
{
    char *alg = malloc(1);
    if (alg == NULL)
    {
        //memory error
        return NULL;
    }
    alg[0] = 0;
    int found = -1;
    int correction = -1;
    //Yellow facing out on the up right edge
    if (cube[R][1] == 5)
    {
        found = 0;
    }
    //Yellow facing out on the up back edge
    else if (cube[B][1] == 5)
    {
        found = 1;
        alg = append(alg, "y ");
        run_algorithm(cube, "y");
    }
    //Yellow facing out on the up left edge
    else if (cube[L][1] == 5)
    {
        found = 2;
        alg = append(alg, "y2 ");
        run_algorithm(cube, "y2");
    }
    //Yellow facing out on the up front edge
    else if (cube[F][1] == 5)
    {
        found = 3;
        alg = append(alg, "y' ");
        run_algorithm(cube, "y'");
    }
    if (found != -1)
    {
        correction = mod(cube[F][4] - cube[U][5], 4);
        move_cube(cube, R, 3);
        move_cube(cube, D, correction);
        move_cube(cube, F, 1);
        move_cube(cube, D, 4 - correction);
        if (correction != 3)
        {
            move_cube(cube, R, 1);
        }
        switch (correction)
        {
            case 0:
                alg = append(alg, "R' F R\n");
                break;
            case 1:
                alg = append(alg, "R' D F D' R\n");
                break;
            case 2:
                alg = append(alg, "R' D2 F D2 R\n");
                break;
            case 3:
                alg = append(alg, "R' D' F D\n");
                break;
        }
        return alg;
    }
    free(alg);
    return NULL;
}

