# Rubik's Cube Solver
#### Video Demo:  https://www.youtube.com/watch?v=A6JyElruhwE
#### Demo: http://d4m4s74.pythonanywhere.com/
#### Description:
My Final Project for CS50 is a "Human Method" Rubik's Cube Solver.  
This means that the entered Rubik's Cube is solved using a method a human could use (CFOP), with clear steps: Cross, F2L, OLL and PLL.  
The final product consists of a shell application, a C library for use in other projects, and a web interface writen in Flask, HTML, CSS and Javascript.

I decided to create a Rubik's Cube solver because I have made a Sudoku solver before in C++, and the cube was a step up.  
Instead of a 2d plane, I have a semi-3d plane, with edges and corners.  
Because AI solvers have been done, and I'm not confident enough of my AI writing abilities I decided to make the program solve the cube the way I do:  
Cross, F2L, OLL and PLL

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
Install Flask and Numpy using pip:
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
#### Windows:
Download or clone the files from this repository.
Remove the Linux version of bin/libcubesolver.so manually.  
Compile all .c files in src except for solver.c to bin/libcubesolver.so. Or if using a different name, change line 19 in app.py to reflect that.  
Install Flask and Numpy using pip:
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
Link with `-lcubesolver` or just compile into the application as if it's a .o file
```C
#include <cubesolver.h>
#include <stdbool.h>
//Use either setup, or both load_olls and load_plls.
//Load all OLLs and PLLs into memory. Path is the folder where the olls.csv and plls.csv file are located.
//Returns indicating for succes or failure.
setup(path);
//Loads the OLLs from a CSV file. Returns bool indicating success or failure.
load_olls(filename);
//Loads the PLLs from a CSV file. Returns bool indicating success or failure.
load_plls(filename);
//Create an array to hold the cube. 6 faces, 9 squares per face.
//The faces in order are Front, Right, Back, Left, Up and Down.
//The "Colors" are saved as the numbers 0 to 5.
int cube[6][9];
//Add the "colors" of the cube to the array as 9 character strings containing numbers 0 to 5.
color_cube(cube, front, right, back, left, up, down);
//Run a multiple move algorithm on the cube, using standard cube notation. (Useful for scrambling)
run_algorithm(cube, "Algorithm");
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
//Returns bool.
if (!validate(cube))
{
    return "Invalid color combinaton";
}
//returns pointer to string containing all algorithms used to solve the cube, separated by newlines,
//and the names of the steps. (eg. Cross, F2L, OLL: Sune, PLL: Y perm)
//Modifies the array to its solved position
char *solution = solve(cube);
// Returns:
/*
Cross
(R D' F D)
(y) (R D' F D)
(y) (R D' F D)
(y) (R D' F D)

F2L
(y) R U R' U R U' R') (d' L U L')
(y2) (L' U' L) (y') (U' F' U F) (R' F R F')
(d2 R' U2 R2 U R2 U R)
(y) (U R U R' U2) (d R' U2 R) (U' R B' R' B)

OLL: Kite
(y2) (R U R' U') R' F R2 U R' U' F'

PLL: G perm c
(y2) L' R' U2 L R (y) L U' R U2 L' U R'
*/

//Generates a cube from an algorithm and returns pointer to its solution, or an error if unsolvable.
char *solution2 = solve_scramble("scramble");
//Solves the (yellow) cross.
//Returns a pointer to a string containing the solve algorithm, with each solved edge separated by newlines.
//May also return an error if unsolvable.
//Modifies the cube array to have a solved cross.
char *cross = solve_cross(cube);
//Solves the first two layers of the cube assuming a solved cross.
//Returns a string containing the solve algorithm, with each solved pair separated by newlines.
//Modifies the cube array to solve its f2l.
//Returns null if the cube is unsolvable.
char *f2l = solve_f2l(cube);
//Looks up the right OLL algorithm and runs it, assuming a solved F2L.
//Returns the name of the OLL, and the algorithm, separated by a newline.
//Returns null if the cube is unsolvable.
char *oll = solve_oll(cube);
//Looks up the right PLL algorithm and runs it, assuming a solved OLL.
//Returns the name of the PLL, and the algorithm, separated by a newline.
//Returns null if the cube is unsolvable.
char *pll = solve_pll(cube);
```
#### Usage with python
See C version to see what each function does.  
Because it's hard to free malloced memory in python, use "safe" versions of the solve functions and use free_strings() after use.
```python
import numpy
import ctypes
# Most functions will need a c 2d int array. Let's give it a name so it's easy to use.
array_2d_int = numpy.ctypeslib.ndpointer(
    dtype=ctypes.c_int, ndim=2, flags='CONTIGUOUS')
# First load the solver library using ctypes CDLL.
solver = ctypes.CDLL("/path/to/libcubesolver.so")
# Then we set up all functions we might want to use.
# Setup, load_olls and load_plls require a string.
solver.setup.argtypes = [ctypes.c_char_p]
solver.load_olls.argtypes = [ctypes.c_char_p]
solver.load_olls.argtypes = [ctypes.c_char_p]
# Run_algorithm requires a 2d array for the cube, and a string.
solver.run_algorithm.argtypes = [array_2d_int, ctypes.c_char_p]
# All other functions require just the 2d array.
solver.print_cube.argtypes = [array_2d_int]
solver.validate.argtypes = [array_2d_int]
solver.solve_safe.argtypes = [array_2d_int]
solver.solve_cross_safe.argtypes = [array_2d_int]
solver.solve_f2l_safe.argtypes = [array_2d_int]
solver.solve_oll_safe.argtypes = [array_2d_int]
solver.solve_pll_safe.argtypes = [array_2d_int]
# For functions that return something other than an int (or bool) we also need to set the response type.
solver.solve_safe.restype = ctypes.c_char_p
solver.solve_cross_safe.restype = ctypes.c_char_p
solver.solve_f2l_safe.restype = ctypes.c_char_p
solver.solve_oll_safe.restype = ctypes.c_char_p
solver.solve_pll_safe.restype = ctypes.c_char_p
# Load the olls and plls csvs. in my case they're in the data folder.
# Use .encode('utf-8') to convert the python string to a c string.
solver.setup("data".encode('utf-8'))
# Set up a cube. The inner lists, in order, are Front, Right, Back, Left, Up and Down.
# By default 0 = green, 1 = red, 2 = blue, 3 = orange, 4 = white, 5 = yellow.
solvedcube = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
              [1, 1, 1, 1, 1, 1, 1, 1, 1],
              [2, 2, 2, 2, 2, 2, 2, 2, 2],
              [3, 3, 3, 3, 3, 3, 3, 3, 3],
              [4, 4, 4, 4, 4, 4, 4, 4, 4],
              [5, 5, 5, 5, 5, 5, 5, 5, 5]]
# Turn it into a C 2d array.
cube = numpy.array(solvedcube).astype(ctypes.c_int)
# Run a scramble on the array. Use .encode('utf-8') to change the python string to a c string.
solver.run_algorithm(
    cube, "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2".encode('utf-8'))
# Print the cube to the shell.
solver.print_cube(cube)
# Validate the color comibinations on the cube.
if not validate(cube):
    return "Invalid color combinaton"
# Solve the cube using solver.solve.
# Note: running the function also modifies the cube array.
solution = solver.solve_safe(cube).decode("utf-8")
# Returns:
"""
Cross
(R D' F D)
(y) (R D' F D)
(y) (R D' F D)
(y) (R D' F D)

F2L
(y) R U R' U R U' R') (d' L U L')
(y2) (L' U' L) (y') (U' F' U F) (R' F R F')
(d2 R' U2 R2 U R2 U R)
(y) (U R U R' U2) (d R' U2 R) (U' R B' R' B)

OLL: Kite
(y2) (R U R' U') R' F R2 U R' U' F'

PLL: G perm c
(y2) L' R' U2 L R (y) L U' R U2 L' U R'
"""
# Or returns an error if the cube is unsolvable.

# You can also do the steps separately.
# solve the cross.
cross = solver.solve_cross_safe(cube).decode("utf-8")
# returns
'''
(R D' F D)
(y) (R D' F D)
(y) (R D' F D)
(y) (R D' F D)
'''
# solve the F2L.
f2l = solver.solve_f2l_safe(cube).decode("utf-8")
# returns
'''
(y) R U R' U R U' R') (d' L U L')
(y2) (L' U' L) (y') (U' F' U F) (R' F R F')
(d2 R' U2 R2 U R2 U R)
(y) (U R U R' U2) (d R' U2 R) (U' R B' R' B)
'''
# solve the OLL.
oll = solver.solve_oll_safe(cube).decode("utf-8")
# returns
'''
Kite
(y2) (R U R' U') R' F R2 U R' U' F'
'''
# solve the PLL.
pll = solver.solve_pll_safe(cube).decode("utf-8")
# returns
'''
G perm c
(y2) L' R' U2 L R (y) L U' R U2 L' U R'
'''
# If a step is already solved, the functions return an empty string.
# With an unsolvable cube, these functions return NULL, which causes an
# AttributeError when trying to decode. You may want to verify before decoding. (or use try)

# Clean up the memory for the solution strings.
solver.free_strings()
# Finally clean up the loaded OLLs or PLLs to prevent memory leaks
solver.cleanup_last_layer()


```
## Structure
CS50 wants me to explain what every file does, and what's in it. So this readme is longer than all but 3 of my files.
<pre>
├── <a href="#apppy">app.py</a>  
├── bin  
│   ├── <a href="#solver-library">libcubesolver.so</a>  
│   └── <a href="#solver">solver</a>  
├── <a href="#data">data</a>  
│   ├── <a href="#errorstxt">errors.txt</a>  
│   ├── <a href="#ollscsv">olls.csv</a>  
│   ├── <a href="#patternscsv">patterns.csv</a>  
│   └── <a href="#pllscsv">plls.csv</a>  
├── <a href="#makefile">makefile</a>  
├── README.md  
├── <a href="#requirementstxt">requirements.txt</a>  
├── <a href="#src">src</a>  
│   ├── <a href="#crossc-and-crossh">cross.c</a>  
│   ├── <a href="#crossc-and-crossh">cross.h</a>  
│   ├── <a href="#cubec-and-cubeh">cube.c</a>  
│   ├── <a href="#cubec-and-cubeh">cube.h</a>  
│   ├── <a href="#solver_libraryc-and-cubesolverh">cubesolver.h</a>  
│   ├── <a href="#f2lc-and-f2lh">f2l.c</a>  
│   ├── <a href="#f2lc-and-f2lh">f2l.h</a>  
│   ├── <a href="#lastlayerc-and-lastlayerh">lastlayer.c</a>  
│   ├── <a href="#lastlayerc-and-lastlayerh">lastlayer.h</a>  
│   ├── <a href="#solverc">solver.c</a>  
│   ├── <a href="#solver_libraryc-and-cubesolverh">solver_library.c</a>    
│   ├── <a href="#utilsc-and-utilsh">utils.c</a>  
│   └── <a href="#utilsc-and-utilsh">utils.h</a>  
├── test  
│   ├── <a href="#randomtestsc">randomtests.c</a>  
│   └── randomtests   
├── static  
│   ├── <a href="#cubecss">cube.css</a>  
│   ├── <a href="#cubejs">cube.js</a>  
│   ├── <a href="#cubeinterfacejs">cubeinterface.js</a>  
│   ├── favicon.ico  
│   ├── next.png  
│   ├── pause.png  
│   ├── play.png  
│   └── prev.png  
├── <a href="#templates">templates</a>  
│   ├── <a href="#cubehtml">cube.html</a>  
│   └── <a href="#solverhtml">solver.html</a>  
</pre>
### src
#### cube.c and cube.h
Cube.c and cube.h contain all the code that controls the rubik's cube in memory.

