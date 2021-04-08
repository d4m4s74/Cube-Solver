#include "cube.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//colors
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
char *C[7] = {"G", "R", "B", "O", "W", "Y", ""};
#else
char *C[7] = {"\e[42mG", "\e[41mR", "\e[44mB", "\e[48;5;208mO", "\e[107mW", "\e[103mY", "\e[49m"}; //green, red, blue, orange, white, yellow
#endif


//A function that adds the colors from four strings onto the cube. Might later be changed to a single string.
void color_cube(int cube[6][9], char *f, char *r, char *b, char *l, char *u, char *d)
{
    for (int i = 0; i < 9; i++)
    {
        cube[0][i] = f[i] - '0';
        cube[1][i] = r[i] - '0';
        cube[2][i] = b[i] - '0';
        cube[3][i] = l[i] - '0';
        cube[4][i] = u[i] - '0';
        cube[5][i] = d[i] - '0';
    }
}

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

bool validate(int cube[6][9])
{
    //This is a 2d bool array containing the color combinations possible in a corner.
    bool edges[6][6] =
    {
        {0, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1, 1},
        {0, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1, 1},
        {1, 1, 1, 1, 0, 0},
        {1, 1, 1, 1, 0, 0}
    };
    //3d array containing the possibible clockwise corner combinations.
    bool corners[6][6][6] =
    {
        {   {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0}
        },

        {   {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {1, 0, 0, 0, 0, 0}
        },

        {   {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1},
            {0, 0, 0, 1, 0, 0},
            {0, 1, 0, 0, 0, 0}
        },

        {   {0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0}
        },

        {   {0, 0, 0, 1, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0}
        },

        {   {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0}
        },
    };
    int up1[4] = {7, 5, 1, 3};
    int down1[4] = {1, 5, 7, 3};
    int up2[4] = {8, 2, 0, 6};
    int down2[4] = {2, 8, 6, 0};

    //check the centers
    //as if 4 is up
    if (cube[U][4] == 4)
    {
        //the colors 0 through 3 to anti-clockwise around the yellow side
        if (!(cube[R][4] == (cube[F][4] + 1) % 4 &&
              cube[B][4] == (cube[R][4] + 1) % 4 &&
              cube[L][4] == (cube[B][4] + 1) % 4 &&
              cube[F][4] == (cube[L][4] + 1) % 4))
        {
            return false;
        }
    }
    //as if 5 is up
    if (cube[U][4] == 5)
    {
        //the colors 3 through 0 to clockwise around the white side
        if (!(cube[L][4] == (cube[F][4] + 1) % 4 &&
              cube[B][4] == (cube[L][4] + 1) % 4 &&
              cube[R][4] == (cube[B][4] + 1) % 4 &&
              cube[F][4] == (cube[R][4] + 1) % 4))
        {
            return false;
        }

    }
    //as if 4 is right
    if (cube[R][4] == 4)
    {
        if (!(cube[F][4] == (cube[U][4] + 1) % 4 &&
              cube[U][4] == (cube[B][4] + 1) % 4 &&
              cube[B][4] == (cube[D][4] + 1) % 4 &&
              cube[D][4] == (cube[F][4] + 1) % 4))
        {
            return false;
        }

    }
    //as if 5 is right
    if (cube[R][4] == 5)
    {
        if (!(cube[B][4] == (cube[U][4] + 1) % 4 &&
              cube[U][4] == (cube[F][4] + 1) % 4 &&
              cube[F][4] == (cube[D][4] + 1) % 4 &&
              cube[D][4] == (cube[B][4] + 1) % 4))
        {
            return false;
        }

    }
    //as if 4 is front
    if (cube[F][4] == 4)
    {
        if (!(cube[L][4] == (cube[U][4] + 1) % 4 &&
              cube[U][4] == (cube[R][4] + 1) % 4 &&
              cube[R][4] == (cube[D][4] + 1) % 4 &&
              cube[D][4] == (cube[L][4] + 1) % 4))
        {
            return false;
        }

    }
    //as if 5 is front
    if (cube[F][4] == 5)
    {
        if (!(cube[R][4] == (cube[U][4] + 1) % 4 &&
              cube[U][4] == (cube[L][4] + 1) % 4 &&
              cube[L][4] == (cube[D][4] + 1) % 4 &&
              cube[D][4] == (cube[R][4] + 1) % 4))
        {
            return false;
        }

    }
//check the edges
//first check the up slice
    for (int i = 0; i < 4; i++)
    {
        //check if square 1 (top center) of each face, combined with the corresponding up square is possible.
        if (edges[cube[i][1]][cube[U][up1[i]]] && edges[cube[U][up1[i]]][cube[i][1]])
        {
            //If so, set them to false so there can't be a second.
            edges[cube[i][1]][cube[U][up1[i]]] = 0;
            edges[cube[U][up1[i]]][cube[i][1]] = 0;
        }
        else
        {
            return false;
        }
    }
//then the E slice
    for (int i = 0; i < 4; i++)
    {
        //check if square 5 (center right) of every face, combined with square 3 (center left) of the next face.
        if (edges[cube[i][5]][cube[(i + 1) % 4][3]] && edges[cube[(i + 1) % 4][3]][cube[i][5]])
        {
            //If so, set them to false so there can't be a second.
            edges[cube[i][5]][cube[(i + 1) % 4][3]] = 0;
            edges[cube[(i + 1) % 4][3]][cube[i][5]] = 0;
        }
        else
        {
            return false;
        }
    }
//then the D slice
    for (int i = 0; i < 4; i++)
    {
        //check if square 7 (bottom center) of each face, combined with the corresponding down square is possible.
        if (edges[cube[i][7]][cube[D][down1[i]]] && edges[cube[D][down1[i]]][cube[i][7]])
        {
            //If so, set them to false so there can't be a second.
            edges[cube[i][7]][cube[D][down1[i]]] = 0;
            edges[cube[D][down1[i]]][cube[i][7]] = 0;
        }
        else
        {
            return false;
        }
    }
    //next check the corners
    //first the U slice
    for (int i = 0; i < 4; i++)
    {
        //check if square 2 (top right) of each face, combined with the corresponding up square, combined with square 0 of the next face is possible.
        //repeat for every possible starting point
        if (corners[cube[i][2]][cube[U][up2[i]]][cube[(i + 1) % 4][0]] &&
            corners[cube[U][up2[i]]][cube[(i + 1) % 4][0]][cube[i][2]] &&
            corners[cube[(i + 1) % 4][0]][cube[i][2]][cube[U][up2[i]]])
        {
            corners[cube[i][2]][cube[U][up2[i]]][cube[(i + 1) % 4][0]] = 0;
            corners[cube[U][up2[i]]][cube[(i + 1) % 4][0]][cube[i][2]] = 0;
            corners[cube[(i + 1) % 4][0]][cube[i][2]][cube[U][up2[i]]] = 0;
        }
        else
        {
            return false;
        }
    }
    //then the D slice
    for (int i = 0; i < 4; i++)
    {
        //check if square 8 (bottom right) of each face, combined with the corresponding down square, combined with square 6 of the next face is possible.
        //repeat for every possible starting point
        if (corners[cube[i][8]][cube[(i + 1) % 4][6]][cube[D][down2[i]]] &&
            corners[cube[(i + 1) % 4][6]][cube[D][down2[i]]][cube[i][8]] &&
            corners[cube[D][down2[i]]][cube[i][8]][cube[(i + 1) % 4][6]])
        {
            corners[cube[i][8]][cube[(i + 1) % 4][6]][cube[D][down2[i]]] = 0;
            corners[cube[(i + 1) % 4][6]][cube[D][down2[i]]][cube[i][8]] = 0;
            corners[cube[D][down2[i]]][cube[i][8]][cube[(i + 1) % 4][6]] = 0;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void print_cube(int cube[6][9])
{
    //Todo: Rewrite this to work with loops if possible

    //%s gets replaced with the color tag with the number, %i with the number on the face
    printf("    %s%s%s%s\n",
           C[cube[4][0]], C[cube[4][1]], C[cube[4][2]], C[6]);
    printf("    %s%s%s%s\n",
           C[cube[4][3]],  C[cube[4][4]], C[cube[4][5]], C[6]);
    printf("    %s%s%s%s\n\n",
           C[cube[4][6]], C[cube[4][7]], C[cube[4][8]],  C[6]);
    printf("%s%s%s%s %s%s%s%s %s%s%s%s %s%s%s%s\n",
           C[cube[3][0]], C[cube[3][1]], C[cube[3][2]], C[6],
           C[cube[0][0]], C[cube[0][1]], C[cube[0][2]], C[6],
           C[cube[1][0]], C[cube[1][1]], C[cube[1][2]], C[6],
           C[cube[2][0]], C[cube[2][1]], C[cube[2][2]], C[6]);
    printf("%s%s%s%s %s%s%s%s %s%s%s%s %s%s%s%s\n",
           C[cube[3][3]], C[cube[3][4]], C[cube[3][5]], C[6],
           C[cube[0][3]], C[cube[0][4]], C[cube[0][5]], C[6],
           C[cube[1][3]], C[cube[1][4]], C[cube[1][5]], C[6],
           C[cube[2][3]], C[cube[2][4]], C[cube[2][5]], C[6]);
    printf("%s%s%s%s %s%s%s%s %s%s%s%s %s%s%s%s\n\n",
           C[cube[3][6]], C[cube[3][7]], C[cube[3][8]], C[6],
           C[cube[0][6]], C[cube[0][7]], C[cube[0][8]], C[6],
           C[cube[1][6]], C[cube[1][7]], C[cube[1][8]], C[6],
           C[cube[2][6]], C[cube[2][7]], C[cube[2][8]], C[6]);
    printf("    %s%s%s%s\n",
           C[cube[5][0]], C[cube[5][1]], C[cube[5][2]], C[6]);
    printf("    %s%s%s%s\n",
           C[cube[5][3]], C[cube[5][4]], C[cube[5][5]], C[6]);
    printf("    %s%s%s%s\n\n",
           C[cube[5][6]], C[cube[5][7]], C[cube[5][8]], C[6]);
}

/*
A function that moves the virtual cube
Each face moves clockwise, assuming you're facing the face.
int move: the move done
0 for F
1 for R
2 for B
...
6 for M
7 for E
8 for S
int times: the amount of times the same move is repeated.
The function does this recursively.
*/
void move_cube(int cube[6][9], int move, int times)
{
    if (times == 0)
    {
        return;
    }
    switch (move)
    {
        case 0: //Front
            //turn the outsides of edges and corners
            cycle(&cube[1][0], &cube[5][2], &cube[3][8], &cube[4][6]);
            cycle(&cube[1][3], &cube[5][1], &cube[3][5], &cube[4][7]);
            cycle(&cube[1][6], &cube[5][0], &cube[3][2], &cube[4][8]);
            //turn the face
            cycle(&cube[0][0], &cube[0][2], &cube[0][8], &cube[0][6]);
            cycle(&cube[0][1], &cube[0][5], &cube[0][7], &cube[0][3]);
            break;
        case 1: //Right
            cycle(&cube[0][8], &cube[4][8], &cube[2][0], &cube[5][8]);
            cycle(&cube[0][5], &cube[4][5], &cube[2][3], &cube[5][5]);
            cycle(&cube[0][2], &cube[4][2], &cube[2][6], &cube[5][2]);

            cycle(&cube[1][0], &cube[1][2], &cube[1][8], &cube[1][6]);
            cycle(&cube[1][1], &cube[1][5], &cube[1][7], &cube[1][3]);
            break;
        case 2: //Back
            cycle(&cube[1][2], &cube[4][0], &cube[3][6], &cube[5][8]);
            cycle(&cube[1][5], &cube[4][1], &cube[3][3], &cube[5][7]);
            cycle(&cube[1][8], &cube[4][2], &cube[3][0], &cube[5][6]);

            cycle(&cube[2][0], &cube[2][2], &cube[2][8], &cube[2][6]);
            cycle(&cube[2][1], &cube[2][5], &cube[2][7], &cube[2][3]);
            break;
        case 3: //Left
            cycle(&cube[0][0], &cube[5][0], &cube[2][8], &cube[4][0]);
            cycle(&cube[0][3], &cube[5][3], &cube[2][5], &cube[4][3]);
            cycle(&cube[0][6], &cube[5][6], &cube[2][2], &cube[4][6]);

            cycle(&cube[3][0], &cube[3][2], &cube[3][8], &cube[3][6]);
            cycle(&cube[3][1], &cube[3][5], &cube[3][7], &cube[3][3]);
            break;
        case 4: //up
            cycle(&cube[0][0], &cube[3][0], &cube[2][0], &cube[1][0]);
            cycle(&cube[0][1], &cube[3][1], &cube[2][1], &cube[1][1]);
            cycle(&cube[0][2], &cube[3][2], &cube[2][2], &cube[1][2]);
            //turn the cube
            cycle(&cube[4][0], &cube[4][2], &cube[4][8], &cube[4][6]);
            cycle(&cube[4][1], &cube[4][5], &cube[4][7], &cube[4][3]);
            break;
        case 5: //down
            cycle(&cube[0][6], &cube[1][6], &cube[2][6], &cube[3][6]);
            cycle(&cube[0][7], &cube[1][7], &cube[2][7], &cube[3][7]);
            cycle(&cube[0][8], &cube[1][8], &cube[2][8], &cube[3][8]);
            //turn the cube
            cycle(&cube[5][0], &cube[5][2], &cube[5][8], &cube[5][6]);
            cycle(&cube[5][1], &cube[5][5], &cube[5][7], &cube[5][3]);
            break;
        case 6: //middle layer
            cycle(&cube[0][1], &cube[5][1], &cube[2][7], &cube[4][1]);
            cycle(&cube[0][4], &cube[5][4], &cube[2][4], &cube[4][4]);
            cycle(&cube[0][7], &cube[5][7], &cube[2][1], &cube[4][7]);
            break;
        case 7: //equatial layer
            cycle(&cube[0][3], &cube[1][3], &cube[2][3], &cube[3][3]);
            cycle(&cube[0][4], &cube[1][4], &cube[2][4], &cube[3][4]);
            cycle(&cube[0][5], &cube[1][5], &cube[2][5], &cube[3][5]);
            break;
        case 8: //standing layer
            cycle(&cube[1][1], &cube[5][5], &cube[3][7], &cube[4][3]);
            cycle(&cube[1][4], &cube[5][4], &cube[3][4], &cube[4][4]);
            cycle(&cube[1][7], &cube[5][3], &cube[3][1], &cube[4][5]);
            break;
    }
    if (times > 1)
    {
        move_cube(cube, move, times - 1);
    }
}

//Runs a multiple move algorithm on the cube, using standard cube notation
void run_algorithm(int cube[6][9], char *algorithm)
{
    if (algorithm == NULL)
    {
        return;
    }
    for (int i = 0, n = strlen(algorithm); i < n; i++)
    {
        int times = 1;
        if (i != n - 1)
        {
            switch (algorithm[i + 1])
            {
                //' after the letter means move it the other way around, which is the same as moving it 3 times
                case '\'':
                    times = 3;
                    break;
                //2 after the letter means move it 2 times
                case '2':
                    times = 2;
                    break;
            }
        }
        switch (algorithm[i])
        {
            case 'F':
                move_cube(cube, F, times);
                break;
            //A small letter means move the face, and the corresponding M, S or E layer.
            case 'f':
                move_cube(cube, F, times);
                move_cube(cube, S, times);
                break;
            case 'R':
                move_cube(cube, R, times);
                break;
            case 'r':
                move_cube(cube, R, times);
                //Since M follows L, which is the oposite direction to R, move M 3 times for 1, and 1 times for 3
                move_cube(cube, M, 4 - times);
                break;
            case 'B':
                move_cube(cube, B, times);
                break;
            case 'b':
                move_cube(cube, S, 4 - times);
                break;
            case 'L':
                move_cube(cube, L, times);
                break;
            case 'l':
                move_cube(cube, L, times);
                move_cube(cube, M, times);
                break;
            case 'U':
                move_cube(cube, U, times);
                break;
            case 'u':
                move_cube(cube, U, times);
                move_cube(cube, E, 4 - times);
                break;
            case 'D':
                move_cube(cube, D, times);
                break;
            case 'd':
                move_cube(cube, D, times);
                move_cube(cube, E, times);
                break;
            case 'M':
                move_cube(cube, M, times);
                break;
            case 'E':
                move_cube(cube, E, times);
                break;
            case 'S':
                move_cube(cube, S, times);
                break;
            case 'x':
                //x, y and z rotate the full cube
                move_cube(cube, R, times);
                move_cube(cube, M, 4 - times);
                move_cube(cube, L, 4 - times);
                break;
            case 'y':
                move_cube(cube, U, times);
                move_cube(cube, E, 4 - times);
                move_cube(cube, D, 4 - times);
                break;
            case 'z':
                move_cube(cube, F, times);
                move_cube(cube, S, times);
                move_cube(cube, B, 4 - times);
                break;
        }
        //If there is a 2 or ' after the number, increment i an extra time to skip it.
        if (times != 1)
        {
            i++;
        }
    }
}