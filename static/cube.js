import * as THREE from 'https://unpkg.com/three/build/three.module.js';
import { OrbitControls } from 'https://unpkg.com/three/examples/jsm/controls/OrbitControls.js';


class Cube {
    geometry = new THREE.BoxGeometry(1.985, 1.985, 1.985); //including outline it's a perfect 2
    //give it colors
    cubeMaterials = [
        new THREE.MeshBasicMaterial({ color: 'red', transparent: true, opacity: 1, side: THREE.DoubleSide, }), //left
        new THREE.MeshBasicMaterial({ color: 'orange', transparent: true, opacity: 1, side: THREE.DoubleSide }), //right
        new THREE.MeshBasicMaterial({ color: 'white', transparent: true, opacity: 1, side: THREE.DoubleSide }), //up
        new THREE.MeshBasicMaterial({ color: 'yellow', transparent: true, opacity: 1, side: THREE.DoubleSide }), //down
        new THREE.MeshBasicMaterial({ color: 'green', transparent: true, opacity: 1, side: THREE.DoubleSide }), //front
        new THREE.MeshBasicMaterial({ color: 'blue', transparent: true, opacity: 1, side: THREE.DoubleSide }), //back
    ];
    planeGeometry = new THREE.PlaneGeometry(6, 6);
    planeMaterial = new THREE.MeshBasicMaterial({ color: 0x222222, side: THREE.DoubleSide });
    //draw the outlines:
    edges = new THREE.EdgesGeometry(this.geometry);
    //create a 27 this.cubies
    cubies = [];
    outlines = [];
    planes = [];
    //cube used to store temporary values
    tempcube = new THREE.Mesh(this.geometry, this.cubeMaterials);
    //Create a group for the pieces currently moving
    moving = new THREE.Group();
    //variables for the moves being made
    xRemaining = 0;
    yRemaining = 0;
    zRemaining = 0;
    //variable for the piece currently moving
    cur = "";
    //Whether it's running an algorithm
    running = false;
    //whether or not running is paused (for stepping through manually)
    paused = false;
    //The wait time until the next step
    wait = 0;
    //The default movement speed in radians per frame
    mspeed = 0.1;
    //The step of a solution being done
    stepnumber = 0;
    steps = [];
    //the step in the algorithm being done.
    movenumber = 0;
    moves = [];
    //A list of moves done to the cube, in order to step back or send to the solver as a scramble
    movesDone = []
    //Axes of rotation
    xAxis = new THREE.Vector3(1, 0, 0);
    yAxis = new THREE.Vector3(0, 1, 0);
    zAxis = new THREE.Vector3(0, 0, 1);
    //The pattern for the default solved cube
    solvedCube = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1, 1],
    [2, 2, 2, 2, 2, 2, 2, 2, 2],
    [3, 3, 3, 3, 3, 3, 3, 3, 3],
    [4, 4, 4, 4, 4, 4, 4, 4, 4],
    [5, 5, 5, 5, 5, 5, 5, 5, 5]];
    solver_api = "";
    solution_div = "";
    current_alg_div = "";
    applied;
    scene;

    /*
        A huge dictionary with the info needed to do moves. This so I can change do_move and finish_move from a huge decision tree to a few for loops
        Cycles contain sets of four cubies that need to be cycled. Swaps contain sets of 2 cubies that need to be swapped.
        Centers contain the center cubie of the slices
        Axis is the axis of rotation
        Rotation is the degrees to rotate in radians.
        Static are the planes that don't move
        Moving are the planes that do move.
    */
    moveInstructions = {
        "U": {
            'cycles': [[2, 8, 6, 0], [5, 7, 3, 1]],
            'swaps': [],
            'centers': [4],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [0],
            'moving': [1]
        },
        "U2": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5]],
            'centers': [4],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [0],
            'moving': [1]
        },
        "U2'": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5]],
            'centers': [4],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [0],
            'moving': [1]
        },
        "U'": {
            'cycles': [[6, 8, 2, 0], [3, 7, 5, 1]],
            'swaps': [],
            'centers': [4],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [0],
            'moving': [1]
        },
        "u": {
            'cycles': [[2, 8, 6, 0], [5, 7, 3, 1], [11, 17, 15, 9], [14, 16, 12, 10]],
            'swaps': [],
            'centers': [4, 13],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [2],
            'moving': [3]
        },
        "u2": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5], [9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [4, 13],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [2],
            'moving': [3]
        },
        "u2'": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5], [9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [4, 13],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [2],
            'moving': [3]
        },
        "u'": {
            'cycles': [[6, 8, 2, 0], [3, 7, 5, 1], [15, 17, 11, 9], [12, 16, 14, 10]],
            'swaps': [],
            'centers': [4, 13],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [2],
            'moving': [3]
        },
        "E": {
            'cycles': [[15, 17, 11, 9], [12, 16, 14, 10]],
            'swaps': [],
            'centers': [13],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [0, 2],
            'moving': [1, 3]
        },
        "E2": {
            'cycles': [],
            'swaps': [[9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [13],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [0, 2],
            'moving': [1, 3]
        },
        "E2'": {
            'cycles': [],
            'swaps': [[9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [13],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [0, 2],
            'moving': [1, 3]
        },
        "E'": {
            'cycles': [[11, 17, 15, 9], [14, 16, 12, 10]],
            'swaps': [],
            'centers': [13],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [0, 2],
            'moving': [1, 3]
        },
        "D": {
            'cycles': [[24, 26, 20, 18], [21, 25, 23, 19]],
            'swaps': [],
            'centers': [22],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [2],
            'moving': [3]
        },
        "D2": {
            'cycles': [],
            'swaps': [[18, 26], [20, 24], [19, 25], [21, 23]],
            'centers': [22],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [2],
            'moving': [3]
        },
        "D2'": {
            'cycles': [],
            'swaps': [[18, 26], [20, 24], [19, 25], [21, 23]],
            'centers': [22],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [2],
            'moving': [3]
        },
        "D'": {
            'cycles': [[20, 26, 24, 18], [23, 25, 21, 19]],
            'swaps': [],
            'centers': [22],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [2],
            'moving': [3]
        },
        "d": {
            'cycles': [[24, 26, 20, 18], [21, 25, 23, 19], [15, 17, 11, 9], [12, 16, 14, 10]],
            'swaps': [],
            'centers': [22, 13],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [0],
            'moving': [1]
        },
        "d2": {
            'cycles': [],
            'swaps': [[18, 26], [20, 24], [19, 25], [21, 23], [9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [22, 13],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [0],
            'moving': [1]
        },
        "d2'": {
            'cycles': [],
            'swaps': [[18, 26], [20, 24], [19, 25], [21, 23], [9, 17], [11, 15], [10, 16], [12, 14]],
            'centers': [22, 13],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [0],
            'moving': [1]
        },
        "d'": {
            'cycles': [[20, 26, 24, 18], [23, 25, 21, 19], [11, 17, 15, 9], [14, 16, 12, 10]],
            'swaps': [],
            'centers': [22, 13],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [0],
            'moving': [1]
        },
        "y": {
            'cycles': [[2, 8, 6, 0], [5, 7, 3, 1], [11, 17, 15, 9], [14, 16, 12, 10], [20, 26, 24, 18], [23, 25, 21, 19]],
            'swaps': [],
            'centers': [22, 13, 4],
            'axis': this.yAxis,
            'rotation': -Math.PI / 2,
            'static': [],
            'moving': []
        },
        "y2": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5], [9, 17], [11, 15], [10, 16], [12, 14], [18, 26], [20, 24], [19, 25],
                [21, 23]],
            'centers': [22, 13, 4],
            'axis': this.yAxis,
            'rotation': -Math.PI,
            'static': [],
            'moving': []
        },
        "y2'": {
            'cycles': [],
            'swaps': [[0, 8], [2, 6], [1, 7], [3, 5], [9, 17], [11, 15], [10, 16], [12, 14], [18, 26], [20, 24], [19, 25],
                [21, 23]],
            'centers': [22, 13, 4],
            'axis': this.yAxis,
            'rotation': Math.PI,
            'static': [],
            'moving': []
        },
        "y'": {
            'cycles': [[6, 8, 2, 0], [3, 7, 5, 1], [15, 17, 11, 9], [12, 16, 14, 10], [24, 26, 20, 18], [21, 25, 23, 19]],
            'swaps': [],
            'centers': [22, 13, 4],
            'axis': this.yAxis,
            'rotation': Math.PI / 2,
            'static': [],
            'moving': []
        },
        "R": {
            'cycles': [[20, 26, 8, 2], [11, 23, 17, 5]],
            'swaps': [],
            'centers': [14],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [4],
            'moving': [5]
        },
        "R2": {
            'cycles': [],
            'swaps': [[2, 26], [8, 20], [5, 23], [11, 17]],
            'centers': [14],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [4],
            'moving': [5]
        },
        "R2'": {
            'cycles': [],
            'swaps': [[2, 26], [8, 20], [5, 23], [11, 17]],
            'centers': [14],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [4],
            'moving': [5]
        },
        "R'": {
            'cycles': [[8, 26, 20, 2], [17, 23, 11, 5]],
            'swaps': [],
            'centers': [14],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [4],
            'moving': [5]
        },
        "r": {
            'cycles': [[20, 26, 8, 2], [11, 23, 17, 5], [19, 25, 7, 1], [10, 22, 16, 4]],
            'swaps': [],
            'centers': [14, 13],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [6],
            'moving': [7]
        },
        "r2": {
            'cycles': [],
            'swaps': [[2, 26], [8, 20], [5, 23], [11, 17], [1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [14, 13],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [6],
            'moving': [7]
        },
        "r2'": {
            'cycles': [],
            'swaps': [[2, 26], [8, 20], [5, 23], [11, 17], [1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [14, 13],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [6],
            'moving': [7]
        },
        "r'": {
            'cycles': [[8, 26, 20, 2], [17, 23, 11, 5], [7, 25, 19, 1], [16, 22, 10, 4]],
            'centers': [14, 13],
            'swaps': [],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [6],
            'moving': [7]
        },
        "M": {
            'cycles': [[7, 25, 19, 1], [16, 22, 10, 4]],
            'swaps': [],
            'centers': [13],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [4, 6],
            'moving': [5, 7]
        },
        "M2": {
            'cycles': [],
            'swaps': [[1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [13],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [4, 6],
            'moving': [5, 7]
        },
        "M2'": {
            'cycles': [],
            'swaps': [[1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [13],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [4, 6],
            'moving': [5, 7]
        },
        "M'": {
            'cycles': [[19, 25, 7, 1], [10, 22, 16, 4]],
            'swaps': [],
            'centers': [13],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [4, 6],
            'moving': [5, 7]
        },
        "L": {
            'cycles': [[6, 24, 18, 0], [15, 21, 9, 3]],
            'swaps': [],
            'centers': [12],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [6],
            'moving': [7]
        },
        "L2": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15]],
            'centers': [12],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [6],
            'moving': [7]
        },
        "L2'": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15]],
            'centers': [12],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [6],
            'moving': [7]
        },
        "L'": {
            'cycles': [[18, 24, 6, 0], [9, 21, 15, 3]],
            'swaps': [],
            'centers': [12],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [6],
            'moving': [7]
        },
        "l": {
            'cycles': [[6, 24, 18, 0], [15, 21, 9, 3], [7, 25, 19, 1], [16, 22, 10, 4]],
            'swaps': [],
            'centers': [12, 13],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [4],
            'moving': [5]
        },
        "l2": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15], [1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [12, 13],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [4],
            'moving': [5]
        },
        "l2'": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15], [1, 25], [7, 19], [4, 22], [10, 16]],
            'centers': [12, 13],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [4],
            'moving': [5]
        },
        "l'": {
            'cycles': [[18, 24, 6, 0], [9, 21, 15, 3], [19, 25, 7, 1], [10, 22, 16, 4]],
            'swaps': [],
            'centers': [12, 13],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [4],
            'moving': [5]
        },
        "x": {
            'cycles': [[18, 24, 6, 0], [9, 21, 15, 3], [19, 25, 7, 1], [10, 22, 16, 4], [20, 26, 8, 2], [11, 23, 17, 5]],
            'swaps': [],
            'centers': [12, 13, 14],
            'axis': this.xAxis,
            'rotation': -Math.PI / 2,
            'static': [],
            'moving': []
        },
        "x2'": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15], [1, 25], [7, 19], [4, 22], [10, 16], [2, 26], [8, 20], [5, 23],
                [11, 17]],
            'centers': [12, 13, 14],
            'axis': this.xAxis,
            'rotation': Math.PI,
            'static': [],
            'moving': []
        },
        "x2": {
            'cycles': [],
            'swaps': [[0, 24], [6, 18], [3, 21], [9, 15], [1, 25], [7, 19], [4, 22], [10, 16], [2, 26], [8, 20], [5, 23],
                [11, 17]],
            'centers': [12, 13, 14],
            'axis': this.xAxis,
            'rotation': -Math.PI,
            'static': [],
            'moving': []
        },
        "x'": {
            'cycles': [[6, 24, 18, 0], [15, 21, 9, 3], [7, 25, 19, 1], [16, 22, 10, 4], [8, 26, 20, 2], [17, 23, 11, 5]],
            'swaps': [],
            'centers': [12, 13, 14],
            'axis': this.xAxis,
            'rotation': Math.PI / 2,
            'static': [],
            'moving': []
        },
        "F": {
            'cycles': [[8, 26, 24, 6], [17, 25, 15, 7]],
            'swaps': [],
            'centers': [16],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [8],
            'moving': [9]
        },
        "F2": {
            'cycles': [],
            'swaps': [[6, 26], [8, 24], [7, 25], [15, 17]],
            'centers': [16],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [8],
            'moving': [9]
        },
        "F2'": {
            'cycles': [],
            'swaps': [[6, 26], [8, 24], [7, 25], [15, 17]],
            'centers': [16],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [8],
            'moving': [9]
        },
        "F'": {
            'cycles': [[24, 26, 8, 6], [15, 25, 17, 7]],
            'swaps': [],
            'centers': [16],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [8],
            'moving': [9]
        },
        "f": {
            'cycles': [[8, 26, 24, 6], [17, 25, 15, 7], [5, 23, 21, 3], [14, 22, 12, 4]],
            'swaps': [],
            'centers': [16, 13],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [10],
            'moving': [11]
        },
        "f2": {
            'cycles': [],
            'swaps': [[6, 26], [8, 24], [7, 25], [15, 17], [3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [16, 13],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [10],
            'moving': [11]
        },
        "f2'": {
            'cycles': [],
            'swaps': [[6, 26], [8, 24], [7, 25], [15, 17], [3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [16, 13],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [10],
            'moving': [11]
        },
        "f'": {
            'cycles': [[24, 26, 8, 6], [15, 25, 17, 7], [21, 23, 5, 3], [12, 22, 14, 4]],
            'swaps': [],
            'centers': [16, 13],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [10],
            'moving': [11]
        },
        "S": {
            'cycles': [[21, 23, 5, 3], [12, 22, 14, 4]],
            'swaps': [],
            'centers': [13],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [8, 10],
            'moving': [9, 11]
        },
        "S2": {
            'cycles': [],
            'swaps': [[3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [13],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [8, 10],
            'moving': [9, 11]
        },
        "S2'": {
            'cycles': [],
            'swaps': [[3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [13],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [8, 10],
            'moving': [9, 11]
        },
        "S'": {
            'cycles': [[5, 23, 21, 3], [14, 22, 12, 4]],
            'swaps': [],
            'centers': [13],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [8, 10],
            'moving': [9, 11]
        },
        "B": {
            'cycles': [[18, 20, 2, 0], [9, 19, 11, 1]],
            'swaps': [],
            'centers': [10],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [10],
            'moving': [11]
        },
        "B2": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11]],
            'centers': [10],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [10],
            'moving': [11]
        },
        "B2'": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11]],
            'centers': [10],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [10],
            'moving': [11]
        },
        "B'": {
            'cycles': [[2, 20, 18, 0], [11, 19, 9, 1]],
            'swaps': [],
            'centers': [10],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [10],
            'moving': [11]
        },
        "b": {
            'cycles': [[18, 20, 2, 0], [9, 19, 11, 1], [21, 23, 5, 3], [12, 22, 14, 4]],
            'swaps': [],
            'centers': [10, 13],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [8],
            'moving': [9]
        },
        "b2": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11], [3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [10, 13],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [8],
            'moving': [9]
        },
        "b2'": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11], [3, 23], [5, 21], [4, 22], [12, 14]],
            'centers': [10, 13],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [8],
            'moving': [9]
        },
        "b'": {
            'cycles': [[2, 20, 18, 0], [11, 19, 9, 1], [5, 23, 21, 3], [14, 22, 12, 4]],
            'swaps': [],
            'centers': [10, 13],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [8],
            'moving': [9]
        },
        "z": {
            'cycles': [[2, 20, 18, 0], [11, 19, 9, 1], [5, 23, 21, 3], [14, 22, 12, 4], [8, 26, 24, 6], [17, 25, 15, 7]],
            'swaps': [],
            'centers': [10, 13, 16],
            'axis': this.zAxis,
            'rotation': -Math.PI / 2,
            'static': [],
            'moving': []
        },
        "z2": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11], [3, 23], [5, 21], [4, 22], [12, 14], [6, 26], [8, 24], [7, 25],
                [15, 17]],
            'centers': [10, 13, 16],
            'axis': this.zAxis,
            'rotation': -Math.PI,
            'static': [],
            'moving': []
        },
        "z2'": {
            'cycles': [],
            'swaps': [[0, 20], [2, 18], [1, 19], [9, 11], [3, 23], [5, 21], [4, 22], [12, 14], [6, 26], [8, 24], [7, 25],
                [15, 17]],
            'centers': [10, 13, 16],
            'axis': this.zAxis,
            'rotation': Math.PI,
            'static': [],
            'moving': []
        },
        "z'": {
            'cycles': [[18, 20, 2, 0], [9, 19, 11, 1], [21, 23, 5, 3], [12, 22, 14, 4], [24, 26, 8, 6], [15, 25, 17, 7]],
            'swaps': [],
            'centers': [10, 13, 16],
            'axis': this.zAxis,
            'rotation': Math.PI / 2,
            'static': [],
            'moving': []
        },
    }

    constructor(scene, applied) {
        this.scene = scene;
        if (applied)
            apply_color(applied);
        else
            this.applied = this.solvedCube.map(function(arr) { return arr.slice() });
        for (let i = 0; i < 27; i++) {
            //make a mesh based on the before box
            this.cubies.push(new THREE.Mesh(this.geometry, this.cubeMaterials));
            //add it to the scene
            this.scene.add(this.cubies[i]);
            //create the outlines and add it to the scene.
            this.outlines.push(new THREE.LineSegments(this.edges, new THREE.LineBasicMaterial({
                color: 0x000000,
                linewidth: 0.015
            })));
            this.scene.add(this.outlines[i]);
        }
        //Position the cubes
        //Set the locations left to right, back to front, top to bottom.
        {
            let i = 0;
            for (let y = 2; y >= -2; y -= 2) {
                for (let z = -2; z <= 2; z += 2) {
                    for (let x = -2; x <= 2; x += 2) {
                        this.cubies[i].position.set(x, y, z);
                        this.outlines[i].position.set(x, y, z);
                        i++;
                    }
                }
            }
        }
        //Adding a few planes between the cubies.
        for (let i = 0; i < 12; i++) {
            this.planes.push(new THREE.Mesh(this.planeGeometry, this.planeMaterial));
        }
        //I'm not sure how to do this programmatically.
        this.planes[0].position.set(0, 1, 0); //up
        this.planes[0].rotateX(Math.PI / 2);
        this.planes[1].position.set(0, 1, 0); //up
        this.planes[1].rotateX(Math.PI / 2);
        this.planes[2].position.set(0, -1, 0); //down
        this.planes[2].rotateX(Math.PI / 2);
        this.planes[3].position.set(0, -1, 0); //down
        this.planes[3].rotateX(Math.PI / 2);
        this.planes[4].position.set(1, 0, 0); //right
        this.planes[4].rotateY(Math.PI / 2);
        this.planes[5].position.set(1, 0, 0); //right
        this.planes[5].rotateY(Math.PI / 2);
        this.planes[6].position.set(-1, 0, 0); //left
        this.planes[6].rotateY(Math.PI / 2);
        this.planes[7].position.set(-1, 0, 0); //left
        this.planes[7].rotateY(Math.PI / 2);
        this.planes[8].position.set(0, 0, 1); //front
        this.planes[9].position.set(0, 0, 1); //front
        this.planes[10].position.set(0, 0, -1); //back
        this.planes[11].position.set(0, 0, -1); //back
    }
    //Method to cycle the rotations of 4 cubies a > b > c > d > a
    cycle_rotations(a, b, c, d) {
        this.tempcube.setRotationFromEuler(a.rotation);
        a.setRotationFromEuler(d.rotation);
        d.setRotationFromEuler(c.rotation);
        c.setRotationFromEuler(b.rotation);
        b.setRotationFromEuler(this.tempcube.rotation);
    }
    //Method to swap the rotations of two cubies
    swap_rotations(a, b) {
        this.tempcube.setRotationFromEuler(a.rotation);
        a.setRotationFromEuler(b.rotation);
        b.setRotationFromEuler(this.tempcube.rotation);
    }
    //Method to reset the cube
    reset_cube() {
        //First stop the animation from running
        this.running = false;
        //Finish whatever move is running so it doesn't finish after te reset
        this.finish_move(this.cur)
        //Set any remaining animation to 0
        this.xRemaining = 0;
        this.yRemaining = 0;
        this.zRemaining = 0;
        //Set wait to 0
        this.wait = 0;
        //Reset all moves
        this.movesDone = [];
        this.moves = [];
        this.movenumber = 0;
        this.steps = [];
        //clear the current field and solution
        $(this.current_alg_div).html("");
        $(this.solution_div).html("");
        //set the applied cube back to the solved cube
        this.applied = this.solvedCube.map(function(arr) { return arr.slice() });
        //Finally set the rotatation back to 0 for every cubie.
        for (let i = 0; i < 27; i++) {
            this.cubies[i].rotation.x = 0;
            this.cubies[i].rotation.y = 0;
            this.cubies[i].rotation.z = 0;
        }
    }
    //returns a bool that says if the cube is moving.
    is_moving() {
        return (this.yRemaining != 0 || this.xRemaining != 0 || this.zRemaining != 0 || this.cur != "");
    }
    //Function prepares the movement animation
    do_move(move) {
        //If no move is given, or the cube is already moving, do nothing.
        if (!move || this.is_moving()) {
            return;
        }
        this.cur = move;
        //console.log(move);
        this.moving.clear();
        //Add all cubies in the cycles to the moving group
        for (let cycle of this.moveInstructions[move]['cycles']) {
            for (let cubie of cycle) {
                this.moving.add(this.cubies[cubie]);
                this.moving.add(this.outlines[cubie]);
            }
        }
        //Add all cubies in the swaps to the moving group
        for (let swap of this.moveInstructions[move]['swaps']) {
            for (let cubie of swap) {
                this.moving.add(this.cubies[cubie]);
                this.moving.add(this.outlines[cubie]);
            }
        }
        //Add all centers to the moving group
        for (let cubie of this.moveInstructions[move]['centers']) {
            this.moving.add(this.cubies[cubie]);
            this.moving.add(this.outlines[cubie]);
        }
        //Add the moving planes to the moving group
        for (let plane of this.moveInstructions[move]['moving']) {
            this.moving.add(this.planes[plane]);
        }
        //Add the static planes to the moving group
        for (let plane of this.moveInstructions[move]['static']) {
            this.scene.add(this.planes[plane]);
        }
        //Add the moving group back to the scene
        this.scene.add(this.moving);
        //Add the rotation to the right axis
        if (this.moveInstructions[move]['axis'] == this.xAxis)
            this.xRemaining = this.moveInstructions[move]['rotation']
        else if (this.moveInstructions[move]['axis'] == this.yAxis)
            this.yRemaining = this.moveInstructions[move]['rotation']
        else if (this.moveInstructions[move]['axis'] == this.zAxis)
            this.zRemaining = this.moveInstructions[move]['rotation']
    }

    //Method that actually moves the pieces (or gives the same effect)
    finish_move(move) {

        //Once the move animation is done, actually turn the cubies
        //Because the cubies don't really move, the basic idea is to copy the rotation of the cubie that on a real cube would move into its place, and then rotate everything 
        //Unfortunately there is no easier, more efficient way to do this.

        //If no move argument is given, return doing nothing.
        if (!move) {
            return;
        }
        //Clear the moving group
        this.moving.clear();
        //reset its rotation
        this.moving.rotation.x = 0;
        this.moving.rotation.y = 0;
        this.moving.rotation.z = 0;
        //Iterate over the cubies in the cycles array
        for (let cycle of this.moveInstructions[move]['cycles']) {
            this.cycle_rotations(this.cubies[cycle[0]], this.cubies[cycle[1]], this.cubies[cycle[2]], this.cubies[cycle[
                3]]);
            for (let cubie of cycle) {
                //Add all cubies in the cycles back to the scene
                this.scene.add(this.cubies[cubie]);
                this.scene.add(this.outlines[cubie]);
                //rotate all cubies in the cycles
                this.cubies[cubie].rotateOnWorldAxis(this.moveInstructions[move]['axis'], this.moveInstructions[move][
                    'rotation']);
            }
        }
        //Iterate over the cubies in the swaps array
        for (let swap of this.moveInstructions[move]['swaps']) {
            this.swap_rotations(this.cubies[swap[0]], this.cubies[swap[1]]);
            for (let cubie of swap) {
                //Add all cubies in the swaps back to the scene
                this.scene.add(this.cubies[cubie]);
                this.scene.add(this.outlines[cubie]);
                //rotate all cubies in the swaps
                this.cubies[cubie].rotateOnWorldAxis(this.moveInstructions[move]['axis'], this.moveInstructions[move][
                    'rotation']);
            }
        }
        //Add all center cubies back to the scene
        for (let cubie of this.moveInstructions[move]['centers']) {
            this.scene.add(this.cubies[cubie]);
            this.scene.add(this.outlines[cubie]);
        }
        //remove the static planes from the scene
        for (let plane of this.moveInstructions[move]['static']) {
            this.scene.remove(this.planes[plane]);
        }
        //reset cur to ""
        this.movesDone.push(this.cur);
        this.cur = "";
    }
    //Method to cycle through moves.
    next_move(click = false) {
        //Only do the mode if we aren't currently moving. Without this check we can desync if the user presses next while moving.
        if (!this.is_moving() && this.moves != undefined) {
            //If we already did the last move in the list
            if (this.movenumber >= this.moves.length) {
                //If there are more steps remaining, grab the next step of sets
                if (this.stepnumber < this.steps.length - 1) {
                    this.stepnumber++;
                    this.moves = this.steps[this.stepnumber];
                    //console.log(moves);
                    this.movenumber = 0;
                    if (!click)
                        this.wait = 20
                    $("#alg" + (this.stepnumber - 1)).removeClass("currentAlg")
                    $("#alg" + this.stepnumber).addClass("currentAlg")
                    document.getElementById("alg" + this.stepnumber).scrollIntoView({ block: "nearest" });
                }
                //If we're finished with the last step, asssume the cube is solved and clear the screen.
                else if (this.stepnumber == this.steps.length - 1) {
                    this.applied = this.solvedCube.map(function(arr) { return arr.slice() });
                    this.movesDone = [];
                    this.running = false;
                    $(this.current_alg_div).html("");
                    return
                }
                //Otherwise just clear the screen.
                else {
                    this.running = false;
                    $(this.current_alg_div).html("");
                    return
                }
            }
            //If next_move was called manually, wait 2 frames.
            else if (!click)
                this.wait = 2;
            //Print the current algorithm, with the current step emphasized 
            let html = "<p>";
            for (let i = 0; i < this.moves.length; i++) {
                if (i == this.movenumber)
                    html += `<span class="currentMove">${this.moves[i]}</span> `
                else
                    html += this.moves[i] + " ";
            }
            html += "</p>"
            $(this.current_alg_div).html(html);
            //actually do the move.
            this.do_move(this.moves[this.movenumber]);
            //increment the move counter
            this.movenumber++;
        }
    }
    //Method to cycle through backwards.
    prev_move() {
        //Function that gives the inverted version of the entered move (note, changes F2 to F2' to invert rotation)
        function invert_move(move) {
            if (move.slice(-1) == "'")
                return move.slice(0, -1);
            else
                return move + "'";
        }
        //Only do the mode if we aren't currently moving. Without this check we can desync if the user presses next while moving.
        if (!this.is_moving()) {
            //If we're already at the first move, return without doing anything
            if (this.movenumber == 0)
                return;
            //First decrement the move counter
            this.movenumber--;
            //Print out the previous move.
            let html = "<p>";
            for (let i = 0; i < this.moves.length; i++) {
                if (i == this.movenumber)
                    html += `<span class="currentMove">${this.moves[i]}</span> `
                else
                    html += this.moves[i] + " ";
            }
            html += "</p>"
            $(this.current_alg_div).html(html);
            //actually do the move in reverse
            this.do_move(invert_move(this.moves[this.movenumber]));
            //If this was the first move,
            if (this.movenumber == 0) {
                //check for earlier steps, and show the last one.
                if (this.stepnumber > 0) {
                    this.stepnumber--;
                    this.moves = this.steps[this.stepnumber];
                    //console.log(moves);
                    this.movenumber = this.moves.length;
                    $("#alg" + (this.stepnumber + 1)).removeClass("currentAlg")
                    $("#alg" + this.stepnumber).addClass("currentAlg")
                    document.getElementById("alg" + this.stepnumber).scrollIntoView({ block: "nearest" });
                }
            }
        }
    }
    //method to apply a color pattern to the cube.
    apply_pattern(pattern) {
        //Internal function to calculate the opposite color of a square. 
        function opposite_color(c) {
            //the front, right, back and left faces are 0, 1, 2, 3 respectively, so just add 2
            if (c < 4) return (c + 2) % 4;
            //switch white and yellow
            else if (c == 4) return 5
            else if (c == 5) return 4
            //if the color is not on the list, just give back the color (should never be reached)
            else return c;
        }

        //function applies the colors to the cubie, asking for the front and left face (or up face if up is true)
        function unbound_apply_color(cubie, front, left, fUp = false, lUp = false) {
            if (front == 0) //green
            {
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 2) //blue
            {
                cubie.rotateOnWorldAxis(this.yAxis, (Math.PI)); //turn 180 degrees
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 3) //orange
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 1) //red
            {
                cubie.rotateOnWorldAxis(this.yAxis, -(Math.PI / 2)); //turn clockwise
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 3) //orange
            {
                cubie.rotateOnWorldAxis(this.yAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 4) //white
            {
                cubie.rotateOnWorldAxis(this.xAxis, (Math.PI / 2)); //turn clockwise
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 5) //yellow
            {
                cubie.rotateOnWorldAxis(this.xAxis, -(Math.PI / 2)); //turn clockwise
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.zAxis, Math.PI); //flip over
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 6) //for if no known color in front
            {
                if (left == 0)
                    cubie.rotateOnWorldAxis(this.yAxis, -Math.PI);
                if (left == 1)
                    cubie.rotateOnWorldAxis(this.yAxis, Math.PI); //flip over
                if (left == 2)
                    cubie.rotateOnWorldAxis(this.yAxis, Math.PI);
                if (left == 4)
                    cubie.rotateOnWorldAxis(this.zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 5)
                    cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2)); //turn counterclockwise
            }
            if (fUp) {
                cubie.rotateOnWorldAxis(this.xAxis, -(Math.PI / 2))
            } else if (lUp) {
                cubie.rotateOnWorldAxis(this.zAxis, -(Math.PI / 2))
            }
        }
        //bind the function to this class.
        var apply_color = unbound_apply_color.bind(this);
        this.reset_cube(); //first give a blank slate
        //I have to do this one by one, because the code to automate it would be longer than just manually doing it.
        apply_color(this.cubies[0], opposite_color(pattern[2][2]), pattern[3][0]);
        apply_color(this.cubies[1], opposite_color(pattern[2][1]), pattern[4][1], false, true);
        apply_color(this.cubies[2], opposite_color(pattern[2][0]), opposite_color(pattern[1][2]));
        apply_color(this.cubies[3], pattern[4][3], pattern[3][1], true, false);
        apply_color(this.cubies[4], pattern[4][4], 6, true, false); //6 means unknown/unnecessary, only used for centers.
        apply_color(this.cubies[5], pattern[4][5], opposite_color(pattern[1][1]), true, false);
        apply_color(this.cubies[6], pattern[0][0], pattern[3][2]);
        apply_color(this.cubies[7], pattern[0][1], pattern[4][7], false, true);
        apply_color(this.cubies[8], pattern[0][2], opposite_color(pattern[1][0]));
        apply_color(this.cubies[9], opposite_color(pattern[2][5]), pattern[3][3]);
        apply_color(this.cubies[10], opposite_color(pattern[2][4]), 6);
        apply_color(this.cubies[11], opposite_color(pattern[2][3]), opposite_color(pattern[1][5]));
        apply_color(this.cubies[12], 6, pattern[3][4]);
        //13 is the center, so nothing to apply
        apply_color(this.cubies[14], 6, opposite_color(pattern[1][4]));
        apply_color(this.cubies[15], pattern[0][3], pattern[3][5]);
        apply_color(this.cubies[16], pattern[0][4], 6);
        apply_color(this.cubies[17], pattern[0][5], opposite_color(pattern[1][3]));
        apply_color(this.cubies[18], opposite_color(pattern[2][8]), pattern[3][6]);
        apply_color(this.cubies[19], opposite_color(pattern[2][7]), opposite_color(pattern[5][7]), false, true);
        apply_color(this.cubies[20], opposite_color(pattern[2][6]), opposite_color(pattern[1][8]));
        apply_color(this.cubies[21], opposite_color(pattern[5][3]), pattern[3][7], true, false);
        apply_color(this.cubies[22], opposite_color(pattern[5][4]), 6, true, false);
        apply_color(this.cubies[23], opposite_color(pattern[5][5]), opposite_color(pattern[1][7]), true, false);
        apply_color(this.cubies[24], pattern[0][6], pattern[3][8]);
        apply_color(this.cubies[25], pattern[0][7], opposite_color(pattern[5][1]), false, true);
        apply_color(this.cubies[26], pattern[0][8], opposite_color(pattern[1][6]));
        this.applied = pattern.map(function(arr) { return arr.slice() });
    }

    //Method to run and animate an algorithm on the cube
    run_alg(alg) {
        if (this.running == false) {
            //alg it into a moves array
            this.moves = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
            //reset the move counter
            this.movenumber = 0;
            //clear the steps
            this.steps = [];
            //reset the step number to 0
            this.stepnumber = 0;
            let html = "";
            for (let i = 0; i < this.moves.length; i++) {
                html += this.moves[i] + " ";
            }
            $(this.current_alg_div).html(html);
            //start running if not paused
            if (!this.paused)
                this.running = true;
        }
    }

    //Method to apply an algorithm to the cube without animation
    apply_alg(alg) {
        this.reset_cube()
        let scramble = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
        this.movenumber = 0;
        this.steps = [];
        this.stepnumber = 0;
        $(this.solution_div).html("");
        for (let move of scramble) {
            //We just do the finishing step (the actual movement) without animation
            this.finish_move(move);
        }
        this.movesDone = scramble;
    }

    set_solver_api(url) {
        this.solver_api = url;
    }

    set_solution_div(id) {
        this.solution_div = id;
    }

    set_current_alg_div(id) {
        this.current_alg_div = id;
    }

    //Method to solve the cube
    solve_cube() {
        this.running = false;
        this.finish_move(this.cur);
        //if no solver api is set, return false
        if (this.solver_api == "")
            return false;
        //in order to call class functions and use variables I need to make a reference to this
        var thisClass = this;
        //Send the pattern (if applied) and any moves done to it already to the solver
        $.post(thisClass.solver_api, { pattern: JSON.stringify(this.applied), scramble: this.movesDone.join(" ") }, function(
            data) {
            //empty the steps array
            thisClass.steps = [];
            //reset the curent step
            thisClass.stepnumber = 0;

            //initialize counter for the algs
            let algs = 0;
            let html = "";
            //for every dictionary in data
            for (let step of data) {
                //add a head with the step and its algorithm
                html += `<h3 class="step"><span class="stepname">${step["step"]}</span>\n`
                if (step["name"])
                    html += ` - <span class="algname">${step["name"]}</span></h3><ul>\n`;
                else
                    html += '</h3><ul>'
                //Print all steps in a list
                for (let alg of step["algorithms"]) {
                    if (step["algorithms"] != "") {
                        //the id is dynamic so I can modify the CSS
                        html +=
                            `<li id='alg${algs}' class='algorithm'>${alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "")}</li>\n`;
                        //add he alg into the steps array, ignoring everything but the moves.
                        thisClass.steps.push(alg.replace(/[^FfBbRrLlUuDdxyzMES\'2 ]/g, "").split(" "))
                        //console.log(steps[algs]);
                        algs++;
                    }
                }
                html += "</ul>";
            }
            //put the html into the solution div
            $(thisClass.solution_div).html(html);
            $("#alg" + thisClass.stepnumber).addClass("currentAlg")
            //put the first step into moves and reset the counter
            thisClass.moves = thisClass.steps[0];
            thisClass.movenumber = 0;
            //start solving the cube
            if (thisClass.paused == false) {
                thisClass.running = true;
            }
            return true;
        });
    }

    //Method to set the next movement speed in radians per frame.
    set_movement_speed(mspeed) {
        this.mspeed = mspeed;
    }

    //Method to prepare the next animation frame
    animate() {
        //First wait for x frames
        if (this.wait > 0) {
            this.wait -= Math.max(10 * this.mspeed, 1);
        }
        //If there is an y move set up
        else if (this.yRemaining < 0) {
            //if there is less than mspeed remaining, just move mspeed
            if (this.yRemaining > -this.mspeed) {
                this.moving.rotateY(this.yRemaining);
                this.yRemaining = 0;
            }
            //otherwise move
            else {
                this.moving.rotateY(-this.mspeed);
                this.yRemaining += this.mspeed;
            }
        } else if (this.yRemaining > 0) {
            if (this.yRemaining < this.mspeed) {
                this.moving.rotateY(this.yRemaining);
                this.yRemaining = 0;
            } else {
                this.moving.rotateY(this.mspeed);
                this.yRemaining -= this.mspeed;
            }
        } else if (this.xRemaining < 0) {
            if (this.xRemaining > -this.mspeed) {
                this.moving.rotateX(this.xRemaining);
                this.xRemaining = 0;
            } else {
                this.moving.rotateX(-this.mspeed);
                this.xRemaining += this.mspeed;
            }
        } else if (this.xRemaining > 0) {
            if (this.xRemaining < this.mspeed) {
                this.moving.rotateX(this.xRemaining);
                this.xRemaining = 0;
            } else {
                this.moving.rotateX(this.mspeed);
                this.xRemaining -= this.mspeed;
            }
        } else if (this.zRemaining < 0) {
            if (this.zRemaining > -this.mspeed) {
                this.moving.rotateZ(this.zRemaining);
                this.zRemaining = 0;
            } else {
                this.moving.rotateZ(-this.mspeed);
                this.zRemaining += this.mspeed;
            }
        } else if (this.zRemaining > 0) {
            if (this.zRemaining < this.mspeed) {
                this.moving.rotateZ(this.zRemaining);
                this.zRemaining = 0;
            } else {
                this.moving.rotateZ(this.mspeed);
                this.zRemaining -= this.mspeed;
            }
        } else if (this.cur) {
            this.finish_move(this.cur);
        } else if (this.running) {
            this.next_move();
        }
    }
}

//Set up scene. Most of this code is copied straight from the tutorial on the three.js site
//Get the container from the html file 
let container = document.getElementById('canvas');
//Create a scene
const scene = new THREE.Scene();
//make it the same color as the canvas
scene.background = new THREE.Color($('#canvas').css("background-color"));
//create a camera
const camera = new THREE.PerspectiveCamera(75, (container.clientWidth) / (container.clientHeight), 0.1, 1000);
//create renderer
const renderer = new THREE.WebGLRenderer();
//Set the size the size of the container
renderer.setSize(container.clientWidth, container.clientHeight);
container.appendChild(renderer.domElement);
//Orbit control camera, from a three.js example
const controls = new OrbitControls(camera, renderer.domElement);
//disable panning because there's just one object in the middle
controls.enablePan = false;
//set the position. The numbers were chosen using trial and error
camera.position.set(4.5, 4.3, 6.2);
//make camera available from the dom so I can console.log
window.camera = camera;
controls.update();

//Now finally create a cube
let cube = new Cube(scene);
cube.set_solver_api("api/solver");
cube.set_solution_div("#solution");
cube.set_current_alg_div("#currentAlg");

//A 2d list of the faces of a cube, for entering your own colors
var faces = [[6, 6, 6, 6, 0, 6, 6, 6, 6],
[6, 6, 6, 6, 1, 6, 6, 6, 6],
[6, 6, 6, 6, 2, 6, 6, 6, 6],
[6, 6, 6, 6, 3, 6, 6, 6, 6],
[6, 6, 6, 6, 4, 6, 6, 6, 6],
[6, 6, 6, 6, 5, 6, 6, 6, 6]]

//Copied from the jquery codebase. Function to get variables from get
function urlparam(name) {
    var results = new RegExp('[\?&]' + name + '=([^&#]*)').exec(window.location.href);
    if (results == null) {
        return null;
    }
    return decodeURI(results[1]) || 0;
}

//temporary code to take an algorithm from get
if (urlparam("alg")) {
    cube.run_alg(urlparam("alg")); //split on plus or space
    cube.wait = 20;
    cube.running = true;
}

//temporary code to take a scramble from get
if (urlparam("scramble")) {
    cube.apply_alg(urlparam("scramble").replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split((/[\s+]+/)));
}

//The colors of the cube, assuming white is up and green is front.
const colors = ["green", "red", "blue", "orange", "white", "yellow", "grey"]
var color = 6;
//If one of the squared is clicked, apply a color
$('.square').click(function() {
    //If the square is one of the color pickers
    if (colors.includes(this.id)) {
        //set the color to the number of that color
        color = colors.indexOf(this.id);
        //update cur to show that color
        $("#cur").css("background-color", colors[color])
    }
    //If the selected square is cur, switch back to grey
    else if (this.id == "#cur") {
        color = 6
        $("#cur").css("background-color", colors[color])
    } //for all other squares, except for the centers
    else if (parseInt(this.id[3]) != 4) {
        if (faces[parseInt(this.id[1])][parseInt(this.id[3])] == color) {
            //set the color of the square to the selected color
            $("#" + this.id).css("background-color", colors[6])
            //update the color in the "faces" array
            faces[parseInt(this.id[1])][parseInt(this.id[3])] = 6;
        } else {
            //set the color of the square to the selected color
            $("#" + this.id).css("background-color", colors[color])
            //update the color in the "faces" array
            faces[parseInt(this.id[1])][parseInt(this.id[3])] = color;
        }
    }
});

//Clear the color picker
$('#clear').click(function() {
    for (let i = 0; i < 6; i++) {
        for (let j = 0; j < 9; j++) {
            if (j != 4) {
                faces[i][j] = 6;
                $(`#f${i}s${j}`).css("background-color", colors[6]);
            }
        }
    }
});

const errors = ["Success", "No pattern entered", "Invalid color combination", "OLL parity", "PLL parity",
    "Error in solving algorithm"]
//If a user applies a pattern
//Apply a pattern to the cube
$('#apply').click(function() {
    //First validate the pattern
    $.post("api/validator", { pattern: JSON.stringify(faces) }, function(data) {
        //If it fails, give the error
        let reply = parseInt(data);
        if (reply != 0) {
            $('#error').html(errors[reply]);
        } else {
            //then apply the faces
            $('#error').html("&nbsp;");
            cube.apply_pattern(faces);
            //reset solution
            $('#solution').html("");
        }
    });
});

//Run an algorithm on the cube
$('#algForm').submit(function(e) {
    //run the alg
    cube.run_alg($('#alg').val());
    //and do not submit.
    e.preventDefault();
})

//Disable return on the algorithm field, because there are two submit buttons
$('#algForm').on('keydown', function(event) {
    var x = event.which;
    if (x === 13) {
        event.preventDefault();
    }
});

//Run an algorithm on the cube without showing the moves
$('#scramble').click(function() {
    cube.apply_alg($('#alg').val());
});
//Select a pattern from the pattern select and put it in the algorithm field.
$('#pattern').change(function() {
    $('#alg').val(document.getElementById("pattern").value)
})
//On click events for play/pause, next and prev
$('#playpause').click(function() {
    if (cube.paused) {
        cube.paused = false;
        cube.running = true;
        $('#playpause').html('<img src="static/pause.png" alt="pause"></img>');
    } else {
        cube.paused = true;
        cube.running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
});


$('#next').click(function() {
    if (cube.paused == false) {
        cube.paused = true;
        cube.running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    cube.next_move(true);
});

$('#prev').click(function() {
    if (cube.paused == false) {
        cube.paused = true;
        cube.running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    cube.prev_move();
});

//On click events for solver and reset.
$('#solve').click(function() {
    window.scrollTo(0, 0); //In case of mobile, scroll to the top of the screen
    cube.solve_cube();
    document.getElementById('solve').blur();
});

$('#reset').click(function() {
    //First finish the moves so no new moves happen after the rest
    cube.running = false;
    cube.finish_move(this.cur);
    cube.reset_cube();
});

//Vary the movement speed.
$('#speed').on('input', function() {
    cube.set_movement_speed($('#speed').val() / 100);
})

//the animate function
function animate() {
    requestAnimationFrame(animate);
    cube.animate();
    controls.update();
    renderer.render(scene, camera);
}
animate();