A rubik's cube, in memory, is a 6 by 9 2d array. The outer array contains the six faces: Front, Right, Back, Left, Up and Down.  
I chose this order because the up and down faces are always the same, so I can hardcode them, but the sides only need to be correct in relation to each other. This means I can use simple plus or minus, combined with modulus to translate the tests and moves to every side.
The same is for colors, which are saved as numbers 0 through 5, 0 being the front color, 1 being right, etc.  
I first chose the color order I use: Red in front, yellow on top, green to the right. But later when writing my python code and interface I switched to the "Official" green in front, white on top so I don't need to rotate the cube entered by the user to have white on the bottom.

##### color_cube()
First I made the color_cube() function. This applies a pattern of your choice to the cube using 6 strings.  
It uses simple char math to change char '0', '1', etc. to ints and stores them the given array.  
I only use this once or twice as a one liner to set up a solved cube in one line, because when translating user input to something the computer understands you might as well enter the ints individually instead of first translating it to a string.
##### print_cube()
Print_cube() outputs the supplied cube to the terminal. This is used a lot when debugging, and for the terminal application.
I started by outputting just the numbers 0 through 5 in the shape of an exploded rubik's cube.
```
    444
    444
    444

333 000 111 222
333 000 111 222
333 000 111 222

    555
    555
    555
```
Because it would make more lines of code to automate this I simply use 9 print statements each containing up to 12 variables.  
Later I found out I could add shell codes like `\e[42m` to give these numbers a background color. Adding those gave me 28 variables per print.  
Finally I thought of simply putting the letter or number for the color in the array that holds the shell codes, bringing it back to 12.
Unfortunately Windows doesn't support this type of shell codes, so I had to keep using just numbers, and later letters.
##### move_cube()
Move_cube() is the first function for actually changing the cube.
This is a recursive function that does the requested move the requested amount of times.
There are 9 possible moves, numbered 0 through 8. The default front, right, back, etc. But also the middle, equator and standing layer.
Using a helper function I wrote in utils.c called Cycle I switch the values of each square on each face correspoding the move clockwise, repeating if necessary.
##### run_algorithm()
Because moves usually don't happen on their own, next I wrote run_algorithm(). This function uses move_cube() to run an algorithm on the cube supplied in standard cube notation. This gives me the ability to do multiple moves at once (or actually in order), and do moves that have effect on 2 or even 3 layers like x, y and z.
##### validate()
Finally, validate(). I added this function while writing the Python interface because you should never trust your user not to enter weird things.  
This function does not check if the rubik's cube is solvable. It checks if the colors are correct. For example, one cubie can not have both white and yellow stickers. And going clockwise it's possible to have green, white and red stickers in that order, but not the other way around.  
In order to do this I partially copied the idea of the locked array in Tideman, using a 2d boolean array for the corners, and a 3d boolean array for the edges to see if a combination of 2 or 3 colors is possible. This code went through multiple iterations. First checking each edge and corner separately, and later using a few for loops.

