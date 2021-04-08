

import * as THREE from 'https://unpkg.com/three/build/three.module.js';
import { OrbitControls } from 'https://unpkg.com/three/examples/jsm/controls/OrbitControls.js';
//Copied from the jquery codebase
function urlparam(name) {
    var results = new RegExp('[\?&]' + name + '=([^&#]*)').exec(window.location.href);
    if (results == null) {
        return null;
    }
    return decodeURI(results[1]) || 0;
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
//create a box
var geometry = new THREE.BoxGeometry(1.985, 1.985, 1.985); //including outline it's a perfect 2
//give it colors
var cubeMaterials = [
    new THREE.MeshBasicMaterial({ color: 'red', transparent: true, opacity: 1, side: THREE.DoubleSide, }), //left
    new THREE.MeshBasicMaterial({ color: 'orange', transparent: true, opacity: 1, side: THREE.DoubleSide }), //right
    new THREE.MeshBasicMaterial({ color: 'white', transparent: true, opacity: 1, side: THREE.DoubleSide }), //up
    new THREE.MeshBasicMaterial({ color: 'yellow', transparent: true, opacity: 1, side: THREE.DoubleSide }), //down
    new THREE.MeshBasicMaterial({ color: 'green', transparent: true, opacity: 1, side: THREE.DoubleSide }), //front
    new THREE.MeshBasicMaterial({ color: 'blue', transparent: true, opacity: 1, side: THREE.DoubleSide }), //back
];
//draw the outlines:
const edges = new THREE.EdgesGeometry(geometry);

//create a 27 cubes
var cubes = [];
var outlines = [];
for (let i = 0; i < 27; i++) {
    //make a mesh based on the before box
    cubes.push(new THREE.Mesh(geometry, cubeMaterials));
    //add it to the scene
    scene.add(cubes[i]);
    //create the outlines and add it to the scene.
    outlines.push(new THREE.LineSegments(edges, new THREE.LineBasicMaterial({ color: 0x000000, linewidth: 0.015 })));
    scene.add(outlines[i]);
}
//cube used to store temporary values
var tempcube = new THREE.Mesh(geometry, cubeMaterials);
//Position the cubes
//Set the locations left to right, back to front, top to bottom.
{
    let i = 0;
    for (let y = 2; y >= -2; y -= 2) {
        for (let z = -2; z <= 2; z += 2) {
            for (let x = -2; x <= 2; x += 2) {
                cubes[i].position.set(x, y, z);
                outlines[i].position.set(x, y, z);
                i++;
            }
        }
    }
}

//Orbit control camera, from a three.js example
const controls = new OrbitControls(camera, renderer.domElement);
//disable panning because there's just one object in the middle
controls.enablePan = false;
//set the position. The numbers were chosen using trial and error
camera.position.set(4.5, 4.3, 6.2);
//make camera available from the dom so I can console.log
window.camera = camera;
controls.update();
//Create a group for the pieces currently moving
var moving = new THREE.Group();
//variables for the moves being made
var xRemaining = 0;
var yRemaining = 0;
var zRemaining = 0;
//variable for the piece currently moving
var cur = "";
//Whether it's running an algorithm
var running = false;
//whether or not running is paused (for stepping through manually)
var paused = false;
//The wait time until the next step
var wait = 0;
//The default movement speed in radians per frame
var mspeed = 0.1;
//The step of a solution being done
var stepnumber = 0;
var steps = [];
//the step in the algorithm being done.
var movenumber = 0;
var moves = [];
//A list of moves done to the cube, in order to step back or send to the solver as a scramble
var movesDone = []
//Axes of rotation
const xAxis = new THREE.Vector3(1, 0, 0);
const yAxis = new THREE.Vector3(0, 1, 0);
const zAxis = new THREE.Vector3(0, 0, 1);
//The cubies that belong to each slice, to make it easier to work with them all in a loop
const slices = {
    'U': [0, 1, 2, 3, 4, 5, 6, 7, 8],
    'E': [9, 10, 11, 12, 13, 14, 15, 16, 17],
    'D': [18, 19, 20, 21, 22, 23, 24, 25, 26],
    'R': [2, 5, 8, 11, 14, 17, 20, 23, 26],
    'M': [1, 4, 7, 10, 13, 16, 19, 22, 25],
    'L': [0, 3, 6, 9, 12, 15, 18, 21, 24],
    'B': [0, 1, 2, 9, 10, 11, 18, 19, 20],
    'S': [3, 4, 5, 12, 13, 14, 21, 22, 23],
    'F': [6, 7, 8, 15, 16, 17, 24, 25, 26]
};

//A 2d list of the faces of a solved cube
const solvedCube = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
[1, 1, 1, 1, 1, 1, 1, 1, 1],
[2, 2, 2, 2, 2, 2, 2, 2, 2],
[3, 3, 3, 3, 3, 3, 3, 3, 3],
[4, 4, 4, 4, 4, 4, 4, 4, 4],
[5, 5, 5, 5, 5, 5, 5, 5, 5]]

