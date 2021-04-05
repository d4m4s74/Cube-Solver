#include "utils.h"
#include "cube.h"
#include "cross.h"
#include "f2l.h"
#include "lastlayer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

bool setup(int argc, char *argv[]);
void print_usage(char *filename);

//The filenames for the PLL and OLL files
char *pllFileName = "plls.csv";
char *ollFileName = "olls.csv";

int main(int argc, char *argv[])
{
    //If no argument provided, print usage and break
    if (argc == 1 || getopt(argc, argv, ":h") == 'h' || strcmp(argv[1], "--help") == 0)
    {
        print_usage(argv[0]);
        return 0;
    }
    //try to set everything up (mostly loading external files)
    if (!setup(argc, argv))
    {
        //If it fails, unload te algorithms and quit
        cleanup_last_layer();
        return 2;
    }
    //create a 2d int array for the cube
    int cube[6][9];
    //Fill in the colors
    //Order:            Front        Right        Back          Left         Up           Down
    color_cube(cube, "000000000", "111111111", "222222222", "333333333", "444444444", "555555555");
    //If only one argument is given (after the options) the user entered an algorithm
    if (argc == optind + 1)
    {
        printf("Scramble:\n%s\n", argv[optind]);
        run_algorithm(cube, argv[optind]);
    }
    //if 6 arguments are given, the user filled in the cube colors
    else if (argc == optind + 6)
    {
        int order[] = {4, 0, 1, 2, 3, 5};
        for (int i = 0; i < 6; i++)
        {
            if (strlen(argv[optind + i]) != 9)
            {
                printf("Invalid color combination. Enter 9 colors for each square.\n");
                return 1;
            }
            for (int j = 0; j < 9; j++)
            {
                if (argv[optind + i][j] == 'g' || argv[optind + i][j] == 'G' || argv[optind + i][j] == '0')
                {
                    cube[order[i]][j] = 0;
                }
                else if (argv[optind + i][j] == 'r' || argv[optind + i][j] == 'R' || argv[optind + i][j] == '1')
                {
                    cube[order[i]][j] = 1;
                }
                else if (argv[optind + i][j] == 'b' || argv[optind + i][j] == 'B' || argv[optind + i][j] == '2')
                {
                    cube[order[i]][j] = 2;
                }
                else if (argv[optind + i][j] == 'o' || argv[optind + i][j] == 'O' || argv[optind + i][j] == '3')
                {
                    cube[order[i]][j] = 3;
                }
                else if (argv[optind + i][j] == 'w' || argv[optind + i][j] == 'W' || argv[optind + i][j] == '4')
                {
                    cube[order[i]][j] = 4;
                }
                else if (argv[optind + i][j] == 'y' || argv[optind + i][j] == 'Y' || argv[optind + i][j] == '5')
                {
                    cube[order[i]][j] = 5;
                }
                else
                {
                    printf("Invalid color %c. Valid colors are G(reen), R(ed), B(lue), O(range), W(hite) and Y(ellow)\n", argv[optind + i][j]);
                    return 1;
                }
            }
        }
        if (!validate(cube))
        {
            print_cube(cube);
            printf("Invalid color combination. Colors are not possible on normal rubik's cube\n");
            return 1;
        }
    }
    else
    {
        print_usage(argv[0]);
        return 0;
    }
    //Print the scrambled cube
    print_cube(cube);
    //Solve the cross,
    char *cross = solve_cross(cube);
    //Check if it works
    if (cross)
    {
        //Print and free the algorithm
        printf("Cross\n%s\n", cross);
        free(cross);
    }
    else
    {
        //If it fails, post an error, clean up and exit
        printf("Error solving cross\n");
        cleanup_last_layer();
        return 2;
    }
    //Print the cube
    print_cube(cube);
    //repeater for f2l
    char *f2l = solve_f2l(cube);
    if (f2l)
    {
        printf("F2L\n%s\n", f2l);
        free(f2l);
    }
    else
    {
        printf("Error solving f2l\n");
        cleanup_last_layer();
        return 2;
    }
    print_cube(cube);
    //Repeat for oll
    char *oll = solve_oll(cube);
    if (oll)
    {
        printf("OLL: %s\n", oll);
        free(oll);
    }
    else
    {
        printf("OLL Parity!\n");
        cleanup_last_layer();
        return 2;
    }
    print_cube(cube);
    //repeat for pll
    char *pll = solve_pll(cube);
    if (pll)
    {
        printf("PLL: %s\n", pll);
        free(pll);
    }
    else
    {
        printf("PLL Parity!\n");
        cleanup_last_layer();
        return 2;
    }
    //Print the cube one last time
    print_cube(cube);
    //Clean up and exit
    cleanup_last_layer();
    return 0;
}

