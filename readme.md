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
Remove the Linux version of bin/solver_library.so manually or using `make clean` and recompile using `make library`.  
Install Flask and Numpy using pip.
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
#### Windows:
Download or clone the files from this repository.
Remove the Linux version of bin/solver_library.so manually.  
Compile all .c files in src except for solver.c to bin/solver_library.so. Or if using a different name, change line 19 in app.py to reflect that.  
Install Flask and Numpy using pip.
```
python3 -m pip -r requirements.txt
```
Either use the command `flask run`, or `python3 app.py` to run the webserver.  
Go to https://127.0.0.1:5000/ to use the solver.
### Solver_Library