//A 2d list of the faces of a cube, for entering your own colors
var faces = [[6, 6, 6, 6, 0, 6, 6, 6, 6],
[6, 6, 6, 6, 1, 6, 6, 6, 6],
[6, 6, 6, 6, 2, 6, 6, 6, 6],
[6, 6, 6, 6, 3, 6, 6, 6, 6],
[6, 6, 6, 6, 4, 6, 6, 6, 6],
[6, 6, 6, 6, 5, 6, 6, 6, 6]]

//The last applied cube (currently solvedCube but not always)
var applied = solvedCube.map(function (arr) { return arr.slice() });

//Function to reset the cube
function reset_cube() {
    for (let i = 0; i < 27; i++) {
        cubes[i].rotation.x = 0;
        cubes[i].rotation.y = 0;
        cubes[i].rotation.z = 0;
    }
    movesDone = [];
    moves = [];
    movenumber = 0;
    running = false;
    steps = [];
    $("#currentAlg").html("");
    applied = solvedCube.map(function (arr) { return arr.slice() });
}
//make this function available to the interface
window.reset_cube = function () { reset_cube(); }
//reset the cube
reset_cube();

//Function to cycle the rotations of 4 cubies a > b > c > d > a
//todo: invert cycle_rotations
function cycle_rotations(a, b, c, d) {
    tempcube.setRotationFromEuler(a.rotation);
    a.setRotationFromEuler(d.rotation);
    d.setRotationFromEuler(c.rotation);
    c.setRotationFromEuler(b.rotation);
    b.setRotationFromEuler(tempcube.rotation);
}

//Function to swap the rotations of two cubies
function swap_rotations(a, b) {
    tempcube.setRotationFromEuler(a.rotation);
    a.setRotationFromEuler(b.rotation);
    b.setRotationFromEuler(tempcube.rotation);
}

//returns a bool that says if the cube is moving.
function is_moving() {
    return (yRemaining != 0 || xRemaining != 0 || zRemaining != 0 || cur != "");
}

