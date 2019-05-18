$fn=500;

use <transformlib2d.scad>

//Parameters
thickness = 1.01;
width = 19.09;  
length = 19.09;
wall_thickness = 0.41;
height = 18.5; // total height = height + thickness -> only letterbox

thickness_letter_ground_plate = 0.81; // thickness of plate on which letter is printed
letter_overhang = 0.2; // letter thickness on top of letter ground plate

offset = 2; // offset for letter -> size reduction of letter ground plate

stripe_height = 1; // height of stripe -> only letterbox
stripe_width = 11; // width  of stripe -> only letterbox


component = "l"; //"l" = letter or "b" = box
letter = "Ü"; // if single letter (ALT + 0149 = •)

module Letter(letter, size=30, thickness) {

    if (letter == "Ü" || letter == "Ö" || letter == "Ä") {
        text(letter, 
             size=size*0.58, // size for ÖÄÜ
             font="Ender3:style=Regular", 
             halign="center",
             valign="center");
    }
    else {
        text(letter, 
             size=size*0.65, // size for all other letters than ÖÄÜ
             font="Ender3:style=Regular",
             halign="center",
             valign="center");
    }
 
}



module printletterbox(letter) {
 
    // outset = space between shape of letter at letterbox and letter
    linear_extrude(thickness) difference() {
        square([width, length], center=true);
        outset(d=0.3) Letter(letter, size=width);
       }
       
    translate([0, 0, -height/2]) //before z=-2*wall_thickness
    difference() {
        cube([width, length, height], center=true);
        cube([width-2*wall_thickness, length-2*wall_thickness, height], center=true);
        //the led stripe
        //translate([0, 0, -height/2+stripe_height/2]) cube([width, stripe_width, stripe_height], center=true);
    }

}


//color("red", 1) translate([0, 0, thickness]) linear_extrude(thickness + letter_overhang) Letter(letter, size=width);


module printletter(letter) {

    translate([0, 0, thickness]) 
        cube([width-wall_thickness-offset, length-wall_thickness-offset, thickness_letter_ground_plate], center=true);
    color("red", 1) translate([0, 0, thickness]) linear_extrude(thickness + letter_overhang) Letter(letter, size=width);
    
}



//connector

c_thickness = 0; // 0 = no connector 
c_len_y = wall_thickness*2 + c_thickness*2;
c_len_x = stripe_width + 0;
c_height = height/3;

module connectorClip() {
    
difference() {
    union() {
        cube([c_len_x, c_len_y, c_thickness]); //ground
        cube([c_len_x, c_thickness ,c_height]); //side1
        translate([0, c_len_y-c_thickness, 0]) cube([c_len_x, c_thickness ,c_height]); //side2
    }
    translate([(c_len_x-stripe_width)/2, 0, c_thickness]) cube([stripe_width, c_len_y, c_height]);
}

}



module LetterBoxWithConnectors2(top, right, bottom, left, letter) {
    rotate([180, 0, 0])
        difference() {
            printletterbox(letter);
            if (left) {
            translate([-width/2,0,c_thickness/2-height]) cube([c_len_y+0.3, c_len_x+0.3, c_thickness+1+stripe_height], center=true); //ground
            }
            if (right) {
            translate([width/2,0,c_thickness/2-height]) cube([c_len_y+0.3, c_len_x+0.3, c_thickness+1+stripe_height], center=true); //ground
            }
            if (top) {
            translate([0,width/2,c_thickness/2-height]) cube([c_len_x+0.3, c_len_y+0.3, 0], center=true); //ground
            }
            if (bottom) {
            translate([0,-width/2,c_thickness/2-height]) cube([c_len_x+0.3, c_len_y+0.3, 0], center=true); //ground
            }
    }
}


layout = [  ["E","S","S","I","S","T","A","F","Ü","N","F"],
            ["Z","W","A","N","Z","I","G","Z","E","H","N"],
            ["V","I","E","R","T","E","L","N","A","C","H"],
            ["P","G","E","N","A","U","T","V","O","R","P"],
            ["H","A","L","B","E","S","I","E","B","E","N"],
            ["V","I","E","R","E","L","F","D","R","E","I"],
            ["A","Z","W","Ö","L","F","E","I","N","S","T"],
            ["F","Ü","N","F","Z","E","H","N","E","U","N"],
            ["A","C","H","T","C","S","E","C","H","S","L"],
            ["B","N","R","Z","W","E","I","E","I","H","D"],
            ["T","L","U","H","R","P","V","J","A","P","R"],
];

if (component == "b") {
//    LetterBoxWithConnectors2(true, true, true, true, letter);
    for (r=[6:1:(11-1)]) {
        for (i =[5:1:(11-1)]) {
            translate([i*width, r*width, 0]) LetterBoxWithConnectors2(true, true, true, true, layout[r][i]);
        }
    }
}
else {
//    printletter(letter);
    for (r=[6:1:(11-1)]) {
        for (i =[6:1:(11-1)]) {
            translate([i*width, r*width, 0]) printletter(layout[r][i]);
        }
    }
}

