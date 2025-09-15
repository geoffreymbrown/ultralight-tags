$fn = 120; // steps in generating circles

 makelid = false;
 makebase = true;

version_string = "BTCase"; // imprinted in base and lid
board_file =  "BitTagv7-Edge_Cuts.dxf";

include  <roundedcube.scad>

// helper function
// create a cube centered [x/2,y/2,0]

module centerCube(x,y,z) {
   translate([0,0,z/2]) cube([x,y,z],center=true);
}

module makePCBOutline(height) {
   %linear_extrude(height = height, convexity = 10) 
   import (file = board_file);
}

//
// Parameters for test pins
// 
pogo_pin_height = 2;  // Pin height at pcb
pogo_box_len = 3.2;          // length of pogo pin group
pogo_box_width = 7;          // width of pogo pin group

//screw_rad = 1.0;


//
// PCB cutout and Base dimensions
// These neeed to be gathered from
// kicad pcb file
//
pcb_min_thick = 0.4;                   // fiberglass thickness
pcb_thick     = 2.0;                    // pcb clearance  (for sweeping space)
pcb_clearance = 0.002*25.4;             // edge clearance (routing error)
pcb_len       = 14 + pcb_clearance*2;
pcb_width     = 9 + pcb_clearance*2;
    // battery support lip, length is arbitrary since this is just
    // to punch a hole
lip_len   = 4;                    
lip_width = 4 + pcb_clearance*2;
lip_rad   = 0.5 + pcb_clearance;
lid_height = 3.5;

cutclear = 1.5; // horizontal space around cutout under pcb (/2 on each edge) 
pogo_center = [4.385, 0.0];   // offset from board center of pogo pins

//
// dimensions of base
//

base_width       = pcb_width + 5.0;
base_len         = pcb_len + 14;
base_height      = 4;//1.5;  // base holds 1.5mm board
cut_depth        = 1;                    // clearance under board
post_rad         = 2.0;                    // radius of post support
post_hole_insert = 0.75;                   // clearance for 2-56 insert
post_hole_screw  = 1.3;                    // clearance for 2-56 screw
post_off         = -5.0;                   // offset from pogo pin
post_dist        = 18.0;                   // distance between posts
align_pin_off    = [-5.0,6.5];             // alignment pin offset
align_pin_rad    = 0.75;                   // radius of alignment pin

// generate the PCB 

module makePCBn() {
    makePCBOutline(4);
}


module makePCB(height) {
   centerCube(pcb_len,pcb_width,height);
   translate([pcb_len/2.0+3,0,0])cylinder(r=5,h=height+2);
}

module makePCBx() {
  difference(){
      // positive
     union(){
           // main body
	      centerCube(pcb_len,pcb_width,pcb_thick+2);
           // lip
         translate([(pcb_len+lip_len)/2.0,0,0])
	     centerCube(lip_len,lip_width,pcb_thick);
       
           // boxes for radii
         translate([(pcb_len+lip_rad)/2.0,(lip_width+lip_rad)/2.0,0])
	     centerCube(lip_rad,lip_rad,pcb_thick);
         translate([(pcb_len+lip_rad)/2.0,-(lip_width+lip_rad)/2.0,0])
 	     centerCube(lip_rad,lip_rad,pcb_thick);
         
         
         
       };
     // negative -- cut radii
     union(){
         translate([pcb_len/2.0+lip_rad,lip_width/2.0+lip_rad,0])
		cylinder(r=lip_rad,h=pcb_thick);
         translate([pcb_len/2.0+lip_rad,-lip_width/2.0-lip_rad,0])
                cylinder(r=lip_rad,h=pcb_thick);
      };
     
      };
    // battery
        // translate([pcb_len/2.0+6.5,0,pcb_min_thick])cylinder(r=6.5,h=3.5);
	    // translate([pcb_len/2 - 2,-9/2,pcb_min_thick]) cube([5,9,3.2]); 
  
}

// make a crossbar with endholes -- we need two of these
// one for the base, one for the lid

module makeCrossBar(height,endheight,hole) {
   translate([pogo_center[0]+post_off, pogo_center[1],0])
   difference(){
             union(){
                   // cross piece
                centerCube(2*post_rad,post_dist,height);
                   // end cylinders
                translate([0,post_dist/2,0])
		            cylinder(r=post_rad, h=endheight);
                translate([0,-post_dist/2,0])
		            cylinder(r=post_rad, h=endheight);
               };
              union(){  // punch holes
               translate([0,post_dist/2,0])
                    cylinder(r=hole,h=endheight);
               translate([0,-post_dist/2,0])
                    cylinder(r=hole,h=endheight);
               };
      };
}

// make the base

module makeBase() {
     difference(){
           // positive -- box with cross bars
           union(){
            translate([0,0,-base_height/2])
                union(){
                    //centerCube(base_len,base_width,base_height);
                    roundedcube([base_len,base_width,base_height],true,0.8,"z");
                    translate([0,0,0.85])centerCube(6,base_width+3,1);
                    //makeCrossBar(base_height,base_height,post_hole_insert);
                    translate([-9,0,-2])makeCrossBar(base_height/2,base_height/2,post_hole_insert);
                    translate([ 10,0,-2])makeCrossBar(base_height/2,base_height/2,post_hole_insert);
                
                };
                 translate([base_len/2-2,base_width/2-2,0])cylinder(h=0.8,r=0.7);
                 translate([2-base_len/2,2-base_width/2,0])cylinder(h=0.8,r=0.7);
            }
        {
        
               translate([1,0,-base_height+.3])
                 rotate(a=[0,180,-90])linear_extrude(0.5)	
                 text(text=version_string,size=2,halign="center");
        };
    }
}


module snap(length){
     rotate([0,90,0]) linear_extrude(length)polygon(points=[[0,0],[4,0],[4,0.5],[2.5,2],[2.5,1],[0,1]]);
    
    
}

module makeLid(){
	translate([0,0,lid_height/2])
	        difference(){union(){
                roundedcube([base_len,base_width,lid_height],true,0.8,"z");
                translate([0,0,-lid_height/2])centerCube(5,base_width+5.3,lid_height);
                //translate([0,0,-lid_height/2-1])centerCube(base_len+2,8,lid_height+1);
               
                // snaps
                translate([-2.5,-9.7,-0.5])snap(5);
                rotate([0,0,180])translate([-2.5,-9.7,-0.5])snap(5);
            };
             // text
                 translate([-1,0,lid_height/2-0.5])
                 rotate(a=[0,0,-90])linear_extrude(0.6)	
                 text(text=version_string,size=2,halign="center");
                 translate([base_len/2-2,base_width/2-2,-1])cylinder(h=1,r=0.8);
                 translate([2-base_len/2,2-base_width/2,-1])cylinder(h=1,r=0.8);
            
            };
                 
}

if (makebase) 
{
      translate(makelid?[0,0,0]:[0,0,base_height])difference() {
        makeBase();
        #translate([-3,0,-base_height+1.25])makePCB(3);
   };

}

   
  //makePCB(3.5);

if (makelid) 
    
     rotate(makebase?[0,0,0]:[180,0,0])translate(makebase?[0,0,0]:[0,0,-lid_height])difference() {
     makeLid();
     translate([-3,0,-base_height+1.25])makePCB(3);
   }
   

