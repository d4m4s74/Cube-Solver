#include "cubesolver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>

char *append(char *, char *);
char *getDir(const char *path);
void helptext(const char *filename);
void usage(const char *filename);
int color_letter_to_number(char c);
void color_cube_interactive(int cube[6][9]);
void color_face_from_input(int cube[6][9], int face);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
char slash = '\\';
#else
char slash = '/';
#endif

//default filenames:
char *pllFileName = "plls.csv";
char *ollFileName = "olls.csv";

int main(int argc, char **argv)
{
    //check for arguments, if not, print usage
    if (argc == 1)
    {
        usage(argv[0]);
        return 0;
    }
    if (strcmp(argv[1], "--help") == 0)
    {
        helptext(argv[0]);
        return 0;
    }
    //Get all options
    int opt;
    char *ollfile = NULL, *pllfile = NULL, *dir = NULL;
    bool textOnly = false;
    bool interactive = false;
    while ((opt = getopt(argc, argv, ":o:p:d:thi")) != -1)
    {
        switch (opt)
        {
            case 'o':
                ollfile = optarg;
                break;
            case 'p':
                pllfile = optarg;
                break;
            case 'd':
                dir = optarg;
                break;
            case 't':
                textOnly = true;
                break;
            case 'i':
                interactive = true;
                break;
            case 'h':
                helptext(argv[0]);
                return 0;
        }
    }
    //If no oll or pll files are given
    if (!ollfile && !pllfile)
    {
        if (dir)
        {
            if (!setup(dir))
            {
                printf("Unable to load OLL and/or PLL algorithms from directory %s", dir);
                return 1;
            }
        }
        else
        {
            dir = getDir(argv[1]);
            if (!setup(dir))
            {
                printf("Unable to load OLL and/or PLL algorithms from directory %s", dir);
                free(dir);
                return 1;
            }
            free(dir);
            dir = NULL;
        }
    }
    //if no oll file is given (but a pll file is)
    else if (!ollfile)
    {
        //load the olls using the given pll file
        if (!load_plls(pllfile))
        {
            printf("Unable to load PLL algorithms from %s", pllfile);
            return 1;
        }
        //generate the path for the pll file by combining the directory the application is in with the default filename
        ollfile = append(getDir(argv[1]), ollFileName);
        //load the ollls
        if (!load_olls(ollfile))
        {
            printf("Unable to load OLL algorithms from %s", ollfile);
            free(ollfile);
            return 1;
        }
        free(ollfile);
    }
    //if no pll file is given (but an oll file is)
    else if (!ollfile)
    {
        //load the olls using the given oll file
        if (!load_olls(ollfile))
        {
            printf("Unable to load OLL algorithms from %s", ollfile);
            return 1;
        }
        //generate the path for the pll file by combining the directory the application is in with the default filename
        pllfile = append(getDir(argv[1]), pllFileName);
        //load the pllls
        if (!load_plls(pllfile))
        {
            printf("Unable to load PLL algorithms from %s", pllfile);
            free(pllfile);
            return 1;
        }
        free(pllfile);
    }
    else
    {
        //load the olls using the given oll file
        if (!load_olls(ollfile))
        {
            printf("Unable to load OLL algorithms from %s", ollfile);
            return 1;
        }
        //load the olls using the given pll file
        if (!load_plls(pllfile))
        {
            printf("Unable to load PLL algorithms from %s", pllfile);
            return 1;
        }
    }

    //create an array to store the cube
    int cube[6][9];
    if (interactive)
    {
        //interactively get the colors
        color_cube_interactive(cube);
    }
    else if (argc == optind + 6)
    {
        //order in which I ask for the colors
        int order[] = {4, 0, 1, 2, 3, 5};
        for (int i = 0; i < 6; i++)
        {
            //If the user didn't enter 9 characters, we have to print an error
            if (strlen(argv[optind + i]) != 9)
            {
                printf("Invalid color combination. Enter 9 colors for each face.\n");
                return 2;
            }
            for (int j = 0; j < 9; j++)
            {
                //Change the color letter to a number the solver understands
                int n = color_letter_to_number(argv[optind + i][j]);
                //If it's valid, add it to the cube
                if (i != -1)
                {
                    cube[order[i]][j] = n;
                }
                //Otherwise, error out.
                else
                {
                    printf("Invalid color %c. Valid colors are G(reen), R(ed), B(lue), O(range), W(hite) and Y(ellow)\n", argv[optind + i][j]);
                    return 2;
                }
            }
        }
        //When done, print the cube
        if (!textOnly)
        {
            print_cube(cube);
        }
    }
    else if (argc == optind + 1)
    {
        //Generate a solved cube
        color_cube(cube, "000000000", "111111111", "222222222", "333333333", "444444444", "555555555");
        //Print the scramble
        printf("Scramble:\n%s\n", argv[optind]);
        //Run the scramble
        run_algorithm(cube, argv[optind]);
        //When done, print the cube
        if (!textOnly)
        {
            print_cube(cube);
        }
    }
    //If the user didn't enter anything, print out usage
    else
    {
        usage(argv[0]);
        return 0;
    }

    //Validate the colors of the cube (This does not check if the cube is solvable)
    if (!validate(cube))
    {
        printf("Invalid color combination. Colors are not possible on normal rubik's cube\n");
        return 2;
    }
    //This code could be copied from the last iteration
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
    if (!textOnly)
    {
        print_cube(cube);
    }
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
    if (!textOnly)
    {
        print_cube(cube);
    }
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
    if (!textOnly)
    {
        print_cube(cube);
    }
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
    if (!textOnly)
    {
        print_cube(cube);
    }
    cleanup_last_layer();
}