#### cross.c and cross.h
Cross.c contains all code needed to solve the cross of the cube. Because I use a "Human method" (CFOP) to solve the cube, I'm kind of forced to use huge decision trees.  
I rewrote almost this whole file last minute so it doesn't have to align the edges every step of the way.  
##### findZero()
Finds where on the bottom layer the yellow-green edge is, or would be if it's not there yet. This makes it easier to align the other edges in fewer steps because they can align to "relative zero" instead of actual zero.
##### crossCase1() through 6
These 6 cases are every way an edge can be oriented, Pointing down, pointing up, pointing out in the up and down layers, pointing left and pointing right on the E layer.  
The functions basically scan the faces in order from 0 to 3 to look for pieces that belong in the cross, rotate them into place, and store the moves in a char array using a helper function I call append().
##### solve_cross()
Solve_cross first orients the cube so yellow is down. Then it does the 6 cross cases in order. If one returns something other than null it goes back to the first case.  
Once all 4 edges are solved the function returns the algorithms. If all cases are tested without success, or after 4 loops the function returns NULL.

#### f2l.c and f2l.h
The same idea as cross.c, except way bigger. 3600 lines of code to solve all the cases. 
##### f2lCase1() through 11
These 11 cases consist of every way a corner can be oriented, and some combinations of corners and edges.  
To save time and lines most functions don't completely solve a corner/edge combination, but bring them to the shortest known state and call the correct function.  
The number 1 through 11 are the order they were written in, not always the order they're run in.
##### solve_f2l()
Solve_f2l first verifies the cross is solved. If not it returns null.  
After that solve_f2l goes through the cases in order of shortest average algorithm to longest, going back to 1 once a function returns an algorithm. If all cases are tested without succss, or after 4 loops the function returns NULL.