//Load all OLLs and PLLs into memory
bool setup(int argc, char *argv[])
{
    //All that work to get the filepath
    //create a char called filepath the same size as argv[0]
    char *filepath = (char *)malloc(strlen(argv[0]) + 1);
    if (filepath == NULL)
    {
        return false;
    }
    //copy argv[0] into filepath
    strncpy(filepath, argv[0], strlen(argv[0]) + 1);

    char slash;
    //define what slash is used
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    slash = '\\';
#else
    slash = '/';
#endif
    //find the last slash
    char *lastslash = strrchr(filepath, slash);
    if (lastslash != NULL)
    {
        //If there is one, the slash becomes \0
        lastslash[0] = 0;
    }
    else
    {
        //Otherwise the first char is . and the second is \0
        filepath[0] = '.';
        filepath[1] = 0;
    }

    //Make a string for the paths of the oll file
    char *ollfile = NULL;
    char *pllfile = NULL;

    //check if the user used the -o or -p options
    int opt;
    //bools for if strings need to be freed. Default to true.
    bool fo = true, fp = true, ff = true;
    //reset optind
    optind = 1;
    //go through the options to check for the oll or pll file, or their folder
    while ((opt = getopt(argc, argv, ":o:p:d:")) != -1)
    {
        switch (opt)
        {
            case 'o':
                ollfile = optarg;
                fo = false;
                break;
            case 'p':
                pllfile = optarg;
                fp = false;
                break;
            case 'd':
                //replace filepath with the argument
                free(filepath);
                filepath = optarg;
                ff = false;
                break;

        }
    }
    //if ollfile is NULL, default to the folder the application is in
    if (ollfile == NULL)
    {
        ollfile = malloc(strlen(filepath) + strlen(ollFileName) + 2);
        if (ollfile == NULL)
        {
            return false;
        }
        snprintf(ollfile, strlen(filepath) + strlen(ollFileName) + 2, "%s%c%s", filepath, slash, ollFileName);
    }
    //load olls
    if (!load_olls(ollfile))
    {
        if (ff)
        {
            free(filepath);
        }
        if (fo)
        {
            free(ollfile);
        }
        printf("Error loading OLLs. Please supply file using -o or supply default path using -d\n");
        return false;
    }
    //if pllfile is NULL, default to the folder the application is in
    if (pllfile == NULL)
    {
        pllfile = malloc(strlen(filepath) + strlen(pllFileName) + 2);
        if (pllfile == NULL)
        {
            return false;
        }
        snprintf(pllfile, strlen(filepath) + strlen(pllFileName) + 2, "%s%c%s", filepath, slash, pllFileName);
    }
    //load the pll file
    if (!load_plls(pllfile))
    {
        if (ff)
        {
            free(filepath);
        }
        if (fp)
        {
            free(pllfile);
        }
        if (fo)
        {
            free(ollfile);
        }
        printf("Error loading PLLs. Please supply file using -p or supply default path using -d\n");
        return false;
    }

    //Free the now unnecessary strings
    if (ff)
    {
        free(filepath);
    }
    if (fp)
    {
        free(pllfile);
    }
    if (fo)
    {
        free(ollfile);
    }
    return true;
}

void print_usage(char *filename)
{
    printf("Usage: %s [option...] \"Scramble algorithm\"\n", filename);
    printf("Solves a rubik's cube, scrambled with a given algorithm\n");
    printf("Usage: %s [option...] [Up] [Front] [Right] [Back] [Left] [Down]\n", filename);
    printf("Solves a rubik's cube with a given pattern\n\n");
    printf("  -d <directory>   Specify the directory where the olls.csv and plls.csv file are located.\n");
    printf("                   Defaults to the same directory as the executable\n");
    printf("  -o <file>        Specify the path to the olls.csv file\n");
    printf("  -p <file>        Specify the path to the plls.csv file\n");
    printf("  -h, --help       Display this help and exit\n\n");
    printf("Valid colors for faces are the first letters of:\n");
    printf("G(reen), R(ed), B(lue), O(range), W(hite) and Y(ellow)\n");
}