char *getDir(const char *path)
{
    //make a copy of the path
    // file deepcode ignore MemoryManagementControl: <please specify a reason of ignoring this>
    char *dir = malloc(strlen(path) + 1);
    if (dir == NULL)
    {
        return NULL;
    }
    strncpy(dir, path, strlen(path) + 1);
    //Look for the last slash
    char *lastslash = strrchr(dir, slash);
    if (lastslash != NULL)
    {
        //If there is one, the char acter the last slash becomes \0
        lastslash[1] = 0;
    }
    else
    {
        //Otherwise the first char is ., the second is slash, and the third is \0
        dir[0] = '.';
        dir[1] = slash;
        dir[2] = 0;
    }
    return dir;
}

int color_letter_to_number(char c)
{
    //to save on conversion and tests, turn c into upper case
    c = toupper(c);
    //Green translate to 0
    if (c == 'G' || c == '0')
    {
        return 0;
    }
    //Red translates to 1
    if (c == 'R' || c == '1')
    {
        return 1;
    }
    //Blue translates to 2
    if (c == 'B' || c == '2')
    {
        return 2;
    }
    //Orange 3
    if (c == 'O' || c == '3')
    {
        return 3;
    }
    //White 4
    if (c == 'W' || c == '4')
    {
        return 4;
    }
    //Yellow 5
    if (c == 'Y' || c == '5')
    {
        return 5;
    }
    return -1;
}

//Colors a single face of the cube from stdin
void color_face_from_input(int cube[6][9], int face)
{
    //We need an interable. if I reached 9 without error we've sucesfully filled in a cube
    int i;
    //Valid stores if the entered information is valid.
    bool valid;
    do
    {
        printf("Enter 9 valid colors: ");
        //set the iterator to 0 and valid to true;
        i = 0;
        valid = true;
        //variable to store the char
        int c;
        //We always go through the entire input to have a clean slate/
        while ((c = getchar()) != '\n')
        {
            //If the user enters too much, just go on until we reach the newline, but don't do anything
            if (i < 9 && valid == true)
            {
                //Change the ltter to the right number
                int n = color_letter_to_number(c);
                //If the number is invalid say so and clear stdin
                if (n == -1)
                {
                    printf("%c is not a valid color\n", c);
                    valid = false;
                }
                else
                {
                    //Otherwise save it in the face.
                    cube[face][i] = n;
                }
            }
            i++;
        }
        //Repeat until we have enough digits;
    }
    while (i < 9 || valid == false);
}

//interactively gets the colors of the cube.
void color_cube_interactive(int cube[6][9])
{
    //Set default colors (mostly white because it looks good)
    color_cube(cube, "444404444", "444414444", "444424444", "444434444", "444444444", "444454444");
    printf("Hold the cube with the green center in front, and the white center on top\n");
    printf("Fill in the colors of each face, using the first letters of each color (Green, Red, Blue, Orange, White, Yellow\n");
    printf("Enter the colors of the \"up\" face      >■\n");
    printf("                                      ■ ■ ■ ■\n");
    printf("                                        ■\n");
    //Call color_face_from_input to ask for the right characters
    color_face_from_input(cube, 4);
    //Print the cube.
    print_cube(cube);
    printf("Enter the colors of the \"front\" face    ■\n");
    printf("                                      ■>■ ■ ■\n");
    printf("                                        ■\n");
    color_face_from_input(cube, 0);
    print_cube(cube);
    printf("Enter the colors of the \"right\" face    ■\n");
    printf("                                      ■ ■>■ ■\n");
    printf("                                        ■\n");
    color_face_from_input(cube, 1);
    print_cube(cube);
    printf("Enter the colors of the \"back\" face     ■\n");
    printf("                                      ■ ■ ■>■\n");
    printf("                                        ■\n");
    color_face_from_input(cube, 2);
    print_cube(cube);
    printf("Enter the colors of the \"left\" face     ■\n");
    printf("                                     >■ ■ ■ ■\n");
    printf("                                        ■\n");
    color_face_from_input(cube, 3);
    print_cube(cube);
    printf("Enter the colors of the \"down\" face     ■\n");
    printf("                                      ■ ■ ■ ■\n");
    printf("                                       >■\n");
    color_face_from_input(cube, 5);
    print_cube(cube);
}

void helptext(const char *filename)
{
    printf("Usage: %s [option...] \"Scramble algorithm\"\n", filename);
    printf("Solves a rubik's cube, scrambled with a given algorithm\n");
    printf("Alt usage: %s [option...] [Up] [Front] [Right] [Back] [Left] [Down]\n", filename);
    printf("Solves a rubik's cube with a given pattern\n\n");
    printf("  -d <directory>   Specify the directory where the olls.csv and plls.csv file are located.\n");
    printf("                   Defaults to the same directory as the executable\n");
    printf("  -o <file>        Specify the path to the olls.csv file\n");
    printf("  -p <file>        Specify the path to the plls.csv file\n");
    printf("  -t               Text Only, do not print the cube\n");
    printf("  -i               Fill in the cube interactively\n");
    printf("  -h, --help       Display this help and exit\n\n");
    printf("Valid colors for faces are the first letters of:\n");
    printf("G(reen), R(ed), B(lue), O(range), W(hite) and Y(ellow)\n");
}

void usage(const char *filename)
{
    printf("Usage: %s [option...] \"Scramble algorithm\"\n", filename);
    printf("Alt usage: %s [option...] [Up] [Front] [Right] [Back] [Left] [Down]\n", filename);
}
