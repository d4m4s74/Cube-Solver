#include "lastlayer.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char *name;
    //The colours in the upper ring, starting with front, ending with left.
    int pattern[12];
    char *algorithm;
} pll;

typedef struct
{
    char *name;
    //The colours in the upper ring, starting with front, ending with left.
    //Saved as yellow or not yellow
    bool pattern[12];
    char *algorithm;
} oll;

//Arrays for patterns and algorithms for PLLs and OLLs. There are 21 possible plls and 57 OLLs
pll plls[22];
oll olls[58];

//Loads the OLLs from a file into memory
bool load_olls(char *filename)
{
    //preparing read file
    FILE *fp;
    fp = fopen(filename, "r");
    //If you can't open the file, return false to indicate failure
    if (fp == NULL)
    {
        printf("Can't open file %s\n", filename);
        return false;
    }
    //create strings to save the items
    char name[30];
    char pattern[13];
    char algorithm[100];
    char buffer[201];
    //We're going to get 58 OLLs
    for (int i = 0; i < 58; i++)
    {
        //Get the next line of the file
        if (fgets(buffer, 200, fp))
        {
            if (buffer[0] == '#')
            {
                //If the line is a comment
                i--;
                continue;
            }
            //To stop potential out of bound writes or reads, let's check the lengths
            char *tmp = strchr(buffer, ',');
            char *tmp2 = strrchr(buffer, ',');
            if (tmp - buffer > 29 || tmp2 - tmp != 13 || buffer + strlen(buffer) - tmp2 > 100)
            {
                //Name, pattern or algorithm too long. Just ignore it and continue
                i--;
                continue;
            }
            //Because some casese can have an empty algorithm
            algorithm[0] = 0;
            //scan the name, pattern and algorithm.
            sscanf(buffer, "%[^,], %[^,], %[^,\n]\n", name, pattern, algorithm);
            //check if the pattern is all ones and zeroes and the right length
            if (!isBinary(pattern) || strlen(pattern) != 12)
            {
                //If not, go to the next line without iterating
                i--;
                continue;
            }
            //make space for the name and copy it in
            olls[i].name = (char *)malloc(strlen(name) + 1);
            if (olls[i].name == NULL)
            {
                return false;
            }
            strcpy(olls[i].name, name);
            //make space for the alg and copy it in
            olls[i].algorithm = (char *)malloc(strlen(algorithm) + 1);
            if (olls[i].algorithm == NULL)
            {
                return false;
            }
            strcpy(olls[i].algorithm, algorithm);
            //turn the pattern into a bool array (yellow or not yellow)
            for (int j = 0; j < 12; j++)
            {
                olls[i].pattern[j] = (pattern[j] == '1');
            }
        }
        else
        {
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

//Loads the PLLs from a file into memory
bool load_plls(char *filename)
{
    //preparing read file
    FILE *fp;
    fp = fopen(filename, "r");
    //If you can't open the file, return false to indicate failure
    if (fp == NULL)
    {
        printf("Can't open file %s\n", filename);
        return false;
    }
    //create strings to save the items
    char name[30];
    char pattern[13];
    char algorithm[100];
    char buffer[200];
    //We're going to get 22 PLLs
    for (int i = 0; i < 22; i++)
    {
        //get the next line
        if (fgets(buffer, 200, fp))
        {
            if (buffer[0] == '#')
            {
                //If the line is a comment
                i--;
                continue;
            }

            //To stop potential out of bound writes or reads, let's check the lengths
            char *tmp = strchr(buffer, ',');
            char *tmp2 = strrchr(buffer, ',');
            if (tmp - buffer > 29 || tmp2 - tmp != 13 || buffer + strlen(buffer) - tmp2 > 100)
            {
                //Name, pattern or algorithm too long. Just ignore it and continue
                i--;
                continue;
            }
            //Because some casese can have an empty algorithm
            algorithm[0] = 0;
            //scan the name, pattern and algorithm.
            sscanf(buffer, "%[^,], %[^,], %[^,\n]\n", name, pattern, algorithm);
            //check if pattern is purely numerical and the right length
            if (!isNumber(pattern) || strlen(pattern) != 12)
            {
                //If not, go to the next line without iterating
                i--;
                continue;
            }
            //make space for the name and copy it in
            plls[i].name = (char *)malloc(strlen(name) + 1);
            if (plls[i].name == NULL)
            {
                return false;
            }
            strcpy(plls[i].name, name);
            //make space for the alg and copy it in
            plls[i].algorithm = (char *)malloc(strlen(algorithm) + 1);
            if (plls[i].algorithm == NULL)
            {
                return false;
            }
            strcpy(plls[i].algorithm, algorithm);
            //turn the pattern into an int array
            for (int j = 0; j < 12; j++)
            {
                plls[i].pattern[j] = pattern[j] - '0';
            }
        }
        else
        {
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

/*
Analyses the cube and finds the right OLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of U moves to do before
*/
pair_int_int find_oll(int cube[6][9])
{
    //Then create an array for the pattern around the edge of the cube, starting with each face.
    bool patterns[4][12];
    //Loop over all places in the array
    for (int i = 0; i < 12; i++)
    {
        //starting with each face (0 being front, 3 being left)
        for (int j = 0; j < 4; j++)
        {
            //Save the pattern of yellow or not yellow
            patterns[j][i] = (cube[(j + i / 3) % 4][i % 3] == 4);
        }
    }
    //For each possible OLL
    for (int i = 0; i < 58; i++)
    {
        //Check every pattern
        for (int j = 0; j < 4; j++)
        {
            if (memcmp(patterns[j], olls[i].pattern, sizeof(patterns[j])) == 0)
            {
                return make_pair_int_int(i, j);
            }
        }
    }
    //If no OLL is found, return -1;
    return make_pair_int_int(-1, -1);
}

/*
Analyses the cube and finds the right PLL algorithm.
Returns pair_int_int of the right algorithm number, and the amount of U moves to do before
*/
pair_int_int find_pll(int cube[6][9])
{
    for (int i = 0; i < 9; i++)
    {
        if (cube[4][i] != 4)
        {
            //If the rest of the cube isn't solved yet, return -2
            return make_pair_int_int(-2, -2);
        }
    }
    //first make an array containing the difference between 0 and the first piece on the face
    int dif[4];
    dif[0] = 0 - cube[0][0];
    dif[1] = 0 - cube[1][0];
    dif[2] = 0 - cube[2][0];
    dif[3] = 0 - cube[3][0];
    //Then create an array for the pattern around the edge of the cube, starting with each face.
    int patterns[4][12];
    //Loop over all places in the array
    for (int i = 0; i < 12; i++)
    {
        //starting with each face (0 being front, 3 being left)
        for (int j = 0; j < 4; j++)
        {
            //Save the pattern, calculating it so the first digit in the array is always 0.
            patterns[j][i] = mod((cube[(j + i / 3) % 4][i % 3] + dif[j]), 4);
        }
    }
    //For each possible PLL
    for (int i = 0; i < 22; i++)
    {
        //Check every pattern
        for (int j = 0; j < 4; j++)
        {
            if (memcmp(patterns[j], plls[i].pattern, sizeof(patterns[j])) == 0)
            {
                return make_pair_int_int(i, j);
            }
        }
    }

    //If no PLL is found, return -1;
    return make_pair_int_int(-1, -1);
}

//Looks up the right OLL algorithm and runs it
char *solve_oll(int cube[6][9])
{
    pair_int_int steps = find_oll(cube);
    if (steps.first < 0)
    {
        return NULL;
    }
    char *alg = malloc(1);
    if (alg == NULL)
    {
        return NULL;
    }
    alg[0] = 0;
    alg = append(alg, olls[steps.first].name);
    alg = append(alg, "\n");
    if (steps.second == 1)
    {
        alg = append(alg, "(y) ");
        run_algorithm(cube, "y");
    }
    else if (steps.second == 2)
    {
        alg = append(alg, "(y2) ");
        run_algorithm(cube, "y2");
    }
    else if (steps.second == 3)
    {
        alg = append(alg, "(y') ");
        run_algorithm(cube, "y'");
    }
    alg = append(alg, olls[steps.first].algorithm);
    run_algorithm(cube, olls[steps.first].algorithm);
    return alg;
}

//Looks up the right PLL algorithm and runs it
char *solve_pll(int cube[6][9])
{
    pair_int_int steps = find_pll(cube);
    if (steps.first < 0)
    {
        return NULL;
    }
    char *alg = malloc(1);
    if (alg == NULL)
    {
        return NULL;
    }
    alg[0] = 0;
    alg = append(alg, plls[steps.first].name);
    alg = append(alg, "\n");
    if (steps.second == 1)
    {
        alg = append(alg, "(y) ");
        run_algorithm(cube, "y");
    }
    else if (steps.second == 2)
    {
        alg = append(alg, "(y2) ");
        run_algorithm(cube, "y2");
    }
    else if (steps.second == 3)
    {
        alg = append(alg, "(y') ");
        run_algorithm(cube, "y'");
    }
    if (strcmp(plls[steps.first].algorithm, "") != 0)
    {
        alg = append(alg,  plls[steps.first].algorithm);
        run_algorithm(cube, plls[steps.first].algorithm);
    }
    int lastmove = mod(cube[0][3] - cube[0][0], 4);
    if (lastmove == 1)
    {
        alg = append(alg, " (U)");
        move_cube(cube, U, 1);
    }
    else if (lastmove == 2)
    {
        alg = append(alg, " (U2)");
        move_cube(cube, U, 2);
    }
    else if (lastmove == 3)
    {
        alg = append(alg, " (U')");
        move_cube(cube, U, 3);
    }
    return alg;
}

//free all the memory used by the OLLs and PLLs.
void cleanup_last_layer()
{
    for (int i = 0; i < 58; i++)
    {
        free(olls[i].name);
        free(olls[i].algorithm);
    }
    for (int i = 0; i < 22; i++)
    {
        free(plls[i].name);
        free(plls[i].algorithm);
    }
}