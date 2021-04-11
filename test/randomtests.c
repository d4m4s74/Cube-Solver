#include <cubesolver.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//These helper functions are in libcubesolver but not in cubesolver.h
char *append(char *a, char *b);
bool isNumber(char *s);

//The four solved states
const int solvedcube1[6][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3, 3, 3, 3, 3},
    {4, 4, 4, 4, 4, 4, 4, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5, 5}
};
const int solvedcube2[6][9] = {{3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2, 2},
    {4, 4, 4, 4, 4, 4, 4, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5, 5}
};
const int solvedcube3[6][9] = {{2, 2, 2, 2, 2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {4, 4, 4, 4, 4, 4, 4, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5, 5}
};
const int solvedcube4[6][9] = {{1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 4, 4, 4, 4, 4, 4, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5, 5}
};

//Compares an entered cube with known correct states.
bool checkSolved(int cube[6][9])
{
    if (memcmp(cube, solvedcube1, sizeof(int) * 6 * 9) == 0)
    {
        return true;
    }
    if (memcmp(cube, solvedcube2, sizeof(int) * 6 * 9) == 0)
    {
        return true;
    }
    if (memcmp(cube, solvedcube3, sizeof(int) * 6 * 9) == 0)
    {
        return true;
    }
    if (memcmp(cube, solvedcube4, sizeof(int) * 6 * 9) == 0)
    {
        return true;
    }
    return false;
}

//Generates a random scramble of a specifies length. Does not protect against double moves or inverted moves.
char *generate_scramble(int length)
{
    char *alg = malloc(length * 5);
    alg[0] = 0;
    //The possible moves on a cube
    char moves[18][4] = {"F ", "F2 ", "F' ", "R ", "R2 ", "R' ", "B ", "B2 ", "B' ", "L ", "L2 ", "L' ",  "U ", "U2 ", "U' ", "D ", "D2 ", "D' "};
    for (int i = 0; i < length; i++)
    {
        int r = rand() % 18;      // Returns a pseudo-random integer between 0 and 18
        alg = append(alg, moves[r]);
    }
    return alg;    
}

int main(int argc, char *argv[])
{
    srand(time(NULL));   // initialize random.
    //create a variable for a cube
    int cube[6][9];
    if (argc != 3 || !isNumber(argv[2]))
    {
        printf("Usage: %s [path/to/csv/files] [num_tests]\n", argv[0]);
        return 1;
    }
    if (!setup(argv[1]))
    {
        return 2;
    }
    int numtests = atoi(argv[2]);
    printf("Solving %i random scrambles\n", numtests);
    for (int i = 1; i <= numtests; i++)
    {
        //copy a solved cube into cube.
        memcpy(cube, solvedcube1, sizeof(int) * 6 * 9);
        char *scramble = generate_scramble(50);
        run_algorithm(cube, scramble);
        char *alg = NULL;
        if ((alg = solve_cross(cube)) == NULL)
        {
            printf("Error in Cross algorithm\n Scramble: %s\n", scramble);
            print_cube(cube);
            free(scramble);
            return 1;
        }
        free(alg);
        if ((alg = solve_f2l(cube)) == NULL)
        {
            printf("Error in F2L algorithm\n Scramble: %s\n", scramble);
            print_cube(cube);
            free(scramble);
            return 1;
        }
        free(alg);
        if ((alg = solve_oll(cube)) == NULL)
        {
            printf("Error in OLL algorithm\n Scramble: %s\n", scramble);
            print_cube(cube);
            free(scramble);
            return 1;
        }
        free(alg);
        if ((alg = solve_pll(cube)) == NULL)
        {
            printf("Error in PLL algorithm\n Scramble: %s\n", scramble);
            print_cube(cube);
            free(scramble);
            return 1;
        }
        free(alg);
        if (checkSolved(cube) == false)
        {
            printf("Cube not solved\n Scramble: %s\n", scramble);
            print_cube(cube);
            free(scramble);
            return 1;
        }
        free(scramble);
    }
    cleanup_last_layer();
    printf("Correctly solved %i random scrambles\n", numtests);
}