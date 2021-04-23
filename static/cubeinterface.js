import * as THREE from 'https://unpkg.com/three@0.128.0/build/three.module.js';
import { OrbitControls } from 'https://unpkg.com/three@0.127.0/examples/jsm/controls/OrbitControls.js';
import { Cube } from './cube.js';

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
let cube = new Cube(scene, 0, 0, 0, 6);
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
    cube.update();
    controls.update();
    renderer.render(scene, camera);
}
animate();