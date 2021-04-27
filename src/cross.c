#include "cube.h"
#include "cross.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int findZero(int cube[6][9]);
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


//list of the bottom layer edges in order
int dSquares[] = {1, 5, 7, 3};
int uSquares[] = {7, 5, 1, 3};

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
        algs = append(algs, "z2 ");
        run_algorithm(cube, "z2");
    }
    else if (cube[R][4] == 5)
    {
        algs = append(algs, "z ");
        run_algorithm(cube, "z");
    }
    else if (cube[L][4] == 5)
    {
        algs = append(algs, "z' ");
        run_algorithm(cube, "z'");
    }
    else if (cube[F][4] == 5)
    {
        algs = append(algs, "x' ");
        run_algorithm(cube, "x'");
    }
    else if (cube[B][4] == 5)
    {
        algs = append(algs, "x ");
        run_algorithm(cube, "x");
    }
    int solved = 0;
    char *alg;
    for (int i = 0; i < 5; i++) //The cross should be able to be solved max 4 rounds. One edge per round.
    {
        //Yellow on the right side of the face
        alg = crossCase3(cube);
        if (alg)
        {
            //printf("%s\n", alg);
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
            //printf("%s\n", alg);
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

        //Yellow facing down
        alg = crossCase1(cube);
        if (alg)
        {
            //printf("%s\n", alg);
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
            //printf("%s\n", alg);
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
            //printf("%s\n", alg);
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
            //printf("%s\n", alg);
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
        //once we reach this point, the cross should be solved.
        break;
    }
    //We only have to twist the bottom layer so "relative" 0 is at 0;
    int zero = findZero(cube);
    switch (zero)
    {
        case 1:
            algs = append(algs, "D'");
            run_algorithm(cube, "D'");
            break;
        case 2:
            algs = append(algs, "D2");
            run_algorithm(cube, "D2");
            break;
        case 3:
            algs = append(algs, "D");
            run_algorithm(cube, "D");
            break;
    }
    //Verify if it's actually solved
    if (!(cube[D][1] == 5 && cube[F][7] == cube[F][4]) ||
        !(cube[D][3] == 5 && cube[L][7] == cube[L][4]) ||
        !(cube[D][5] == 5 && cube[R][7] == cube[R][4]) ||
        !(cube[D][7] == 5 && cube[B][7] == cube[B][4]))
    {
        //If not, return NULL
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

//Finds the location of the yellow/green edge. Returns 6 if there are no edges at all
int findZero(int cube[6][9])
{
    //init counter and array to hold yellow squares
    int ny = 0;
    int yellow[4];
    //Store the locations of every yellow down edge
    for (int i = 0; i < 4; i++)
    {
        if (cube[D][dSquares[i]] == 5)
        {
            yellow[ny] = i;
            ny++;
        }
    }
    //If there are none, return 6 to signify there are none
    if (ny == 0)
    {
        return 6;
    }
    //If there is one, return where the green square would be relative to the square we have
    //If there are two, return green relative to the first one, we do not have to check if they align.
    if (ny == 1 || ny == 2)
    {
        return mod(yellow[0] - cube[yellow[0]][7], 4);
    }
    //If there are three or four, check if two align, if so return green relative to those, otherwise return green relative to the first option
    if (ny == 3 || ny == 4)
    {
        //iterate over every square
        for (int i = 0; i < ny - 1; i++)
        {
            //it over every later square
            for (int j = i + 1; j < ny; j++)
            {
                //If the difference between the values is equal to the difference between the squares, return relative green
                if (mod(yellow[i] - yellow[j], 4) == mod(cube[yellow[i]][7] - cube[yellow[j]][7], 4))
                {
                    return mod(yellow[i] - cube[yellow[i]][7], 4);
                }
            }
        }
        //If iteration fails, just return relative to the first
        return mod(yellow[0] - cube[yellow[0]][7], 4);
    }
}

//Yellow facing down in wrong location
char *crossCase1(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    //Then we find zero
    int zero = findZero(cube);
    //If zero == 6, there are no squares facing down at all
    if (zero == 6)
    {
        free(alg);
        return NULL;
    }
    int found = 6;
    //Now we iterate over the possible edges to find misplaced squares.
    for (int i = 0; i < 4; i++)
    {
        //We only have to check if the edge has yellow on the bottom.
        if (cube[D][dSquares[i]] == 5)
        {
            //We calculate where 0 zould be compared to this cubie. If it isn't actual (relative) zero, it's wrong and we need to correct it.
            if (mod(i - cube[i][7], 4) != zero)
            {
                found = i;
                break;
            }
        }
    }
    if (found != 6)
    {
        //First we twist the incorrectly placed cubie out of the way
        switch (found)
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
        }
        //Generate where the new zero should be
        int newZero = mod(found - cube[found][7], 4);
        //Twist zero into position
        switch (mod(newZero - zero, 4))
        {
            case 1:
                alg = append(alg, "D ");
                break;
            case 2:
                alg = append(alg, "D2 ");
                break;
            case 3:
                alg = append(alg, "D' ");
        }
        //Twist the cubie back
        switch (found)
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
        }
        //And run the generated algorithm on the cube
        run_algorithm(cube, alg);
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow facing up.
char *crossCase2(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    //Then we find zero
    int zero = findZero(cube);

    //Now we iterate over the possible edges to find yellow squares.
    int found = 6;
    for (int i = 0; i < 4; i++)
    {
        //If we find a yellow square.
        if (cube[U][uSquares[i]] == 5)
        {
            found = i;
            break;
        }
    }
    //If we found one
    if (found != 6)
    {
        //If there are there are placed squares (if zero != 6), twist U so the yellow square is above it's place
        if (zero != 6)
        {
            //Find zero relative to the edge we have to place
            int rzero =  mod(found - cube[found][1], 4);
            switch (mod(rzero - zero, 4))
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
            //modify found to signify its new location
            found = mod(found - mod(rzero - zero, 4), 4);
        }
        //And twist it down
        switch (found)
        {
            case 0:
                alg = append(alg, "F2 ");
                break;
            case 1:
                alg = append(alg, "R2 ");
                break;
            case 2:
                alg = append(alg, "B2 ");
                break;
            case 3:
                alg = append(alg, "L2 ");
                break;
        }
        //And run the generated algorithm on the cube
        run_algorithm(cube, alg);
        return alg;
    }
    free(alg);
    return NULL;
}

//Next: Yellow on the right side of the face
char *crossCase3(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    //Then we find zero
    int zero = findZero(cube);
    int found = 6;
    //Then we iterate over the faces to find a yellow square
    for (int i = 0; i < 4; i++)
    {
        //If we find a yellow square.
        if (cube[i][5] == 5)
        {
            //If yellow is at i, the face we have to move is i+1;
            found = mod(i + 1, 4);
            break;
        }
    }
    //If we found one
    if (found != 6)
    {
        //First find 0 relative to found;
        int newZero = mod(found - cube[found][3], 4);

        //Twist zero into position
        switch (mod(newZero - zero, 4))
        {
            case 1:
                alg = append(alg, "D ");
                break;
            case 2:
                alg = append(alg, "D2 ");
                break;
            case 3:
                alg = append(alg, "D' ");
        }
        //Twist the cubie in place
        switch (found)
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
        }
        //And run the generated algorithm on the cube
        run_algorithm(cube, alg);
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the left side of the face
char *crossCase4(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    //Then we find zero
    int zero = findZero(cube);
    int found = 6;
    //Then we iterate over the faces to find a yellow square
    for (int i = 0; i < 4; i++)
    {
        //If we find a yellow square.
        if (cube[i][3] == 5)
        {
            //If yellow is at i, the face we have to move is i+1;
            found = mod(i - 1, 4);
            break;
        }
    }
    //If we found one
    if (found != 6)
    {
        //First find 0 relative to found;
        int newZero = mod(found - cube[found][5], 4);

        //Twist zero into position
        switch (mod(newZero - zero, 4))
        {
            case 1:
                alg = append(alg, "D ");
                break;
            case 2:
                alg = append(alg, "D2 ");
                break;
            case 3:
                alg = append(alg, "D' ");
        }
        //Twist the cubie in place
        switch (found)
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
        }
        //And run the generated algorithm on the cube
        run_algorithm(cube, alg);
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the sides of the down layer
char *crossCase5(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    int found = 6;
    //Then we iterate over the faces to find a yellow square
    for (int i = 0; i < 4; i++)
    {
        //If we find a yellow square.
        if (cube[i][7] == 5)
        {
            found = i;
            break;
        }
    }
    //If we found one
    if (found != 6)
    {
        //We only have to twist the cubie up and then run case 3
        switch (found)
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
        run_algorithm(cube, alg);
        char *alg2 = crossCase4(cube);
        alg = append(alg, alg2);
        free(alg2);
        return alg;
    }
    free(alg);
    return NULL;
}

//Yellow on the sides of the up layer
char *crossCase6(int cube[6][9])
{
    //First we need a place to store the algorithm
    char *alg = malloc(1);
    alg[0] = 0;
    int zero = findZero(cube);
    int found = 6;
    //Then we iterate over the faces to find a yellow square
    for (int i = 0; i < 4; i++)
    {
        //If we find a yellow square.
        if (cube[i][1] == 5)
        {
            found = i;
            break;
        }
    }
    //If we found one
    if (found != 6)
    {
        //If the cubie below it is empty
        if (cube[D][dSquares[found]] != 5)
        {
            //We only have to twist the cubie down and then run case 3
            switch (found)
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
            run_algorithm(cube, alg);
            char *alg2 = crossCase4(cube);
            alg = append(alg, alg2);
            free(alg2);
            return alg;
        }
        //If the below cubie is not empty
        //TODO: recalculate newZero because it's wrong
        int newZero = mod(found - cube[4][uSquares[found]] - 1, 4);
        //Twist zero into position
        switch (mod(newZero - zero, 4))
        {
            case 1:
                alg = append(alg, "D ");
                break;
            case 2:
                alg = append(alg, "D2 ");
                break;
            case 3:
                alg = append(alg, "D' ");
        }
        switch (found)
        {
            case 0:
                alg = append(alg, "F' L F ");
                break;
            case 1:
                alg = append(alg, "R' F R ");
                break;
            case 2:
                alg = append(alg, "B' R B ");
                break;
            case 3:
                alg = append(alg, "L' B L ");
                break;
        }
        //And run the generated algorithm on the cube
        run_algorithm(cube, alg);
        return alg;
    }
    free(alg);
    return NULL;
}