//Function prepares the movement animation
function do_move(move) {
    //If no move is given, or the cube is already moving, do nothing.
    if (!move || is_moving()) {
        return;
    }
    cur = move;
    //console.log(move);
    moving.clear();
    //If the first letter of the move is U or u, add the up layer to the moving group
    if (move[0] == "U" || move[0] == "u" || move[0] == "y") {
        for (let i of slices['U']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //Set up the movement for U or u
        if (move == "U" || move == "u" || move == "y") {
            yRemaining = -(Math.PI / 2);
        }
        else if (move == "U2" || move == "u2" || move == "y2") {
            yRemaining = -Math.PI;
        }
        else if (move == "U2'" || move == "u2'" || move == "y2'") {
            yRemaining = Math.PI;
        }
        else if (move == "U'" || move == "u'" || move == "y'") {
            yRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is E or u or d, add the equator layer to the moving group
    if (move[0] == "E" || move[0] == "u" || move[0] == "d" || move[0] == "y") {
        for (let i of slices['E']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //only set up moves for E
        if (move == "E'") {
            yRemaining = -(Math.PI / 2);
        }
        else if (move == "E2") {
            yRemaining = -Math.PI;
        }
        else if (move == "E2'") {
            yRemaining = Math.PI;
        }
        else if (move == "E") {
            yRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is D or d, add the D layer to the moving group
    if (move[0] == "D" || move[0] == "d" || move[0] == "y") {
        for (let i of slices['D']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //set up moves for D and d
        if (move == "D'" || move == "d'") {
            yRemaining = -(Math.PI / 2);
        }
        else if (move == "D2" || move == "d2") {
            yRemaining = -Math.PI;
        }
        else if (move == "D2'" || move == "d2'") {
            yRemaining = Math.PI;
        }
        else if (move == "D" || move == "d") {
            yRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is R or r, add the R layer to the moving group,
    if (move[0] == "R" || move[0] == "r" || move[0] == "x") {
        for (let i of slices['R']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //Set up the movement for R or r
        if (move == "R" || move == "r" || move == "x") {
            xRemaining = -(Math.PI / 2);
        }
        else if (move == "R2" || move == "r2" || move == "x2") {
            xRemaining = -Math.PI;
        }
        else if (move == "R2'" || move == "r2'" || move == "x2'") {
            xRemaining = Math.PI;
        }
        else if (move == "R'" || move == "r'" || move == "x'") {
            xRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is M or r or l, add the middle  layer to the moving group
    if (move[0] == "M" || move[0] == "r" || move[0] == "l" || move[0] == "x") {
        for (let i of slices['M']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //only set up moves for M
        if (move == "M'") {
            xRemaining = -(Math.PI / 2);
        }
        else if (move == "M2") {
            xRemaining = Math.PI;
        }
        else if (move == "M2'") {
            xRemaining = -Math.PI;
        }
        else if (move == "M") {
            xRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is L or l, add the L  layer to the moving group
    if (move[0] == "L" || move[0] == "l" || move[0] == "x") {
        for (let i of slices['L']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //set up moves for L and l
        if (move == "L'" || move == "l'") {
            xRemaining = -(Math.PI / 2);
        }
        else if (move == "L2" || move == "l2") {
            xRemaining = Math.PI;
        }
        else if (move == "L2'" || move == "l2'") {
            xRemaining = -Math.PI;
        }
        else if (move == "L" || move == "l") {
            xRemaining = (Math.PI / 2);
        }
    }
    //FSB
    //If the first letter of the move is F or f, add the F layer to the moving group,
    if (move[0] == "F" || move[0] == "f" || move[0] == "z") {
        for (let i of slices['F']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //Set up the movement for F or f
        if (move == "F" || move == "f" || move == "z") {
            zRemaining = -(Math.PI / 2);
        }
        else if (move == "F2" || move == "f2" || move == "z2") {
            zRemaining = -Math.PI;
        }
        else if (move == "F2'" || move == "f2'" || move == "z2'") {
            zRemaining = Math.PI;
        }
        else if (move == "F'" || move == "f'" || move == "z'") {
            zRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is S or f or b, add the middle  layer to the moving group
    if (move[0] == "S" || move[0] == "f" || move[0] == "b" || move[0] == "z") {
        for (let i of slices['S']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //only set up moves for S
        if (move == "S'") {
            zRemaining = -(Math.PI / 2);
        }
        else if (move == "S2") {
            zRemaining = Math.PI;
        }
        else if (move == "S2'") {
            zRemaining = -Math.PI;
        }
        else if (move == "S") {
            zRemaining = (Math.PI / 2);
        }
    }
    //If the first letter of the move is B or b, add the B  layer to the moving group
    if (move[0] == "B" || move[0] == "b" || move[0] == "z") {
        for (let i of slices['B']) {
            moving.add(cubes[i]);
            moving.add(outlines[i]);
            scene.add(moving);
        }
        //set up moves for B and b
        if (move == "B'" || move == "b'") {
            zRemaining = -(Math.PI / 2);
        }
        else if (move == "B2" || move == "b2") {
            zRemaining = Math.PI;
        }
        else if (move == "B2'" || move == "b2'") {
            zRemaining = -Math.PI;
        }
        else if (move == "B" || move == "b") {
            zRemaining = (Math.PI / 2);
        }
    }
}
//make the function usable outside of the js file.
window.do_move = function (move, inverted = false) { if (inverted) do_move(move + "'"); else do_move(move); }

//Function actually moves the pieces (or gives the same effect)
function finish_move(move) {

    //Once the move animation is done, actually turn the cubies
    //Because the cubies don't really move, the basic idea is to copy the rotation of the cubie that on a real cube would move into its place, and then rotate everything 
    //Unfortunately there is no easier, more efficient way to do this.

    //If no move argument is given, return doing nothing.
    if (!move) {
        return;
    }
    //Clear the moving group
    moving.clear();
    //reset its rotation
    moving.rotation.x = 0;
    moving.rotation.y = 0;
    moving.rotation.z = 0;

    if (move[0] == "U" || move[0] == "u" || move[0] == 'y') {
        //for the first nine cubies
        for (let i of slices['U']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is U or u
        if (move == "U" || move == "u" || move == "y") {
            //Cycle the rotation of the corners
            cycle_rotations(cubes[2], cubes[8], cubes[6], cubes[0]);
            //Cycle the rotation of the edges
            cycle_rotations(cubes[5], cubes[7], cubes[3], cubes[1]);
            for (let i of slices['U']) {
                //rotate them all clockwise
                cubes[i].rotateOnWorldAxis(yAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "U2" || move == "u2" || move == "y2" || move == "U2'" || move == "u2'" || move == "y2'") {
            //swap the corners
            swap_rotations(cubes[0], cubes[8]);
            swap_rotations(cubes[2], cubes[6]);
            //swap the edges
            swap_rotations(cubes[1], cubes[7]);
            swap_rotations(cubes[3], cubes[5]);
            for (let i of slices['U']) {
                //rote them 180
                cubes[i].rotateOnWorldAxis(yAxis, Math.PI);
            }
        }
        //Inverted move
        else if (move == "U'" || move == "u'" || move == "y'") {
            //Cycle the rotation of the corners
            cycle_rotations(cubes[6], cubes[8], cubes[2], cubes[0]);
            //Cycle the rotation of the edges
            cycle_rotations(cubes[3], cubes[7], cubes[5], cubes[1]);
            for (let i of slices['U']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(yAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "E" || move[0] == "u" || move[0] == "d" || move[0] == 'y') {
        //for the next nine cubies
        for (let i of slices['E']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "E'" || move == "u" || move == "d'" || move == "y") {
            //Cycle the rotation of the edges
            cycle_rotations(cubes[11], cubes[17], cubes[15], cubes[9]);
            //Cycle the rotation of the centers
            cycle_rotations(cubes[14], cubes[16], cubes[12], cubes[10]);
            for (let i of slices['E']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(yAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "E2" || move == "u2" || move == "d2" || move == "y2" || move == "E2'" || move == "u2'" || move == "d2'" || move == "y2'") {
            //swap the edges
            swap_rotations(cubes[9], cubes[17]);
            swap_rotations(cubes[11], cubes[15]);
            //swap the centers
            swap_rotations(cubes[10], cubes[16]);
            swap_rotations(cubes[12], cubes[14]);
            for (let i of slices['E']) {
                //rotate them 180
                cubes[i].rotateOnWorldAxis(yAxis, Math.PI);
            }
        }
        else if (move == "E" || move == "u'" || move == "d" || move == "y'") {
            //Cycle the rotation of the edges
            cycle_rotations(cubes[15], cubes[17], cubes[11], cubes[9]);
            //Cycle the rotation of the centers
            cycle_rotations(cubes[12], cubes[16], cubes[14], cubes[10]);
            for (let i of slices['E']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(yAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "D" || move[0] == "d" || move[0] == 'y') {
        //for the next nine cubies
        for (let i of slices['D']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "D'" || move == "d'" || move == "y") {
            //Cycle the rotation of the corners
            cycle_rotations(cubes[20], cubes[26], cubes[24], cubes[18]);
            //Cycle the rotation of the edges
            cycle_rotations(cubes[23], cubes[25], cubes[21], cubes[19]);
            for (let i of slices['D']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(yAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "D2" || move == "d2" || move == "y2" || move == "D2'" || move == "d2'" || move == "y2'") {
            //swap the corners
            swap_rotations(cubes[18], cubes[26]);
            swap_rotations(cubes[20], cubes[24]);
            //swap the edges
            swap_rotations(cubes[19], cubes[25]);
            swap_rotations(cubes[21], cubes[23]);
            for (let i of slices['D']) {
                //rote them 180
                cubes[i].rotateOnWorldAxis(yAxis, Math.PI);
            }
        }
        else if (move == "D" || move == "d" || move == "y'") {
            //Cycle the rotation of the corners
            cycle_rotations(cubes[24], cubes[26], cubes[20], cubes[18]);
            //Cycle the rotation of the edges
            cycle_rotations(cubes[21], cubes[25], cubes[23], cubes[19]);
            for (let i of slices['D']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(yAxis, (Math.PI / 2));
            }
        }
    }
    //Now the X direction
    if (move[0] == "R" || move[0] == "r" || move[0] == "x") {
        //for the first nine cubies
        for (let i of slices['R']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is U or u
        if (move == "R" || move == "r" || move == "x") {
            //cycle the corners
            cycle_rotations(cubes[20], cubes[26], cubes[8], cubes[2]);
            //cycle the edges
            cycle_rotations(cubes[11], cubes[23], cubes[17], cubes[5]);
            for (let i of slices['R']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(xAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "R2" || move == "r2" || move == "x2" || move == "R2'" || move == "r2'" || move == "x2'") {
            //swap the corners
            swap_rotations(cubes[2], cubes[26]);
            swap_rotations(cubes[8], cubes[20]);
            //swap the edges
            swap_rotations(cubes[5], cubes[23]);
            swap_rotations(cubes[11], cubes[17]);
            for (let i of slices['R']) {
                //rotate them 180
                cubes[i].rotateOnWorldAxis(xAxis, Math.PI);
            }
        }
        else if (move == "R'" || move == "r'" || move == "x'") {
            //cycle the corners
            cycle_rotations(cubes[8], cubes[26], cubes[20], cubes[2]);
            //cycle the edges
            cycle_rotations(cubes[17], cubes[23], cubes[11], cubes[5]);
            for (let i of slices['R']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(xAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "M" || move[0] == "r" || move[0] == "l" || move[0] == "x") {
        //for the next nine cubies
        for (let i of slices['M']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "M'" || move == "r" || move == "l'" || move == "x") {
            //cycle the edges
            cycle_rotations(cubes[19], cubes[25], cubes[7], cubes[1]);
            //cycle the centers
            cycle_rotations(cubes[10], cubes[22], cubes[16], cubes[4]);
            for (let i of slices['M']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(xAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "M2" || move == "r2" || move == "l2" || move == "x2" || move == "M2'" || move == "r2'" || move == "l2'" || move == "x2'") {
            //swap the edges
            swap_rotations(cubes[1], cubes[25]);
            swap_rotations(cubes[7], cubes[19]);
            //swap the centers
            swap_rotations(cubes[4], cubes[22]);
            swap_rotations(cubes[10], cubes[16]);
            for (let i of slices['M']) {
                //rote them 180
                cubes[i].rotateOnWorldAxis(xAxis, Math.PI);
            }
        }
        else if (move == "M" || move == "r'" || move == "l" || move == "x'") {
            //cycle the edges
            cycle_rotations(cubes[7], cubes[25], cubes[19], cubes[1]);
            //cycle the centers
            cycle_rotations(cubes[16], cubes[22], cubes[10], cubes[4]);
            for (let i of slices['M']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(xAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "L" || move[0] == "l" || move[0] == "x") {
        //for the next nine cubies
        for (let i of slices['L']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "L'" || move == "l'" || move == "x") {
            //cycle the corners
            cycle_rotations(cubes[18], cubes[24], cubes[6], cubes[0]);
            //cycle the edges
            cycle_rotations(cubes[9], cubes[21], cubes[15], cubes[3]);
            for (let i of slices['L']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(xAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "L2" || move == "l2" || move == "x2" || move == "L2'" || move == "l2'" || move == "x2'") {
            //swap the corners
            swap_rotations(cubes[0], cubes[24]);
            swap_rotations(cubes[6], cubes[18]);
            //swap the edges
            swap_rotations(cubes[3], cubes[21]);
            swap_rotations(cubes[9], cubes[15]);
            for (let i of slices['L']) {
                //rotate them 180
                cubes[i].rotateOnWorldAxis(xAxis, Math.PI);
            }
        }
        else if (move == "L" || move == "l" || move == "x'") {
            //cycle the corners
            cycle_rotations(cubes[6], cubes[24], cubes[18], cubes[0]);
            //cycle the edges
            cycle_rotations(cubes[15], cubes[21], cubes[9], cubes[3]);
            for (let i of slices['L']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(xAxis, (Math.PI / 2));
            }
        }
    }
    //Z direction
    if (move[0] == "F" || move[0] == "f" || move[0] == "z") {
        //for the first nine cubies
        for (let i of slices['F']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is U or u
        if (move == "F" || move == "f" || move == "z") {
            //cycle the corners
            cycle_rotations(cubes[8], cubes[26], cubes[24], cubes[6]);
            //cycle the edges
            cycle_rotations(cubes[17], cubes[25], cubes[15], cubes[7]);
            for (let i of slices['F']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(zAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "F2" || move == "f2" || move == "z2" || move == "F2'" || move == "f2'" || move == "z2'") {
            //swap the corners
            swap_rotations(cubes[6], cubes[26]);
            swap_rotations(cubes[8], cubes[24]);
            //swap the edges
            swap_rotations(cubes[7], cubes[25]);
            swap_rotations(cubes[15], cubes[17]);
            for (let i of slices['F']) {
                //rotate them 180
                cubes[i].rotateOnWorldAxis(zAxis, Math.PI);
            }
        }
        else if (move == "F'" || move == "f'" || move == "z'") {
            //cycle the corners
            cycle_rotations(cubes[24], cubes[26], cubes[8], cubes[6]);
            //cycle the edges
            cycle_rotations(cubes[15], cubes[25], cubes[17], cubes[7]);
            for (let i of slices['F']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(zAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "S" || move[0] == "f" || move[0] == "b" || move[0] == "z") {
        //for the next nine cubies
        for (let i of slices['S']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "S'" || move == "f" || move == "b'" || move == "z") {
            //cycle the edges
            cycle_rotations(cubes[5], cubes[23], cubes[21], cubes[3]);
            //cycle the centers
            cycle_rotations(cubes[14], cubes[22], cubes[12], cubes[4]);
            for (let i of slices['S']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(zAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "S2" || move == "f2" || move == "b2" || move == "z2") {
            //swap the edges
            swap_rotations(cubes[3], cubes[23]);
            swap_rotations(cubes[5], cubes[21]);
            //swap the centers
            swap_rotations(cubes[4], cubes[22]);
            swap_rotations(cubes[12], cubes[14]);
            for (let i of slices['S']) {
                //rote them 180
                cubes[i].rotateOnWorldAxis(zAxis, Math.PI);
            }
        }
        else if (move == "S" || move == "f'" || move == "b" || move == "z'") {
            //cycle the edges
            cycle_rotations(cubes[21], cubes[23], cubes[5], cubes[3]);
            //cycle the centers
            cycle_rotations(cubes[12], cubes[22], cubes[14], cubes[4]);
            for (let i of slices['S']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(zAxis, (Math.PI / 2));
            }
        }
    }
    if (move[0] == "B" || move[0] == "b" || move[0] == "z") {
        //for the next nine cubies
        for (let i of slices['B']) {
            //add them back to the scene
            scene.add(cubes[i]);
            scene.add(outlines[i]);
        }
        //If the move is E', d' or u
        if (move == "B'" || move == "b'" || move == "z") {
            //cycle the corners
            cycle_rotations(cubes[2], cubes[20], cubes[18], cubes[0]);
            //cycle the edges
            cycle_rotations(cubes[11], cubes[19], cubes[9], cubes[1]);
            for (let i of slices['B']) {
                //rotate them clockwise
                cubes[i].rotateOnWorldAxis(zAxis, -(Math.PI / 2));
            }
        }
        //for 2 moves,
        else if (move == "B2" || move == "b2" || move == "z2" || move == "B2'" || move == "b2'" || move == "z2'") {
            //swap the corners
            swap_rotations(cubes[0], cubes[20]);
            swap_rotations(cubes[2], cubes[18]);
            //swap the edges
            swap_rotations(cubes[1], cubes[19]);
            swap_rotations(cubes[9], cubes[11]);
            for (let i of slices['B']) {
                //rotate them 180
                cubes[i].rotateOnWorldAxis(zAxis, Math.PI);
            }
        }
        else if (move == "B" || move == "b" || move == "z'") {
            //cycle the corners
            cycle_rotations(cubes[18], cubes[20], cubes[2], cubes[0]);
            //cycle the edges
            cycle_rotations(cubes[9], cubes[19], cubes[11], cubes[1]);
            for (let i of slices['B']) {
                //rotate them counterclockwise
                cubes[i].rotateOnWorldAxis(zAxis, (Math.PI / 2));
            }
        }
    }
    //reset cur to ""
    movesDone.push(cur);
    cur = "";
}

// function to cycle through moves.
function next_move(click = false) {
    //Only do the mode if we aren't currently moving. Without this check we can desync if the user presses next while moving.
    if (!is_moving() && moves != undefined) {
        //If we already did the last move in the list
        if (movenumber >= moves.length) {
            //If there are more steps remaining, grab the next step of sets
            if (stepnumber < steps.length - 1) {
                stepnumber++;
                moves = steps[stepnumber];
                //console.log(moves);
                movenumber = 0;
                if (!click)
                    wait = 20
                $("#alg" + (stepnumber - 1)).removeClass("currentAlg")
                $("#alg" + stepnumber).addClass("currentAlg")
                document.getElementById("alg" + stepnumber).scrollIntoView({ block: "nearest" });
            }
            //If we're finished with the last step, asssume the cube is solved and clear the screen.
            else if (stepnumber == steps.length - 1) {
                applied = solvedCube.map(function (arr) { return arr.slice() });
                movesDone = [];
                running = false;
                $("#currentAlg").html("");
                return
            }
            //Otherwise just clear the screen.
            else {
                running = false;
                $("#currentAlg").html("");
                return
            }
        }
        //If next_move was called manually, wait 2 frames.
        else if (!click)
            wait = 2;
        //Print the current algorithm, with the current step emphasized 
        let html = "<p>";
        for (let i = 0; i < moves.length; i++) {
            if (i == movenumber)
                html += `<span class="currentMove">${moves[i]}</span> `
            else
                html += moves[i] + " ";
        }
        html += "</p>"
        $("#currentAlg").html(html);
        //actually do the move.
        do_move(moves[movenumber]);
        //increment the move counter
        movenumber++;
    }
}

//Function that gives the inverted version of the entered move (note, changes F2 to F2' to invert rotation)
function invert_move(move) {
    if (move.slice(-1) == "'")
        return move.slice(0, -1);
    else
        return move + "'";
}

// function to cycle through backwards.
function prev_move() {
    //Only do the mode if we aren't currently moving. Without this check we can desync if the user presses next while moving.
    if (!is_moving()) {
        //If we're already at the first move, return without doing anything
        if (movenumber == 0)
            return;
        //First decrement the move counter
        movenumber--;
        //Print out the previous move.
        let html = "<p>";
        for (let i = 0; i < moves.length; i++) {
            if (i == movenumber)
                html += `<span class="currentMove">${moves[i]}</span> `
            else
                html += moves[i] + " ";
        }
        html += "</p>"
        $("#currentAlg").html(html);
        //actually do the move in reverse
        do_move(invert_move(moves[movenumber]));
        //If this was the first move,
        if (movenumber == 0) {
            //check for earlier steps, and show the last one.
            if (stepnumber > 0) {
                stepnumber--;
                moves = steps[stepnumber];
                //console.log(moves);
                movenumber = moves.length;
                $("#alg" + (stepnumber + 1)).removeClass("currentAlg")
                $("#alg" + stepnumber).addClass("currentAlg")
                document.getElementById("alg" + stepnumber).scrollIntoView({ block: "nearest" });
            }
        }
    }
}

//temporary code to take an algorithm from get
if (urlparam("alg")) {
    moves = urlparam("alg").replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split((/[\s+]+/)); //split on plus or space
    wait = 20;
    running = true;
}

//temporary code to take a scramble from get
if (urlparam("scramble")) {
    let scramble = urlparam("scramble").replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split((/[\s+]+/));
    for (let move of scramble) {
        //We just do the finishing step (the actual movement) without animation
        finish_move
            (move);
    }
    movesDone = scramble;
}

//The colors of the cube, assuming white is up and green is front.
const colors = ["green", "red", "blue", "orange", "white", "yellow", "grey"]
var color = 6;
//If one of the squared is clicked
$('.square').click(function () {
    //If the square is one of the color pickers
    if (colors.includes(this.id)) {
        //set the color to the number of that color
        color = colors.indexOf(this.id);
        //update cur to show that color
        $("#cur").css("background-color", colors[color])
    }
    //If the selected square is cur, switch back to grey
    else if (this.id == "cur") {
        color = 6
        $("#cur").css("background-color", colors[color])
    }//for all other squares, except for the centers
    else if (parseInt(this.id[3]) != 4) {
        //set the color of the square to the selected color
        $("#" + this.id).css("background-color", colors[color])
        //update the color in the "faces" array
        faces[parseInt(this.id[1])][parseInt(this.id[3])] = color;
    }
});

//Clear the color picker
$('#clear').click(function () {
    for (let i = 0; i < 6; i++) {
        for (let j = 0; j < 9; j++) {
            if (j != 4) {
                faces[i][j] = 6;
                $(`#f${i}s${j}`).css("background-color", colors[6]);
            }
        }
    }
});

//Function to apply a color pattern to the cube.
function apply_pattern(pattern) {
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
    function apply_color(cubie, front, left, fUp = false, lUp = false) {
        if (front == 0) //green
        {
            if (left == 4) //white
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 1) //red
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 5) //yellow
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 2) //blue
        {
            cubie.rotateOnWorldAxis(yAxis, (Math.PI)); //turn 180 degrees
            if (left == 4) //white
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 3) //orange
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 5) //yellow
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 1) //red
        {
            cubie.rotateOnWorldAxis(yAxis, -(Math.PI / 2)); //turn clockwise
            if (left == 4) //white
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 2) //blue
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 5) //yellow
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 3) //orange
        {
            cubie.rotateOnWorldAxis(yAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 4) //white
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 0) //green
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 5) //yellow
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 4) //white
        {
            cubie.rotateOnWorldAxis(xAxis, (Math.PI / 2)); //turn clockwise
            if (left == 2) //blue
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 1) //red
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 0) //green
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 5) //yellow
        {
            cubie.rotateOnWorldAxis(xAxis, -(Math.PI / 2)); //turn clockwise
            if (left == 0) //green
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 1) //red
                cubie.rotateOnWorldAxis(zAxis, Math.PI); //flip over
            if (left == 2) //blue
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn clockwise
        }
        if (front == 6) //for if no known color in front
        {
            if (left == 0)
                cubie.rotateOnWorldAxis(yAxis, -Math.PI);
            if (left == 1)
                cubie.rotateOnWorldAxis(yAxis, Math.PI); //flip over
            if (left == 2)
                cubie.rotateOnWorldAxis(yAxis, Math.PI);
            if (left == 4)
                cubie.rotateOnWorldAxis(zAxis, (Math.PI / 2)); //turn counterclockwise
            if (left == 5)
                cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2)); //turn counterclockwise
        }
        if (fUp) {
            cubie.rotateOnWorldAxis(xAxis, -(Math.PI / 2))
        }
        else if (lUp) {
            cubie.rotateOnWorldAxis(zAxis, -(Math.PI / 2))
        }
    }
    reset_cube(); //first give a blank slate
    //I have to do this one by one, because the code to automate it would be longer than just manually doing it.
    apply_color(cubes[0], opposite_color(faces[2][2]), faces[3][0]);
    apply_color(cubes[1], opposite_color(faces[2][1]), faces[4][1], false, true);
    apply_color(cubes[2], opposite_color(faces[2][0]), opposite_color(faces[1][2]));
    apply_color(cubes[3], faces[4][3], faces[3][1], true, false);
    apply_color(cubes[4], faces[4][4], 6, true, false); //6 means unknown/unnecessary, only used for centers.
    apply_color(cubes[5], faces[4][5], opposite_color(faces[1][1]), true, false);
    apply_color(cubes[6], faces[0][0], faces[3][2]);
    apply_color(cubes[7], faces[0][1], faces[4][7], false, true);
    apply_color(cubes[8], faces[0][2], opposite_color(faces[1][0]));
    apply_color(cubes[9], opposite_color(faces[2][5]), faces[3][3]);
    apply_color(cubes[10], opposite_color(faces[2][4]), 6);
    apply_color(cubes[11], opposite_color(faces[2][3]), opposite_color(faces[1][5]));
    apply_color(cubes[12], 6, faces[3][4]);
    //13 is the center, so nothing to apply
    apply_color(cubes[14], 6, opposite_color(faces[1][4]));
    apply_color(cubes[15], faces[0][3], faces[3][5]);
    apply_color(cubes[16], faces[0][4], 6);
    apply_color(cubes[17], faces[0][5], opposite_color(faces[1][3]));
    apply_color(cubes[18], opposite_color(faces[2][8]), faces[3][6]);
    apply_color(cubes[19], opposite_color(faces[2][7]), opposite_color(faces[5][7]), false, true);
    apply_color(cubes[20], opposite_color(faces[2][6]), opposite_color(faces[1][8]));
    apply_color(cubes[21], opposite_color(faces[5][3]), faces[3][7], true, false);
    apply_color(cubes[22], opposite_color(faces[5][4]), 6, true, false);
    apply_color(cubes[23], opposite_color(faces[5][5]), opposite_color(faces[1][7]), true, false);
    apply_color(cubes[24], faces[0][6], faces[3][8]);
    apply_color(cubes[25], faces[0][7], opposite_color(faces[5][1]), false, true);
    apply_color(cubes[26], faces[0][8], opposite_color(faces[1][6]));
}

const errors = ["Success", "No pattern entered", "Invalid color combination", "OLL parity", "PLL parity", "Error in solving algorithm"]
//If a user applies a pattern
$('#apply').click(function () {
    //First validate the pattern
    $.post("api/validator", { pattern: JSON.stringify(faces) }, function (data) {
        //If it fails, give the error
        let reply = parseInt(data);
        if (reply != 0) {
            $('#error').html(errors[reply]);
        }
        else {
            //then apply the faces
            $('#error').html("&nbsp;");
            apply_pattern(faces);
            applied = faces.map(function (arr) { return arr.slice() });
        }
    });
});

//Run an algorithm on the cube
$('#algForm').submit(function (e) {
    //Only submit if there's no algorithm running
    if (running == false) {
        //In case of mobile, scroll to the top of the screen
        window.scrollTo(0, 0);
        //get the alg from the alg field
        let alg = $('#alg').val();
        //turn it into the moves array
        moves = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
        //reset the move counter
        movenumber = 0;
        //clear the steps
        steps = [];
        //reset the step number to 0
        stepnumber = 0;
        let html = "";
        for (let i = 0; i < moves.length; i++) {
            html += moves[i] + " ";
        }
        $("#currentAlg").html(html);
        //start running if not paused
        if (!paused)
            running = true;
    }
    //and do not submit.
    e.preventDefault();
})

//Disable return on the algorithm field, because there are two submit buttons
$('#algForm').on('keydown', function (event) {
    var x = event.which;
    if (x === 13) {
        event.preventDefault();
    }
});

//Select a pattern from the pattern select and put it in the algorithm field.
$('#pattern').change(function () {
    $('#alg').val(document.getElementById("pattern").value)
})

//Run an algorithm on the cube without showing the moves
$('#scramble').click(function () {
    reset_cube()
    let alg = $('#alg').val();
    let scramble = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
    movenumber = 0;
    steps = [];
    stepnumber = 0;
    for (let move of scramble) {
        //We just do the finishing step (the actual movement) without animation
        finish_move(move);
    }
    movesDone = scramble;
});

//On click events for play/pause, next and prev
$('#playpause').click(function () {
    if (paused) {
        paused = false;
        running = true;
        $('#playpause').html('<img src="static/pause.png" alt="pause"></img>');
    }
    else {
        paused = true;
        running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
});

$('#next').click(function () {
    if (paused == false) {
        paused = true;
        running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    next_move(true);
});

$('#prev').click(function () {
    if (paused == false) {
        paused = true;
        running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    prev_move();
});

//Function to get the solution for the cube, show it in the solution div and add the moves to the steps array.
function solve_cube() {
    //Send the pattern (if applied) and any moves done to it already to the solver
    $.post("api/solver", { pattern: JSON.stringify(applied), scramble: movesDone.join(" ") }, function (data) {
        //empty the steps array
        steps = [];
        //reset the curent step
        stepnumber = 0;

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
                    html += `<li id='alg${algs}' class='algorithm'>${alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "")}</li>\n`;
                    //add he alg into the steps array, ignoring everything but the moves.
                    steps.push(alg.replace(/[^FfBbRrLlUuDdxyzMES\'2 ]/g, "").split(" "))
                    //console.log(steps[algs]);
                    algs++;
                }
            }
            html += "</ul>";
        }
        //put the html into the solution div
        $('#solution').html(html);
        $("#alg" + stepnumber).addClass("currentAlg")
        //put the first step into moves and reset the counter
        moves = steps[0];
        movenumber = 0;
        //start solving the cube
        if (paused == false) {
            running = true;
        }
    });
}


//On click events for solver and reset.
$('#solve').click(function () {
    window.scrollTo(0, 0); //In case of mobile, scroll to the top of the screen
    solve_cube();
});

$('#reset').click(function () { reset_cube(); });

//Vary the movement speed.
$('#speed').on('input', function () {
    mspeed = $('#speed').val() / 100;
    console.log(mspeed);
})

//the animate function
function animate() {
    requestAnimationFrame(animate);
    //If there's a wait, we wait
    if (wait > 0) {
        wait -= Math.max(10 * mspeed, 1);
    }
    //If there is an y move set up
    else if (yRemaining < 0) {
        //if there is less than mspeed remaining, just move mspeed
        if (yRemaining > -mspeed) {
            moving.rotateY(yRemaining);
            yRemaining = 0;
        }
        //otherwise move
        else {
            moving.rotateY(-mspeed);
            yRemaining += mspeed;
        }
    }
    else if (yRemaining > 0) {
        if (yRemaining < mspeed) {
            moving.rotateY(yRemaining);
            yRemaining = 0;
        }
        else {
            moving.rotateY(mspeed);
            yRemaining -= mspeed;
        }
    }
    else if (xRemaining < 0) {
        if (xRemaining > -mspeed) {
            moving.rotateX(xRemaining);
            xRemaining = 0;
        }
        else {
            moving.rotateX(-mspeed);
            xRemaining += mspeed;
        }
    }
    else if (xRemaining > 0) {
        if (xRemaining < mspeed) {
            moving.rotateX(xRemaining);
            xRemaining = 0;
        }
        else {
            moving.rotateX(mspeed);
            xRemaining -= mspeed;
        }
    }
    else if (zRemaining < 0) {
        if (zRemaining > -mspeed) {
            moving.rotateZ(zRemaining);
            zRemaining = 0;
        }
        else {
            moving.rotateZ(-mspeed);
            zRemaining += mspeed;
        }
    }
    else if (zRemaining > 0) {
        if (zRemaining < mspeed) {
            moving.rotateZ(zRemaining);
            zRemaining = 0;
        }
        else {
            moving.rotateZ(mspeed);
            zRemaining -= mspeed;
        }
    }
    else if (cur) {
        finish_move(cur);
    }
    if (running) {
        next_move();
    }
    controls.update();
    renderer.render(scene, camera);
}
animate();