#### lastlayer.c and lastlayer.h
The file I'm most proud of. No giant loops, no decision trees. Just smart math and lookup tables.
##### struct oll, and pll
Oll and pll are two structs meant to store OLL and PLLs. They store the name, the pattern I use to recognize them, and the algorithm to solve them.  
On a sleepless night I figured out I could recognize PLL using just the colors of the outer ring, and the OLL by just 12 booleans meaning yellow, or not yellow.  
##### load_olls() and load_plls()
Load_olls() and load_plls() load the names, patterns and algorithms from a CSV file. This went through some iterations.  
I first started by getline() to get each line of the CSV files, and splitting the string at 24, characters and 36 characters, padding the name with spaces.  
This wasn't pretty so I started looking for the commas and splitting things there.  
Finally someone on the discord directed me to sscanf() allowing me to easily, cleanly split each string.  
I also switched to fgets() to make getting the line more cross compatible.  
Lines starting with # are rejected because they are comments. Lines where the pattern isn't purely numeric are recognized using a helper function called isNumber() or isBinary() and are rejected, and while writing this section I added a test to see if the name, pattern and algorithm are the right length to stop out of bound reads and writes.  
In the olls and plls array space for the names and algorithms is created using malloc and the information is copied in using strncpy. The pattern is turned into an array of 12 ints or booleans.
##### find_oll() and find_pll()
Find_pll() is my pride and joy. It was the first algorithm I wrote for solving the cube (before the cross and f2l), and it came to me in a dream.  A way to check the oll pattern for any color combination with some simple maths. 
I literally woke up and wrote down `int case[12] containing the outer ring. if (front[0] == (front[1]-(case[0]-case[1])%4))`. The next day I simplified it to `(faces[0][0] == (faces[0][1] - (plls[i].pattern[0] - plls[i].pattern[1])) % 4)` (times 12). Then `(faces[j][0] - faces[j][1]) % 4 == (plls[i].pattern[0] - plls[i].pattern[1]) % 4` and finally after many iterations I ended up with `mod((cube[(j + i / 3) % 4][i % 3] + dif[j]), 4)` allowing me to encode the pattern of the top layer of the cube using a 12 int array, for easy comparison with all known values.  
I could have encoded the pattern as a 4 byte int, but I decided not to because I think it's better to make the patterns easy to understand than save 44 bytes (3408 kb total for patterns)  
Finally the function returns a pair_int_int containing the orientation of the layer, and the correct OLL or PLL. Or two negative numbers if nothing is found.
##### solve_oll() and solve_pll()
The functions solve_oll() and solve_pll() are relatively simple. They call find_oll or pll to get the right index and orientation (amount of y moves). Then the function turns the cube using an y move, runs the right OLL or PLL algorithm, and in case of solve_pll() turns the U layer to finally solve the cube. The functions return the algorithm used to solve the cube in standard cube notation.
##### cleanup_last_layer()
Frees the memory used for storing all OLLs and PLLs. 
#### utils.c and utils.h
Some functions to make my life a bit easier.
##### mod()
Because the normal % doesn't work with negative numbers, this is "my own version" that does. With special thanks to the cs50 discord.
##### make_pair_int_int()
For some functions I want to return more than one variable. This one is based on the C++ pair<int, int> and make_pair(). It creates a struct with two ints: first and second. In this case used to return the oll/pll number and the preceding U or y move.
##### cycle()
A function that cycles 4 numbers. Basically a step up from swap. Used in cube.c to cycle the edges and corners for cube moves. There is a copy of that move in cube.js
##### append()
A function that adds a string to another string, reallocing if necessary. Used everywhere in this application.
##### isNumber() and isBinary()
Checks if a string contains only digits, or only zeroes and ones. Currently only used for loading OLLs and PLLs.
#### solver_library.c and cubesolver.h
I first wrote solver.c as an interface for my cube solver, but quickly decided I wanted a graphical UI, which I'm not ready to do in pure C.  
That's why I started to make my cubesolvers into a library for use with whatevery I want.  
##### solve()
Puts it all together to solve the cube.
It takes a cube and validates it. Then attempts to solve the cross. If it fails it returns an error, otherwise it stores the output. Then it solves the cross or returns an error. It then solves the OLL (or returns) and the PLL. Finally it returns a string containing the full solution.
##### solve_scramble()
An interface for solve. Generates a cube from an entered scramble and passes it to solve()
##### setup()
Just to make everyone's life a little easier. Takes a path, and loads the olls and plls from that path. Returns true if it works, cleans up after itself and returns false if it doesn't.
##### store_string_pointer()
Because it's hard to free memory from Python, I decided to do it from C. I have "Safe" versions of every function you might want to use from outside of C. These store the generated pointers that need to be freed to a global array for freeing all at once.
##### free_strings()
Free all stored trings
##### solve_safe(), solve_scramble_safe(), solve_cross_safe(), solve_f2l_safe(), solve_oll_safe() and solve_pll_safe()
Multiple functions return strings, that need to be freed in the end. These functions store the pointers to these strings for later deletion.
#### solver.c
For testing, and those who are better with keyboard than with mouse, a terminal interface.  
It went to multiple rewrites, finally being rewritten while writing this readme to work with solver_library instead of on its own.
##### main()
It does everything! It loads the OLLs and PLLs based on either default or user input. Then it generates a cube based on either the command line or user input, and finally solves the cube step by step, printing the cube along the way.
##### getDir()
Gets the directory from a given filepath (eg. argv[0]). Written to work with both Windows and Unix based OSses.
##### color_letter_to_number()
Changes a color letter to the corresponding int. In python I just used a dictionary. Way easier.
##### color_face_from_input()
Ask the user the nine colors of a rubik's cube's face, and puts them on a cube.
##### color_cube_interactive()
Step by step interactive interface for entering the colors onto a cube.
##### helptext()
Prints the help test.
##### usage()
Prints the usage.
### test
#### randomtests.c
A simple short application for bug testing. Generates lots of scrambles and solves them. Prints the first scramble that fails and stops.  
I used this to find one single typo. Everything else I did manually.
##### checkSolved()
Uses memcmp to compare the cube spit out by the solve algorithm with one of 4 solved cubes (one per orientation).  
Trades in a bit of memory for speed.
##### generate_scramble()
Generates random scrambles for the rubik's cube. Badly. Does not protect against double move and reverse moves. But it's good enough if the scramble is long enough.
##### main()
Loads the OLLs and PLLs from user input. Solves the cube as many times as the user wants, until it fails.
### data
#### olls.csv
Contains a list of all 58 OLL cases (including solved) for use with my C library  
Algorithms from my own memory, and from https://www.speedsolving.com/wiki/index.php/OLL
#### plls.csv
Contains a list of all 22 PLL cases (including solved) for use with my C library  
Algorithms from my own memory, and from https://www.speedsolving.com/wiki/index.php/PLL
#### patterns.csv
Contains a few patterns for scrambling the cube in pretty ways. Mostly used for testing my javascript, but is a nice feature for the user.
#### errors.txt
Hopefully contains nothing, but is meant to store any and all errors in solving the cube.
### ./
#### app.py
The bridge between my C code and javascript. It loads the C library, and initializes function I need with argtypes and if necessary restypes. Using numpy to convert from python types to ctypes.  
##### decomment()
A function I "borrowed" from stackoverflow. It takes an input stream and outputs every line up to #
##### getsteps()
The actual solver. It takes a scrambled cube and solves it step by step, returning the steps as an array of dictionaries (containing an array with algorithms). In case of exceptions it stores the pattern in data/errors.txt
##### solverInterface()
Only used for testing. Takes a scramble algorithm from GET, solves it using getsteps() and passes it to the solver template.
##### solverJSON()
Takes a pattern and scramble from GET or POST, solves it using getsteps and returns it as JSON.
##### validatorJSON()
Takes a pattern from GET or POST and solves the cube, not caring about the actual solution, but checking if and where it fails. Returns 0 for success or the error number.
##### cube()
Shows the cube.html template: the actual UI. While it does so, it also takes the patterns from data/patterns.csv to give to the template, using a dictreader and decomment to remove my comments.
##### favicon()
Passes the favicon though to the browser for the nice cube symbol.
##### cleanup()
Frees all memory from my C code: the PLLs and the strings. Registered using atexit to run at exit
#### makefile
All commands for compiling my C code on Linux or OS X. Even though my library can be used as a library I decided to use all files to compile so I can just give out one executable.
#### requirements.txt
Contains the two libraries I used that need to be installed separately. Just flask and numpy. Nothing special.
### templates
#### solver.html
Asks for a scramble, passes it to app.py to solve and prints the solution. Only used for quick testing.
#### cube.html
Contains lots of empty divs to be filled by my javascript code, and shaped by css. Contains 3 images for the playback controls, a form for entering scrambles, and a short explanation.  
Finally loads the javascript because it's nothing without the dom. Not that pretty, but very functional.
### static
Except for src contains the most actual code. Some CSS to make everything look usable, and javascript to make it actually be usable. And some images to click on.
#### cube.css
Changes the link of random divs to something you can see.  
Uses CSS grids in the default, and less default ways.  
I use an outer container to place the controls on the left, the solver animation top right, and solution bottom right.  
Using absolute positioning I put the playback controls and current alg over the animation.  
Everything else is quite default except for the colorpicker. If you look at the html code you just see a div containing 6 divs, each containing 9 divs. Using grids I shape the 6 "face" divs to look like an unfolded cube, each face contains a grid of 9 squares. This allows me to make the size variable while the shape stays the same, without needing to use images.  
I also spent a week or 2 making a mobile view, because the text seemed to be a variable size. I contacted the discord, facebook, and even stackoverflow. Finally I found a html line on stackoverflow: `<meta name="viewport" content="width=device-width, initial-scale=1.0">` which solved everything in one line.
#### cube.js
Brings my algorithm to the user. Based on three.js  
This is my first time actually writing javascript. It doesn't look pretty, probably breaks all the rules, but it works.

