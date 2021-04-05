# A simple command line interface for the cube solver

import ctypes
import numpy
# instructions for the 2d array from https://stackoverflow.com/questions/58727931/how-to-pass-a-2d-array-from-python-to-c
# the "bad" solution int he question worked for me because I need a 2d array and not a pointer
# c libraries in python from https://docs.python.org/3/extending/extending.html

# Give a ctypes 2d int array an easier name
array_2d_int = numpy.ctypeslib.ndpointer(
    dtype=ctypes.c_int, ndim=2, flags='CONTIGUOUS')
# Load the solver library
solver = ctypes.CDLL("./bin/solver_library.so")
# Set the argument types. String for one, 2d int array for the rest
solver.setup.argtypes = [ctypes.c_char_p]
solver.solve.argtypes = [array_2d_int]
solver.print_cube.argtypes = [array_2d_int]
solver.validate.argtypes = [array_2d_int]
solver.solve_cross.argtypes = [array_2d_int]
solver.solve_f2l.argtypes = [array_2d_int]
solver.solve_oll.argtypes = [array_2d_int]
solver.solve_pll.argtypes = [array_2d_int]
# Set the argument types for the functions that return something else than an int. Basically all char arrays.
solver.solve.restype = ctypes.c_char_p
solver.solve_cross.restype = ctypes.c_char_p
solver.solve_f2l.restype = ctypes.c_char_p
solver.solve_oll.restype = ctypes.c_char_p
solver.solve_pll.restype = ctypes.c_char_p
# List of possible errors.
errors = ["Unable to load algorithms",
          "Invalid color combination",
          "Bug in cross algorithm. Please report scramble to d4m4s74 in discord\n",
          "Bug in f2l algorithm. Please report scramble to d4m4s74 in discord\n",
          "OLL Parity.\nOne of the cubies is flipped or twisted\n",
          "PLL Parity\nTwo cubies are switched\n"]
# Set up a default cube. This one is just used for printing the solved cube in the beginning of the script.
cubelist = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
            [1, 1, 1, 1, 1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3, 3, 3, 3, 3],
            [4, 4, 4, 4, 4, 4, 4, 4, 4],
            [5, 5, 5, 5, 5, 5, 5, 5, 5]]
# Create a 2d array containing c ints from the aforementioned default solved cube
cube = numpy.array(cubelist).astype(ctypes.c_int)
# Use a C function to print the cube (because I'm too lazy to write it again in python)
solver.print_cube(cube)
# Default explanation text, and input for the colors
print("This application solves rubik's cubes the same way a human would")
print("Colors are first letters of:\nRed\nGreen\nOrange\nBlue\nWhite\nYellow")
print("Input the colors of the faces left to right, top to bottom as if you're facing the face (ex rygbogwbo)")
up = input("Up: ")
front = input("Front: ")
right = input("Rignt: ")
back = input("Back: ")
left = input("left: ")
down = input("Down: ")
# Dictionary to turn the letters into numbers my C program unerstands
colors = {"g": 0, "r": 1, "b": 2, "o": 3, "w": 4, "y": 5,
          "G": 0, "R": 1, "B": 2, "O": 3, "W": 4, "Y": 5}
if not (len(up) == len(front) == len(right) == len(back) == len(left) == len(down) == 9):
    print("wrong amount of colors on one of the faces")
    quit()
# Conversion of the letters into ints, and input validation.
for i, c in enumerate(front):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[0][i] = colors[c]
for i, c in enumerate(right):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[1][i] = colors[c]
for i, c in enumerate(back):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[2][i] = colors[c]
for i, c in enumerate(left):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[3][i] = colors[c]
for i, c in enumerate(up):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[4][i] = colors[c]
for i, c in enumerate(down):
    if not c in colors:
        print(c, "is not a color")
        quit()
    else:
        cubelist[5][i] = colors[c]
# Turn the cubelist matrix into a 2d array again.
cube = numpy.array(cubelist).astype(ctypes.c_int)
# Print the scrambled cube
solver.print_cube(cube)
# Load the scrambles from the data folder
if solver.setup("data".encode('utf-8')) != 1:
    print(errors[0])
    quit()
# Check if the colors are correct (there are no impossible cubies). This does not check if the cube is solvable
if solver.validate(cube) != 1:
    print(errors[1])
    quit()
# Solve the cross
alg = solver.solve_cross(cube)
# Check if it works
if (alg is None):
    print(errors[2])
    quit()
# If nothing fails, print the algorithm and the cube
print("Cross\n" + alg.decode("utf-8"))
solver.print_cube(cube)
# Solve the F2L
alg = solver.solve_f2l(cube)
# Check for errors
if (alg is None):
    print(errors[3])
    quit()
# Print the alg solved f2l
print("F2L\n" + alg.decode("utf-8"))
solver.print_cube(cube)
# Solve the OLL
alg = solver.solve_oll(cube)
if (alg is None):
    print(errors[4])
    quit()
print("OLL: " + alg.decode("utf-8"))
solver.print_cube(cube)
# Solve the PLL
alg = solver.solve_pll(cube)
if (alg is None):
    print(errors[5])
    quit()
print("PLL: " + alg.decode("utf-8"))
solver.print_cube(cube)
