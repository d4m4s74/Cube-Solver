import * as THREE from 'https://unpkg.com/three/build/three.module.js';
import { OrbitControls } from 'https://unpkg.com/three/examples/jsm/controls/OrbitControls.js';

//Copied from the jquery codebase
class Cube {
    #geometry = new THREE.BoxGeometry(1.985, 1.985, 1.985); //including outline it's a perfect 2
    //give it colors
    #cubeMaterials = [
        new THREE.MeshBasicMaterial({ color: 'red', transparent: true, opacity: 1, side: THREE.DoubleSide, }), //left
        new THREE.MeshBasicMaterial({ color: 'orange', transparent: true, opacity: 1, side: THREE.DoubleSide }), //right
        new THREE.MeshBasicMaterial({ color: 'white', transparent: true, opacity: 1, side: THREE.DoubleSide }), //up
        new THREE.MeshBasicMaterial({ color: 'yellow', transparent: true, opacity: 1, side: THREE.DoubleSide }), //down
        new THREE.MeshBasicMaterial({ color: 'green', transparent: true, opacity: 1, side: THREE.DoubleSide }), //front
        new THREE.MeshBasicMaterial({ color: 'blue', transparent: true, opacity: 1, side: THREE.DoubleSide }), //back
    ];
    //draw the outlines:
    #edges = new THREE.EdgesGeometry(this.#geometry);
    //create a 27 this.#cubies
    #cubies = [];
    #outlines = [];
    //cube used to store temporary values
    #tempcube = new THREE.Mesh(this.#geometry, this.#cubeMaterials);
    //Create a group for the pieces currently moving
    #moving = new THREE.Group();
    //variables for the moves being made
    #xRemaining = 0;
    #yRemaining = 0;
    #zRemaining = 0;
    //variable for the piece currently moving
    #cur = "";
    //Whether it's running an algorithm
    running = false;
    //whether or not running is paused (for stepping through manually)
    paused = false;
    //The wait time until the next step
    #wait = 0;
    //The default movement speed in radians per frame
    #mspeed = 0.1;
    //The step of a solution being done
    #stepnumber = 0;
    #steps = [];
    //the step in the algorithm being done.
    #movenumber = 0;
    #moves = [];
    //A list of moves done to the cube, in order to step back or send to the solver as a scramble
    #movesDone = []
    //Axes of rotation
    #xAxis = new THREE.Vector3(1, 0, 0);
    #yAxis = new THREE.Vector3(0, 1, 0);
    #zAxis = new THREE.Vector3(0, 0, 1);
    //The cubies that belong to each slice, to make it easier to work with them all in a loop
    #slices = {
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
    
    //set the last applied cube
    #solvedCube = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1, 1],
    [2, 2, 2, 2, 2, 2, 2, 2, 2],
    [3, 3, 3, 3, 3, 3, 3, 3, 3],
    [4, 4, 4, 4, 4, 4, 4, 4, 4],
    [5, 5, 5, 5, 5, 5, 5, 5, 5]];
    #solver_api = "";
    #solution_div = "";
    #current_alg_div = "";
    #applied;
    #scene;
    constructor(scene, applied = this.#solvedCube) {
        this.#scene = scene;
        this.#applied = applied.map(function (arr) { return arr.slice() });
        for (let i = 0; i < 27; i++) {
            //make a mesh based on the before box
            this.#cubies.push(new THREE.Mesh(this.#geometry, this.#cubeMaterials));
            //add it to the scene
            this.#scene.add(this.#cubies[i]);
            //create the outlines and add it to the scene.
            this.#outlines.push(new THREE.LineSegments(this.#edges, new THREE.LineBasicMaterial({ color: 0x000000, linewidth: 0.015 })));
            this.#scene.add(this.#outlines[i]);
        }
        //Position the cubes
        //Set the locations left to right, back to front, top to bottom.
        {
            let i = 0;
            for (let y = 2; y >= -2; y -= 2) {
                for (let z = -2; z <= 2; z += 2) {
                    for (let x = -2; x <= 2; x += 2) {
                        this.#cubies[i].position.set(x, y, z);
                        this.#outlines[i].position.set(x, y, z);
                        i++;
                    }
                }
            }
        }
    }
    //Method to cycle the rotations of 4 cubies a > b > c > d > a
    cycle_rotations(a, b, c, d) {
        this.#tempcube.setRotationFromEuler(a.rotation);
        a.setRotationFromEuler(d.rotation);
        d.setRotationFromEuler(c.rotation);
        c.setRotationFromEuler(b.rotation);
        b.setRotationFromEuler(this.#tempcube.rotation);
    }
    //Method to swap the rotations of two cubies
    swap_rotations(a, b) {
        this.#tempcube.setRotationFromEuler(a.rotation);
        a.setRotationFromEuler(b.rotation);
        b.setRotationFromEuler(this.#tempcube.rotation);
    }
    //Method to reset the cube
    reset_cube() {
        this.running = false;
        this.finish_move(this.#cur)
        this.#xRemaining = 0;
        this.#yRemaining = 0;
        this.#zRemaining = 0;
        this.#wait = 0;
        this.#movesDone = [];
        this.#moves = [];
        this.#movenumber = 0;
        this.#steps = [];
        $(this.#current_alg_div).html("");
        this.#applied = this.#solvedCube.map(function (arr) { return arr.slice() });
        for (let i = 0; i < 27; i++) {
            this.#cubies[i].rotation.x = 0;
            this.#cubies[i].rotation.y = 0;
            this.#cubies[i].rotation.z = 0;
        }
    }
    //returns a bool that says if the cube is moving.
    is_moving() {
        return (this.#yRemaining != 0 || this.#xRemaining != 0 || this.#zRemaining != 0 || this.#cur != "");
    }
    //Function prepares the movement animation
    do_move(move) {
        //If no move is given, or the cube is already moving, do nothing.
        if (!move || this.is_moving()) {
            return;
        }
        this.#cur = move;
        //console.log(move);
        this.#moving.clear();
        //If the first letter of the move is U or u, add the up layer to the moving group
        if (move[0] == "U" || move[0] == "u" || move[0] == "y") {
            for (let i of this.#slices['U']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //Set up the movement for U or u
            if (move == "U" || move == "u" || move == "y") {
                this.#yRemaining = -(Math.PI / 2);
            } else if (move == "U2" || move == "u2" || move == "y2") {
                this.#yRemaining = -Math.PI;
            } else if (move == "U2'" || move == "u2'" || move == "y2'") {
                this.#yRemaining = Math.PI;
            } else if (move == "U'" || move == "u'" || move == "y'") {
                this.#yRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is E or u or d, add the equator layer to the moving group
        if (move[0] == "E" || move[0] == "u" || move[0] == "d" || move[0] == "y") {
            for (let i of this.#slices['E']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //only set up moves for E
            if (move == "E'") {
                this.#yRemaining = -(Math.PI / 2);
            } else if (move == "E2") {
                this.#yRemaining = -Math.PI;
            } else if (move == "E2'") {
                this.#yRemaining = Math.PI;
            } else if (move == "E") {
                this.#yRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is D or d, add the D layer to the moving group
        if (move[0] == "D" || move[0] == "d" || move[0] == "y") {
            for (let i of this.#slices['D']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //set up moves for D and d
            if (move == "D'" || move == "d'") {
                this.#yRemaining = -(Math.PI / 2);
            } else if (move == "D2" || move == "d2") {
                this.#yRemaining = -Math.PI;
            } else if (move == "D2'" || move == "d2'") {
                this.#yRemaining = Math.PI;
            } else if (move == "D" || move == "d") {
                this.#yRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is R or r, add the R layer to the moving group,
        if (move[0] == "R" || move[0] == "r" || move[0] == "x") {
            for (let i of this.#slices['R']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //Set up the movement for R or r
            if (move == "R" || move == "r" || move == "x") {
                this.#xRemaining = -(Math.PI / 2);
            } else if (move == "R2" || move == "r2" || move == "x2") {
                this.#xRemaining = -Math.PI;
            } else if (move == "R2'" || move == "r2'" || move == "x2'") {
                this.#xRemaining = Math.PI;
            } else if (move == "R'" || move == "r'" || move == "x'") {
                this.#xRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is M or r or l, add the middle  layer to the moving group
        if (move[0] == "M" || move[0] == "r" || move[0] == "l" || move[0] == "x") {
            for (let i of this.#slices['M']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //only set up moves for M
            if (move == "M'") {
                this.#xRemaining = -(Math.PI / 2);
            } else if (move == "M2") {
                this.#xRemaining = Math.PI;
            } else if (move == "M2'") {
                this.#xRemaining = -Math.PI;
            } else if (move == "M") {
                this.#xRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is L or l, add the L  layer to the moving group
        if (move[0] == "L" || move[0] == "l" || move[0] == "x") {
            for (let i of this.#slices['L']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //set up moves for L and l
            if (move == "L'" || move == "l'") {
                this.#xRemaining = -(Math.PI / 2);
            } else if (move == "L2" || move == "l2") {
                this.#xRemaining = Math.PI;
            } else if (move == "L2'" || move == "l2'") {
                this.#xRemaining = -Math.PI;
            } else if (move == "L" || move == "l") {
                this.#xRemaining = (Math.PI / 2);
            }
        }
        //FSB
        //If the first letter of the move is F or f, add the F layer to the moving group,
        if (move[0] == "F" || move[0] == "f" || move[0] == "z") {
            for (let i of this.#slices['F']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //Set up the movement for F or f
            if (move == "F" || move == "f" || move == "z") {
                this.#zRemaining = -(Math.PI / 2);
            } else if (move == "F2" || move == "f2" || move == "z2") {
                this.#zRemaining = -Math.PI;
            } else if (move == "F2'" || move == "f2'" || move == "z2'") {
                this.#zRemaining = Math.PI;
            } else if (move == "F'" || move == "f'" || move == "z'") {
                this.#zRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is S or f or b, add the middle  layer to the moving group
        if (move[0] == "S" || move[0] == "f" || move[0] == "b" || move[0] == "z") {
            for (let i of this.#slices['S']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //only set up moves for S
            if (move == "S'") {
                this.#zRemaining = -(Math.PI / 2);
            } else if (move == "S2") {
                this.#zRemaining = Math.PI;
            } else if (move == "S2'") {
                this.#zRemaining = -Math.PI;
            } else if (move == "S") {
                this.#zRemaining = (Math.PI / 2);
            }
        }
        //If the first letter of the move is B or b, add the B  layer to the moving group
        if (move[0] == "B" || move[0] == "b" || move[0] == "z") {
            for (let i of this.#slices['B']) {
                this.#moving.add(this.#cubies[i]);
                this.#moving.add(this.#outlines[i]);
                this.#scene.add(this.#moving);
            }
            //set up moves for B and b
            if (move == "B'" || move == "b'") {
                this.#zRemaining = -(Math.PI / 2);
            } else if (move == "B2" || move == "b2") {
                this.#zRemaining = Math.PI;
            } else if (move == "B2'" || move == "b2'") {
                this.#zRemaining = -Math.PI;
            } else if (move == "B" || move == "b") {
                this.#zRemaining = (Math.PI / 2);
            }
        }
    }
    //Method actually moves the pieces (or gives the same effect)
    finish_move(move) {

        //Once the move animation is done, actually turn the cubies
        //Because the cubies don't really move, the basic idea is to copy the rotation of the cubie that on a real cube would move into its place, and then rotate everything 
        //Unfortunately there is no easier, more efficient way to do this.

        //If no move argument is given, return doing nothing.
        if (!move) {
            return;
        }
        //Clear the moving group
        this.#moving.clear();
        //reset its rotation
        this.#moving.rotation.x = 0;
        this.#moving.rotation.y = 0;
        this.#moving.rotation.z = 0;

        if (move[0] == "U" || move[0] == "u" || move[0] == 'y') {
            //for the first nine cubies
            for (let i of this.#slices['U']) {
                //add them back to the scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is U or u
            if (move == "U" || move == "u" || move == "y") {
                //Cycle the rotation of the corners
                this.cycle_rotations(this.#cubies[2], this.#cubies[8], this.#cubies[6], this.#cubies[0]);
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[5], this.#cubies[7], this.#cubies[3], this.#cubies[1]);
                for (let i of this.#slices['U']) {
                    //rotate them all clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "U2" || move == "u2" || move == "y2" || move == "U2'" || move == "u2'" || move == "y2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[0], this.#cubies[8]);
                this.swap_rotations(this.#cubies[2], this.#cubies[6]);
                //swap the edges
                this.swap_rotations(this.#cubies[1], this.#cubies[7]);
                this.swap_rotations(this.#cubies[3], this.#cubies[5]);
                for (let i of this.#slices['U']) {
                    //rote them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, Math.PI);
                }
            }
            //Inverted move
            else if (move == "U'" || move == "u'" || move == "y'") {
                //Cycle the rotation of the corners
                this.cycle_rotations(this.#cubies[6], this.#cubies[8], this.#cubies[2], this.#cubies[0]);
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[3], this.#cubies[7], this.#cubies[5], this.#cubies[1]);
                for (let i of this.#slices['U']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "E" || move[0] == "u" || move[0] == "d" || move[0] == 'y') {
            //for the next nine cubies
            for (let i of this.#slices['E']) {
                //add them back to the scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "E'" || move == "u" || move == "d'" || move == "y") {
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[11], this.#cubies[17], this.#cubies[15], this.#cubies[9]);
                //Cycle the rotation of the centers
                this.cycle_rotations(this.#cubies[14], this.#cubies[16], this.#cubies[12], this.#cubies[10]);
                for (let i of this.#slices['E']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "E2" || move == "u2" || move == "d2" || move == "y2" || move == "E2'" || move == "u2'" || move == "d2'" ||
                move == "y2'") {
                //swap the edges
                this.swap_rotations(this.#cubies[9], this.#cubies[17]);
                this.swap_rotations(this.#cubies[11], this.#cubies[15]);
                //swap the centers
                this.swap_rotations(this.#cubies[10], this.#cubies[16]);
                this.swap_rotations(this.#cubies[12], this.#cubies[14]);
                for (let i of this.#slices['E']) {
                    //rotate them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, Math.PI);
                }
            } else if (move == "E" || move == "u'" || move == "d" || move == "y'") {
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[15], this.#cubies[17], this.#cubies[11], this.#cubies[9]);
                //Cycle the rotation of the centers
                this.cycle_rotations(this.#cubies[12], this.#cubies[16], this.#cubies[14], this.#cubies[10]);
                for (let i of this.#slices['E']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "D" || move[0] == "d" || move[0] == 'y') {
            //for the next nine cubies
            for (let i of this.#slices['D']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "D'" || move == "d'" || move == "y") {
                //Cycle the rotation of the corners
                this.cycle_rotations(this.#cubies[20], this.#cubies[26], this.#cubies[24], this.#cubies[18]);
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[23], this.#cubies[25], this.#cubies[21], this.#cubies[19]);
                for (let i of this.#slices['D']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "D2" || move == "d2" || move == "y2" || move == "D2'" || move == "d2'" || move == "y2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[18], this.#cubies[26]);
                this.swap_rotations(this.#cubies[20], this.#cubies[24]);
                //swap the edges
                this.swap_rotations(this.#cubies[19], this.#cubies[25]);
                this.swap_rotations(this.#cubies[21], this.#cubies[23]);
                for (let i of this.#slices['D']) {
                    //rote them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, Math.PI);
                }
            } else if (move == "D" || move == "d" || move == "y'") {
                //Cycle the rotation of the corners
                this.cycle_rotations(this.#cubies[24], this.#cubies[26], this.#cubies[20], this.#cubies[18]);
                //Cycle the rotation of the edges
                this.cycle_rotations(this.#cubies[21], this.#cubies[25], this.#cubies[23], this.#cubies[19]);
                for (let i of this.#slices['D']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#yAxis, (Math.PI / 2));
                }
            }
        }
        //Now the X direction
        if (move[0] == "R" || move[0] == "r" || move[0] == "x") {
            //for the first nine cubies
            for (let i of this.#slices['R']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is U or u
            if (move == "R" || move == "r" || move == "x") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[20], this.#cubies[26], this.#cubies[8], this.#cubies[2]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[11], this.#cubies[23], this.#cubies[17], this.#cubies[5]);
                for (let i of this.#slices['R']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "R2" || move == "r2" || move == "x2" || move == "R2'" || move == "r2'" || move == "x2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[2], this.#cubies[26]);
                this.swap_rotations(this.#cubies[8], this.#cubies[20]);
                //swap the edges
                this.swap_rotations(this.#cubies[5], this.#cubies[23]);
                this.swap_rotations(this.#cubies[11], this.#cubies[17]);
                for (let i of this.#slices['R']) {
                    //rotate them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, Math.PI);
                }
            } else if (move == "R'" || move == "r'" || move == "x'") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[8], this.#cubies[26], this.#cubies[20], this.#cubies[2]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[17], this.#cubies[23], this.#cubies[11], this.#cubies[5]);
                for (let i of this.#slices['R']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "M" || move[0] == "r" || move[0] == "l" || move[0] == "x") {
            //for the next nine cubies
            for (let i of this.#slices['M']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "M'" || move == "r" || move == "l'" || move == "x") {
                //cycle the edges
                this.cycle_rotations(this.#cubies[19], this.#cubies[25], this.#cubies[7], this.#cubies[1]);
                //cycle the centers
                this.cycle_rotations(this.#cubies[10], this.#cubies[22], this.#cubies[16], this.#cubies[4]);
                for (let i of this.#slices['M']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "M2" || move == "r2" || move == "l2" || move == "x2" || move == "M2'" || move == "r2'" ||
                move == "l2'" || move == "x2'") {
                //swap the edges
                this.swap_rotations(this.#cubies[1], this.#cubies[25]);
                this.swap_rotations(this.#cubies[7], this.#cubies[19]);
                //swap the centers
                this.swap_rotations(this.#cubies[4], this.#cubies[22]);
                this.swap_rotations(this.#cubies[10], this.#cubies[16]);
                for (let i of this.#slices['M']) {
                    //rote them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, Math.PI);
                }
            } else if (move == "M" || move == "r'" || move == "l" || move == "x'") {
                //cycle the edges
                this.cycle_rotations(this.#cubies[7], this.#cubies[25], this.#cubies[19], this.#cubies[1]);
                //cycle the centers
                this.cycle_rotations(this.#cubies[16], this.#cubies[22], this.#cubies[10], this.#cubies[4]);
                for (let i of this.#slices['M']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "L" || move[0] == "l" || move[0] == "x") {
            //for the next nine cubies
            for (let i of this.#slices['L']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "L'" || move == "l'" || move == "x") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[18], this.#cubies[24], this.#cubies[6], this.#cubies[0]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[9], this.#cubies[21], this.#cubies[15], this.#cubies[3]);
                for (let i of this.#slices['L']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "L2" || move == "l2" || move == "x2" || move == "L2'" || move == "l2'" || move == "x2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[0], this.#cubies[24]);
                this.swap_rotations(this.#cubies[6], this.#cubies[18]);
                //swap the edges
                this.swap_rotations(this.#cubies[3], this.#cubies[21]);
                this.swap_rotations(this.#cubies[9], this.#cubies[15]);
                for (let i of this.#slices['L']) {
                    //rotate them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, Math.PI);
                }
            } else if (move == "L" || move == "l" || move == "x'") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[6], this.#cubies[24], this.#cubies[18], this.#cubies[0]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[15], this.#cubies[21], this.#cubies[9], this.#cubies[3]);
                for (let i of this.#slices['L']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#xAxis, (Math.PI / 2));
                }
            }
        }
        //Z direction
        if (move[0] == "F" || move[0] == "f" || move[0] == "z") {
            //for the first nine cubies
            for (let i of this.#slices['F']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is U or u
            if (move == "F" || move == "f" || move == "z") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[8], this.#cubies[26], this.#cubies[24], this.#cubies[6]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[17], this.#cubies[25], this.#cubies[15], this.#cubies[7]);
                for (let i of this.#slices['F']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "F2" || move == "f2" || move == "z2" || move == "F2'" || move == "f2'" || move == "z2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[6], this.#cubies[26]);
                this.swap_rotations(this.#cubies[8], this.#cubies[24]);
                //swap the edges
                this.swap_rotations(this.#cubies[7], this.#cubies[25]);
                this.swap_rotations(this.#cubies[15], this.#cubies[17]);
                for (let i of this.#slices['F']) {
                    //rotate them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, Math.PI);
                }
            } else if (move == "F'" || move == "f'" || move == "z'") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[24], this.#cubies[26], this.#cubies[8], this.#cubies[6]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[15], this.#cubies[25], this.#cubies[17], this.#cubies[7]);
                for (let i of this.#slices['F']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "S" || move[0] == "f" || move[0] == "b" || move[0] == "z") {
            //for the next nine cubies
            for (let i of this.#slices['S']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "S'" || move == "f" || move == "b'" || move == "z") {
                //cycle the edges
                this.cycle_rotations(this.#cubies[5], this.#cubies[23], this.#cubies[21], this.#cubies[3]);
                //cycle the centers
                this.cycle_rotations(this.#cubies[14], this.#cubies[22], this.#cubies[12], this.#cubies[4]);
                for (let i of this.#slices['S']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "S2" || move == "f2" || move == "b2" || move == "z2") {
                //swap the edges
                this.swap_rotations(this.#cubies[3], this.#cubies[23]);
                this.swap_rotations(this.#cubies[5], this.#cubies[21]);
                //swap the centers
                this.swap_rotations(this.#cubies[4], this.#cubies[22]);
                this.swap_rotations(this.#cubies[12], this.#cubies[14]);
                for (let i of this.#slices['S']) {
                    //rote them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, Math.PI);
                }
            } else if (move == "S" || move == "f'" || move == "b" || move == "z'") {
                //cycle the edges
                this.cycle_rotations(this.#cubies[21], this.#cubies[23], this.#cubies[5], this.#cubies[3]);
                //cycle the centers
                this.cycle_rotations(this.#cubies[12], this.#cubies[22], this.#cubies[14], this.#cubies[4]);
                for (let i of this.#slices['S']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, (Math.PI / 2));
                }
            }
        }
        if (move[0] == "B" || move[0] == "b" || move[0] == "z") {
            //for the next nine cubies
            for (let i of this.#slices['B']) {
                //add them back to the this.#scene
                this.#scene.add(this.#cubies[i]);
                this.#scene.add(this.#outlines[i]);
            }
            //If the move is E', d' or u
            if (move == "B'" || move == "b'" || move == "z") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[2], this.#cubies[20], this.#cubies[18], this.#cubies[0]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[11], this.#cubies[19], this.#cubies[9], this.#cubies[1]);
                for (let i of this.#slices['B']) {
                    //rotate them clockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2));
                }
            }
            //for 2 moves,
            else if (move == "B2" || move == "b2" || move == "z2" || move == "B2'" || move == "b2'" || move == "z2'") {
                //swap the corners
                this.swap_rotations(this.#cubies[0], this.#cubies[20]);
                this.swap_rotations(this.#cubies[2], this.#cubies[18]);
                //swap the edges
                this.swap_rotations(this.#cubies[1], this.#cubies[19]);
                this.swap_rotations(this.#cubies[9], this.#cubies[11]);
                for (let i of this.#slices['B']) {
                    //rotate them 180
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, Math.PI);
                }
            } else if (move == "B" || move == "b" || move == "z'") {
                //cycle the corners
                this.cycle_rotations(this.#cubies[18], this.#cubies[20], this.#cubies[2], this.#cubies[0]);
                //cycle the edges
                this.cycle_rotations(this.#cubies[9], this.#cubies[19], this.#cubies[11], this.#cubies[1]);
                for (let i of this.#slices['B']) {
                    //rotate them counterclockwise
                    this.#cubies[i].rotateOnWorldAxis(this.#zAxis, (Math.PI / 2));
                }
            }
        }
        //reset cur to ""
        this.#movesDone.push(this.#cur);
        this.#cur = "";
    }
    //Method to cycle through moves.
    next_move(click = false) {
        //Only do the mode if we aren't currently moving. Without this check we can desync if the user presses next while moving.
        if (!this.is_moving() && this.#moves != undefined) {
            //If we already did the last move in the list
            if (this.#movenumber >= this.#moves.length) {
                //If there are more steps remaining, grab the next step of sets
                if (this.#stepnumber < this.#steps.length - 1) {
                    this.#stepnumber++;
                    this.#moves = this.#steps[this.#stepnumber];
                    //console.log(moves);
                    this.#movenumber = 0;
                    if (!click)
                        this.wait = 20
                    $("#alg" + (this.#stepnumber - 1)).removeClass("currentAlg")
                    $("#alg" + this.#stepnumber).addClass("currentAlg")
                    document.getElementById("alg" + this.#stepnumber).scrollIntoView({ block: "nearest" });
                }
                //If we're finished with the last step, asssume the cube is solved and clear the screen.
                else if (this.#stepnumber == this.#steps.length - 1) {
                    this.#applied = this.#solvedCube.map(function (arr) { return arr.slice() });
                    this.#movesDone = [];
                    this.running = false;
                    $(this.#current_alg_div).html("");
                    return
                }
                //Otherwise just clear the screen.
                else {
                    this.running = false;
                    $(this.#current_alg_div).html("");
                    return
                }
            }
            //If next_move was called manually, wait 2 frames.
            else if (!click)
                this.#wait = 2;
            //Print the current algorithm, with the current step emphasized 
            let html = "<p>";
            for (let i = 0; i < this.#moves.length; i++) {
                if (i == this.#movenumber)
                    html += `<span class="currentMove">${this.#moves[i]}</span> `
                else
                    html += this.#moves[i] + " ";
            }
            html += "</p>"
            $(this.#current_alg_div).html(html);
            //actually do the move.
            this.do_move(this.#moves[this.#movenumber]);
            //increment the move counter
            this.#movenumber++;
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
            if (this.#movenumber == 0)
                return;
            //First decrement the move counter
            this.#movenumber--;
            //Print out the previous move.
            let html = "<p>";
            for (let i = 0; i < this.#moves.length; i++) {
                if (i == this.#movenumber)
                    html += `<span class="currentMove">${this.#moves[i]}</span> `
                else
                    html += this.#moves[i] + " ";
            }
            html += "</p>"
            $(this.#current_alg_div).html(html);
            //actually do the move in reverse
            do_move(invert_move(this.#moves[this.#movenumber]));
            //If this was the first move,
            if (this.#movenumber == 0) {
                //check for earlier steps, and show the last one.
                if (this.#stepnumber > 0) {
                    this.#stepnumber--;
                    this.#moves = this.#steps[this.#stepnumber];
                    //console.log(moves);
                    this.#movenumber = this.#moves.length;
                    $("#alg" + (this.#stepnumber + 1)).removeClass("currentAlg")
                    $("#alg" + this.#stepnumber).addClass("currentAlg")
                    document.getElementById("alg" + this.#stepnumber).scrollIntoView({ block: "nearest" });
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
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 2) //blue
            {
                cubie.rotateOnWorldAxis(this.#yAxis, (Math.PI)); //turn 180 degrees
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 3) //orange
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 1) //red
            {
                cubie.rotateOnWorldAxis(this.#yAxis, -(Math.PI / 2)); //turn clockwise
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 3) //orange
            {
                cubie.rotateOnWorldAxis(this.#yAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 4) //white
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 5) //yellow
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 4) //white
            {
                cubie.rotateOnWorldAxis(this.#xAxis, (Math.PI / 2)); //turn clockwise
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 5) //yellow
            {
                cubie.rotateOnWorldAxis(this.#xAxis, -(Math.PI / 2)); //turn clockwise
                if (left == 0) //green
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 1) //red
                    cubie.rotateOnWorldAxis(this.#zAxis, Math.PI); //flip over
                if (left == 2) //blue
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn clockwise
            }
            if (front == 6) //for if no known color in front
            {
                if (left == 0)
                    cubie.rotateOnWorldAxis(this.#yAxis, -Math.PI);
                if (left == 1)
                    cubie.rotateOnWorldAxis(this.#yAxis, Math.PI); //flip over
                if (left == 2)
                    cubie.rotateOnWorldAxis(this.#yAxis, Math.PI);
                if (left == 4)
                    cubie.rotateOnWorldAxis(this.#zAxis, (Math.PI / 2)); //turn counterclockwise
                if (left == 5)
                    cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2)); //turn counterclockwise
            }
            if (fUp) {
                cubie.rotateOnWorldAxis(this.#xAxis, -(Math.PI / 2))
            } else if (lUp) {
                cubie.rotateOnWorldAxis(this.#zAxis, -(Math.PI / 2))
            }
        }
        //bind the function to this class.
        var apply_color = unbound_apply_color.bind(this);
        this.reset_cube(); //first give a blank slate
        //I have to do this one by one, because the code to automate it would be longer than just manually doing it.
        apply_color(this.#cubies[0], opposite_color(pattern[2][2]), pattern[3][0]);
        apply_color(this.#cubies[1], opposite_color(pattern[2][1]), pattern[4][1], false, true);
        apply_color(this.#cubies[2], opposite_color(pattern[2][0]), opposite_color(pattern[1][2]));
        apply_color(this.#cubies[3], pattern[4][3], pattern[3][1], true, false);
        apply_color(this.#cubies[4], pattern[4][4], 6, true, false); //6 means unknown/unnecessary, only used for centers.
        apply_color(this.#cubies[5], pattern[4][5], opposite_color(pattern[1][1]), true, false);
        apply_color(this.#cubies[6], pattern[0][0], pattern[3][2]);
        apply_color(this.#cubies[7], pattern[0][1], pattern[4][7], false, true);
        apply_color(this.#cubies[8], pattern[0][2], opposite_color(pattern[1][0]));
        apply_color(this.#cubies[9], opposite_color(pattern[2][5]), pattern[3][3]);
        apply_color(this.#cubies[10], opposite_color(pattern[2][4]), 6);
        apply_color(this.#cubies[11], opposite_color(pattern[2][3]), opposite_color(pattern[1][5]));
        apply_color(this.#cubies[12], 6, pattern[3][4]);
        //13 is the center, so nothing to apply
        apply_color(this.#cubies[14], 6, opposite_color(pattern[1][4]));
        apply_color(this.#cubies[15], pattern[0][3], pattern[3][5]);
        apply_color(this.#cubies[16], pattern[0][4], 6);
        apply_color(this.#cubies[17], pattern[0][5], opposite_color(pattern[1][3]));
        apply_color(this.#cubies[18], opposite_color(pattern[2][8]), pattern[3][6]);
        apply_color(this.#cubies[19], opposite_color(pattern[2][7]), opposite_color(pattern[5][7]), false, true);
        apply_color(this.#cubies[20], opposite_color(pattern[2][6]), opposite_color(pattern[1][8]));
        apply_color(this.#cubies[21], opposite_color(pattern[5][3]), pattern[3][7], true, false);
        apply_color(this.#cubies[22], opposite_color(pattern[5][4]), 6, true, false);
        apply_color(this.#cubies[23], opposite_color(pattern[5][5]), opposite_color(pattern[1][7]), true, false);
        apply_color(this.#cubies[24], pattern[0][6], pattern[3][8]);
        apply_color(this.#cubies[25], pattern[0][7], opposite_color(pattern[5][1]), false, true);
        apply_color(this.#cubies[26], pattern[0][8], opposite_color(pattern[1][6]));
        this.#applied = pattern.map(function (arr) { return arr.slice() });
    }

    //Method to run and animate an algorithm on the cube
    run_alg(alg) {
        if (this.running == false) {
            //alg it into a moves array
            this.#moves = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
            //reset the move counter
            this.#movenumber = 0;
            //clear the steps
            this.#steps = [];
            //reset the step number to 0
            this.#stepnumber = 0;
            let html = "";
            for (let i = 0; i < this.#moves.length; i++) {
                html += this.#moves[i] + " ";
            }
            $(this.#current_alg_div).html(html);
            //start running if not paused
            if (!this.paused)
                this.running = true;
        }
    }

    //Method to apply an algorithm to the cube without animation
    apply_alg(alg) {
        this.reset_cube()
        let scramble = alg.replace(/[^FfBbRrLlUuDdxyzMES\'2+ ]/g, "").split(" ");
        this.#movenumber = 0;
        this.#steps = [];
        this.#stepnumber = 0;
        $(this.#solution_div).html("");
        for (let move of scramble) {
            //We just do the finishing step (the actual movement) without animation
            this.finish_move(move);
        }
        this.#movesDone = scramble;
    }

    set_solver_api(url) {
        this.#solver_api = url;
    }

    set_solution_div(id) {
        this.#solution_div = id;
    }

    set_current_alg_div(id) {
        this.#current_alg_div = id;
    }

    //Method to solve the cube
    solve_cube() {
        //if no solver api is set, return false
        if (this.#solver_api == "")
            return false;
        //in order to call class functions and use variables I need to make a reference to this
        var thisClass = this;
        //Send the pattern (if applied) and any moves done to it already to the solver
        $.post(thisClass.#solver_api, { pattern: JSON.stringify(this.#applied), scramble: this.#movesDone.join(" ") }, function (data) {
            //empty the steps array
            thisClass.#steps = [];
            //reset the curent step
            thisClass.#stepnumber = 0;

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
                        thisClass.#steps.push(alg.replace(/[^FfBbRrLlUuDdxyzMES\'2 ]/g, "").split(" "))
                        //console.log(steps[algs]);
                        algs++;
                    }
                }
                html += "</ul>";
            }
            //put the html into the solution div
            $(thisClass.#solution_div).html(html);
            $("#alg" + thisClass.#stepnumber).addClass("currentAlg")
            //put the first step into moves and reset the counter
            thisClass.#moves = thisClass.#steps[0];
            thisClass.#movenumber = 0;
            //start solving the cube
            if (thisClass.paused == false) {
                thisClass.running = true;
            }
            return true;
        });
    }

    //Method to set the next movement speed in radians per frame.
    set_movement_speed(mspeed) {
        this.#mspeed = mspeed;
    }

    //Method to grab the next animation frame
    animate() {
        if (this.#wait > 0) {
            this.#wait -= Math.max(10 * this.#mspeed, 1);
        }
        //If there is an y move set up
        else if (this.#yRemaining < 0) {
            //if there is less than mspeed remaining, just move mspeed
            if (this.#yRemaining > -this.#mspeed) {
                this.#moving.rotateY(this.#yRemaining);
                this.#yRemaining = 0;
            }
            //otherwise move
            else {
                this.#moving.rotateY(-this.#mspeed);
                this.#yRemaining += this.#mspeed;
            }
        } else if (this.#yRemaining > 0) {
            if (this.#yRemaining < this.#mspeed) {
                this.#moving.rotateY(this.#yRemaining);
                this.#yRemaining = 0;
            } else {
                this.#moving.rotateY(this.#mspeed);
                this.#yRemaining -= this.#mspeed;
            }
        } else if (this.#xRemaining < 0) {
            if (this.#xRemaining > -this.#mspeed) {
                this.#moving.rotateX(this.#xRemaining);
                this.#xRemaining = 0;
            } else {
                this.#moving.rotateX(-this.#mspeed);
                this.#xRemaining += this.#mspeed;
            }
        } else if (this.#xRemaining > 0) {
            if (this.#xRemaining < this.#mspeed) {
                this.#moving.rotateX(this.#xRemaining);
                this.#xRemaining = 0;
            } else {
                this.#moving.rotateX(this.#mspeed);
                this.#xRemaining -= this.#mspeed;
            }
        } else if (this.#zRemaining < 0) {
            if (this.#zRemaining > -this.#mspeed) {
                this.#moving.rotateZ(this.#zRemaining);
                this.#zRemaining = 0;
            } else {
                this.#moving.rotateZ(-this.#mspeed);
                this.#zRemaining += this.#mspeed;
            }
        } else if (this.#zRemaining > 0) {
            if (this.#zRemaining < this.#mspeed) {
                this.#moving.rotateZ(this.#zRemaining);
                this.#zRemaining = 0;
            } else {
                this.#moving.rotateZ(this.#mspeed);
                this.#zRemaining -= this.#mspeed;
            }
        } else if (this.#cur) {
            this.finish_move(this.#cur);
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

const errors = ["Success", "No pattern entered", "Invalid color combination", "OLL parity", "PLL parity",
    "Error in solving algorithm"]
//If a user applies a pattern
//Apply a pattern to the cube
$('#apply').click(function () {
    //First validate the pattern
    $.post("api/validator", { pattern: JSON.stringify(faces) }, function (data) {
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
$('#algForm').submit(function (e) {
    //run the alg
    cube.run_alg($('#alg').val());
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

//Run an algorithm on the cube without showing the moves
$('#scramble').click(function () {
    cube.apply_alg($('#alg').val());
});
//Select a pattern from the pattern select and put it in the algorithm field.
$('#pattern').change(function () {
    $('#alg').val(document.getElementById("pattern").value)
})
//On click events for play/pause, next and prev
$('#playpause').click(function () {
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

$('#next').click(function () {
    if (cube.paused == false) {
        cube.paused = true;
        cube.running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    cube.next_move(true);
});

$('#prev').click(function () {
    if (cube.paused == false) {
        cube.paused = true;
        cube.running = false;
        $('#playpause').html('<img src="static/play.png" alt="play"></img>');
    }
    cube.prev_move();
});

//On click events for solver and reset.
$('#solve').click(function () {
    window.scrollTo(0, 0); //In case of mobile, scroll to the top of the screen
    cube.solve_cube();
});

$('#reset').click(function () {
    //First finish the moves so no new moves happen after the rest
    cube.running = false;
    cube.finish_move(this.cur);
    cube.reset_cube();
});

//Vary the movement speed.
$('#speed').on('input', function () {
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