I rewrote this file multiple times. First iterative, that was before I started uploading to github, then functional, and then finally sort of object oriented    
First I declare all variables that need to be used by more than one function: The materials and geometry for the three.js objects, arrays to store the objects, the axes of rotation, and more.   
There is also a gigantic dictionary containing the necessary variables for every legal cube move: The cubies to cycle or swap, the axis and degrees of rotation, and what to show. This could all have fit in 72 lines, but style50 wants one variable per line, making it 649 lines.
##### constructor()
Takes the scene and saves a reference, generates the cubies, the outlines and planes in a loop, sets the positions of the cubies and outlines in a loop, and the planes manually.
##### cycle_rotations()
Helper function. Cycles the rotations of 4 cubies a > b > c > d > a using a temp cubie.
##### swap_rotations()
Helper function. Swaps the rotation of 2 cubies using a temp cubie.
##### reset_cube()
Reset the cubies to their default position and resets the solution div. This needs more steps than you'd think. First we need to reset all animation, finish it. Reset all future and past moves, and finally reset the rotation of the cubies.
##### is_moving()
Returns true or false depending on if the cube is moving.
##### do_move()
Sets up the animation for a move on the cube.  
This function was originally 160 lines of if/else statements, but I later rewrote it to get the cubies and moves from a gigantic dictionary. This makes this and the next function easier to read, and makes it so if I write a 4x4 or 5x5 version I only have to change the dictionary.
##### finish_move()
Completes the animation, and actually moves (rotates) the cubies.  
Because the cubies don't really move, the basic idea is to copy the rotation of the cubie that on a real cube would move into its place, and then rotate everything.  
This function was originally a 375 line monstrosity of nexted if/else statements. It was later rewritten to work with the moves dictionary.
##### next_move()
Does the next move in the moves list, or the next algorithm in the steps list, and shows the current algorithm on screen.
##### prev_move()
Does the previous move int he moves list in reverse, or the previous algortithm in the steps list
##### apply_pattern()
Applies the colors from the color picker (or actually the faces array) to the cube by rotating each cubie. Contains some of its own functions
Every cubie has its own line with manual modifiers. To me it's too random to automate. But if you want to try, be my guest.

