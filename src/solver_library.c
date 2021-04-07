#include "utils.h"
#include "cube.h"
#include "cross.h"
#include "f2l.h"
#include "lastlayer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//returns string containing all algorithms used to solve the cube
char *solve(int cube[6][9]);
//Generates a cube from an algorithm and returns its solution
char *solve_scramble(char *scramble);
/*
Load all OLLs and PLLs into memory
path is the folder in which the csv files are located
*/
bool setup(char *path);
bool issetup;

//The default filenames for the PLL and OLL files
char *pllFileName = "plls.csv";
char *ollFileName = "olls.csv";

//Generates a cube from an algorithm and returns its solution
char *solve_scramble(char *scramble)
{
    int cube[6][9];
    color_cube(cube, "000000000", "111111111", "222222222", "333333333", "444444444", "555555555");
    run_algorithm(cube, scramble);
    return solve(cube);
}

//returns string containing all algorithms used to solve the cube
char *solve(int cube[6][9])
{
    //If the algorithms aren't already loaded, return error
    if (!issetup)
    {
        cleanup_last_layer();
        return "OLLs and PLLs not loaded";
    }
    if (!validate(cube))
    {
        return "Invalid color combination";
    }
    char *algs = malloc(1);
    algs[0] = 0;
    //print_cube(cube);
    char *cross = solve_cross(cube);
    if (cross)
    {  
        algs = append(algs, "Cross\n");
        algs = append(algs, cross);

        free(cross);
    }
    else
    {
        free(algs);
        cleanup_last_layer();
        return "Bug in cross algorithm.\n";
    }
    //print_cube(cube);
    algs = append(algs, "\n");
    char *f2l = solve_f2l(cube);
    if (f2l)
    {
        algs = append(algs, "\nF2L\n");
        algs = append(algs, f2l);
        free(f2l);
    }
    else
    {
        free(algs);
        cleanup_last_layer();
        return "Bug in f2l algorithm.\n";
    }
    //print_cube(cube);
    algs = append(algs, "\n");
    char *oll = solve_oll(cube);
    if (oll)
    {
        algs = append(algs, "\nOLL: \n");
        algs = append(algs, oll);
        algs = append(algs, "\n");
        free(oll);
    }
    else
    {
        free(algs);
        cleanup_last_layer();
        return "OLL Parity.\n";
    }
    //print_cube(cube);
    algs = append(algs, "\n");
    char *pll = solve_pll(cube);
    if (pll)
    {
        algs = append(algs, "\nPLL: \n");
        algs = append(algs, pll);
        algs = append(algs, "\n");
        free(pll);
    }
    else
    {
        free(algs);
        cleanup_last_layer();
        return "PLL Parity\n";
    }
    return algs;
}

/*
Load all OLLs and PLLs into memory
path is the folder in which the csv files are located
*/
bool setup(char *path)
{
    if (strlen(path) == 0)
    {
        path = ".";
    }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char slash = '\\';
#else
    char slash =  '/';
#endif
    if (path[strlen(path)-1] == slash)
    {
        path[strlen(path)-1] = 0;
    }
    //Make a string with the full path of the plls file
    char *pllfile = malloc(strlen(path) + strlen(pllFileName) + 2);
    if (pllfile == NULL)
    {
        return false;
    }
    snprintf(pllfile, strlen(path) + strlen(pllFileName) + 2, "%s%c%s", path, slash, pllFileName);
    //Make a string with the full path of the olls file
    char *ollfile = malloc(strlen(path) + strlen(ollFileName) + 2);
    if (ollfile == NULL)
    {
        return false;
    }
    snprintf(ollfile, strlen(path) + strlen(ollFileName) + 2, "%s%c%s", path, slash, ollFileName);
    //finally load olls and plls
    if (!load_olls(ollfile))
    {
        free(pllfile);
        free(ollfile);
        return false;
    }
    if (!load_plls(pllfile))
    {
        free(pllfile);
        free(ollfile);
        return false;
    }

    //Free the now unnecessary strings
    free(pllfile);
    free(ollfile);
    issetup = true;
    return true;
}
