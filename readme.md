# Rubik's Cube Solver
#### Video Demo:  <URL HERE>
#### Description:
My Final Project for CS50 is a "Human Method" Rubik's Cube Solver.  
This means that the entered Rubik's Cube is solved using a method a human could use (CFOP), with clear steps: Cross, F2L, OLL and PLL.  
The final product consists of a shell application, a C library for use in other projects, and a web interface writen in Flask, HTML, CSS and Javascript. 
## How to use
### Solver
#### Linux
Download the bin/solver executable or build from source using `make solver`
#### OS X
Download or clone the files from this repository and build using `make solver`
#### Windows
Download or clone the files using this repository and compile all .c files in src except for solver_library
#### All
Execute the solver with one of the following commands:
```
./solver "Algorithm"
./solver [Up] [Front] [Right] [Back] [Left] [Down]
```
Replacing the algorithm with a scramble algorithm (eg `U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2`) or the faces with the colors on that face (eg `wbwowrwgw gwgogrgyg rwrgrbryr bwbrbobyb owobogoyo ygyoyryby`).  
The possible colors are the first characters of Green, Red, Blue, Orange, White and Yellow.  
If the olls.txt and plls.txt files are not in the same folder as the binary, use options -o and -p, or -d to point to the files
```
bin/solver -d data/ "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2"
bin/solver -o data/olls.csv -p data/plls.csv "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2"
```
Add option `-t` to only print the text without images of the cube. 
### Web interface
#### Linux:  
Download or clone the files from this repository.  
Install Flask and Numpy using pip.
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
#### OS X:
Download or clone the files from this repository.  
Remove the Linux version of bin/libcubesolver.so manually or using `make clean` and recompile using `make library`.  
Install Flask and Numpy using pip.
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
#### Windows:
Download or clone the files from this repository.
Remove the Linux version of bin/libcubesolver.so manually.  
Compile all .c files in src except for solver.c to bin/libcubesolver.so. Or if using a different name, change line 19 in app.py to reflect that.  
Install Flask and Numpy using pip.
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
### Solver Library
#### Install for use with C or C++
Compile all files in src except for solver.c to libcubesolver.so, libcubesolver.dylib or libcubesolver.dll and save it wherever libraries are saved on your pc.  
On Linux you can use `make library`
Copy libcubesolver.h from src to wherever headers are saved on your pc (ex. /usr/include/)
#### Usage with C or C++
Link with -lcubesolver
```C
#include <cubesolver.h>

//Use either setup, or both load_olls and load_plls
//Load all OLLs and PLLs into memory. Path is the folder where the olls.csv and plls.csv file are located.
//Returns indicating for succes or failure
setup(path);
//Loads the OLLs from a CSV file. Returns bool indicating success or failure
load_olls(filename);
//Loads the PLLs from a CSV file. Returns bool indicating success or failure
load_plls(filename);
//Create an array to hold the cube. 6 faces, 9 squares per face.
//The faces in order are Front, Right, Back, Left, Up and Down
int cube[6][9];
//Add the "colors" of the cube to the array as 9 character strings containing numbers 0 to 5
color_cube(cube, front, right, back, left, up, down);
//Run a multiple move algorithm on the cube, using standard cube notation (Useful for scrambling)
void run_algorithm(cube, "Algorithm");
/*
A function that prints the sides of the cube in an exploded fashion. Uses colors when in linux or OS X
Uses Green for 0, Red for 1, Blue for 2, Orange for 3, White for 4, Yellow for 5
    WWW
    WWW
    WWW

OOO GGG RRR BBB
OOO GGG RRR BBB
OOO GGG RRR BBB

    YYY
    YYY
    YYY
*/
print_cube(cube);
//Validate the colors on the cube for impossible cubies. This does not check if the scramble is solvable.
//Returns bool
validate(cube);
//returns pointer to string containing all algorithms used to solve the cube, separated by newlines and the names of the steps
//(eg. Cross, F2L, OLL: Sune, PLL: Y perm)
//solves the cube in the given array
char *solution = solve(cube);
//Generates a cube from an algorithm and returns pointer to its solution
char *solution2 = solve_scramble("scramble");
//Solves the (yellow) cross.
//Returns a pointer to a string containing the solve algorithm, with each solved edge separated by newlines
//Modifies the cube array to have a solved cross
char *cross = solve_cross(cube);
//Solves the first two layers of the cube assuming a solved cross.
//Returns a string containing the solve algorithm, with each solved pair separated by newlines
//Modifies the cube array to solve its f2l
char *f2l = solve_f2l(cube);
//Looks up the right OLL algorithm and runs it, assuming a solved F2L
//Returns the name of the OLL, and the algorithm, separated by a newline
char *oll = solve_oll(cube);
//Looks up the right PLL algorithm and runs it, assuming a solved OLL.
//Returns the name of the PLL, and the algorithm, separated by a newline
char *pll = solve_pll(cube);
```
#### Usage with python (todo)