###### opposite_color()  
Changes green to blue, red to orange, white to yellow, and the other way around. Now I think of it, this could have been a dict, but one line of math and 3 ifs works too.

###### apply_color()  
Rotates a cubie based on the front and left color, or the up color. Basically lots of if statements with hardcoded moves, but that's enough here.
##### run_alg()
Adds an algorithm to the moves array and starts the animation (if not paused)
##### apply_alg()
Runs an algorithm on the rubik's cube without showing the animation
##### solve_cube()
Sends both the applied pattern, and all moves done to api/solver, puts the algorithms on screen in the Solutions div, and adds all steps to the steps array and sets running to true so the animation starts.
##### set_movement_speed()
Sets the rotation speed in radians per frame. 
##### update()
If there is wait time, it does nothing except lowering the wait time by one.  
If there are x, y or z moves remaining it rotates the moving group mspeed radians.  
If there are no x, y or z moves remaining but there is a current move, it sends the move to finish_move().  

#### cubeinterface.js
The javascript for the actual interface. It imports the Cube class from cube.js and uses it. 
##### urlparam()
Copied from the jquery codebase. Function to get variables from get.
##### if (urlparam("alg") and if (urlparam("scramble"))
Not really functions but worth saying. Hidden feature. Animates an algorithm from GET, or scrambles the cube from GET. Useful when sharing algorithms with others.
##### $('.square').click
Controls the color picker. If the user picks a color and clicks a square, this piece of code gives the square its color. It also changes the corresponding field in the faces array to be applied later.
##### $('#clear').click
Returns the color picker, and the faces array back to default.
##### $('#apply').click
Validates the entered rubik's cube using the validator api, then sends the pattern to cube.apply_pattern()
##### $('#pattern').change
Takes the selected pattern and puts the algorithm in the alg field.
##### \$('#playpause').click
Sets running to false, paused to true and switches the play icon with the pause icon in the interface
##### <span>$</span>('#next').click and <span>$</span>('#prev').click
Pauses animation and runs either next_move or prev_move
##### animate()
Runs every frame and controls the 3d animation.  
It first sets up an animation frame (default 3.js function).  
It updates the cube.    
Finally it updates the camera and renders everything.
