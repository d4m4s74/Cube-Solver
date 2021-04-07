import numpy
from flask import Flask, render_template, request, jsonify, send_from_directory
import ctypes
import atexit
import json
import datetime
import csv
import os

app = Flask(__name__)

# instructions for the 2d array from https://stackoverflow.com/questions/58727931/how-to-pass-a-2d-array-from-python-to-c
# the "bad" solution in the question worked for me because I need a 2d array and not a pointer
# c libraries in python from https://docs.python.org/3/extending/extending.html
# Setup for the cube solver
# Give a ctypes 2d int array an easier name
array_2d_int = numpy.ctypeslib.ndpointer(
    dtype=ctypes.c_int, ndim=2, flags='CONTIGUOUS')
# Load the solver library
solver = ctypes.CDLL("./bin/libcubesolver.so")
# Set the argument types. String for one, 2d int array for the rest
solver.setup.argtypes = [ctypes.c_char_p]
solver.run_algorithm.argtypes = [array_2d_int, ctypes.c_char_p]
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
solver.setup("data".encode('utf-8'))
# The order of this cube is assuming green is 0, red is 1, etc. white is 4, yellow is 5.
solvedcube = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
              [1, 1, 1, 1, 1, 1, 1, 1, 1],
              [2, 2, 2, 2, 2, 2, 2, 2, 2],
              [3, 3, 3, 3, 3, 3, 3, 3, 3],
              [4, 4, 4, 4, 4, 4, 4, 4, 4],
              [5, 5, 5, 5, 5, 5, 5, 5, 5]]


# stolen from stackoverflow: decomment. Which removes comments from CSVs
# url: https://stackoverflow.com/questions/14158868/python-skip-comment-lines-marked-with-in-csv-dictreader
def decomment(csvfile):
    for row in csvfile:
        raw = row.split('#')[0].strip()
        if raw:
            yield raw

# Function that gets the steps of solving a cube and returns a list of dicts
def getsteps(cube):
    # empty step list
    steps = []
    # every step could generate an AttributeError because it's trying to decode null, this should not occur because my cube.js file validates everything side.
    # because I don't need to return an error here, I just put everything in a try except
    try:
        # solve the cross
        alg = solver.solve_cross(cube).decode("utf-8")
        # add the cross to the steps
        steps.append({'step': "Cross", 'name': "",
                      'algorithms': alg.strip().split("\n")})
        # solve the F2L
        alg = solver.solve_f2l(cube).decode("utf-8")
        # add the F2L to the steps
        steps.append({'step': "F2L", 'name': "",
                      'algorithms': alg.strip().split("\n")})
        # solve the OLL
        alg = solver.solve_oll(cube).decode("utf-8")
        # add the OLL to the steps
        steps.append({'step': "OLL", 'name': alg.split("\n")[
                     0], 'algorithms': alg.strip().split("\n")[1:]})
        # solve the PLL
        alg = solver.solve_pll(cube).decode("utf-8")
        steps.append({'step': "PLL", 'name': alg.split("\n")[
                     0], 'algorithms': alg.strip().split("\n")[1:]})
        return steps
    except:
        # in case of an exception, there might be an error with my validator, or my js file.
        with open("data/errors.txt", "a") as errorFile:  # save the error
            now = datetime.datetime.now()
            errorFile.write(now.strftime(
                "%Y-%m-%d %H:%M:%S getsteps: ") + jsonify(cube))
        return list()


def cleanup():
    solver.cleanup_last_layer()


atexit.register(cleanup)


# Only used for testing purposes: generates solution from scramble algorithm
@app.route('/solver')
def solverInterface():
    scramble = request.args.get("scramble", "").encode('utf-8')
    if not scramble:
        return render_template("solver.html", steps=list())

    # Create a 2d array containing c ints from the aforementioned default solved cube
    cube = numpy.array(solvedcube).astype(ctypes.c_int)
    solver.run_algorithm(cube, scramble)
    steps = getsteps(cube)
    return render_template("solver.html", steps=steps)


@app.route("/api/solver", methods=['GET', 'POST'])
def solverJSON():
    # gets scramble algorithm, and pattern from get or post
    if request.method == 'POST':
        scramble = request.form.get("scramble", "").encode('utf-8')
        patternJSON = request.form.get("pattern", "")
    else:
        scramble = request.args.get("scramble", "").encode('utf-8')
        patternJSON = request.form.get("pattern", "")
    # if there are no scramble and pattern json, return an empty list
    if not scramble and not patternJSON:
        return jsonify(list())
    # if there is a pattern, turn it into a numpy array
    if patternJSON:
        pattern = json.loads(patternJSON)
        cube = numpy.array(pattern).astype(ctypes.c_int)
    # if not, take a solved cube
    else:
        cube = numpy.array(solvedcube).astype(ctypes.c_int)
    # scramble the cube using the given scramble algorithm
    solver.run_algorithm(cube, scramble)
    # generate a solution
    steps = getsteps(cube)
    # return the solution
    return jsonify(steps)

# this function basically solves the cube, just to see if it's valid.


@app.route("/api/validator", methods=['GET', 'POST'])
def validatorJSON():
    if request.method == 'POST':
        patternJSON = request.form.get("pattern", "")
    else:
        patternJSON = request.args.get("pattern", "")
    if not patternJSON:
        return jsonify(1)  # 1 stands for no pattern
    pattern = json.loads(patternJSON)
    # Create a 2d array containing c ints from the aforementioned cube
    cube = numpy.array(pattern).astype(ctypes.c_int)
    if solver.validate(cube) == 0:  # If it's an invalid cube
        return jsonify(2)
    # This only happens if there's a bug in the solver
    if solver.solve_cross(cube) is None:
        with open("data/errors.txt", "a") as errorFile:  # save the error
            now = datetime.datetime.now()
            errorFile.write(now.strftime(
                "%Y-%m-%d %H:%M:%S CROSS: ") + patternJSON)
        return jsonify(5)
    # This only happens if there's a bug in the solver
    if solver.solve_f2l(cube) is None:
        with open("data/errors.txt", "a") as errorFile:  # save the error
            now = datetime.datetime.now()
            errorFile.write(now.strftime(
                "%Y-%m-%d %H:%M:%S F2L: ") + patternJSON)
        return jsonify(5)
    if solver.solve_oll(cube) is None:
        return jsonify(3)
    if solver.solve_pll(cube) is None:
        return jsonify(4)
    return jsonify(0)


@app.route('/')
@app.route("/cube")
def cube():
    with open('data/patterns.csv', 'r') as fp:
        patterns = csv.DictReader(decomment(fp))
        return render_template('cube.html', patterns=patterns)


@app.route('/favicon.ico')
def favicon():
    return send_from_directory(os.path.join(app.root_path, 'static'),
                               'favicon.ico', mimetype='image/vnd.microsoft.icon')


if __name__ == '__main__':
    app.debug = True
    app.run(host='127.0.0.1', port=5000)
