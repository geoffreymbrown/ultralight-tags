$fn = 120; // steps in generating circles

makelid = true;
makebase = true;

version_string = "BitTagVE"; // imprinted in base and lid

// helper function
// create a cube centered [x/2,y/2,0]

module centerCube(x,y,z) {
   translate([0,0,z/2]) cube([x,y,z],center=true);
}

//
// Parameters for test pins
// 
pogo_pin_height = 6.27-0.9;  // Pin height at pcb
pogo_box_len = 3.2;          // length of pogo pin group
pogo_box_width = 7;          // width of pogo pin group

//screw_rad = 1.0;


//
// PCB cutout and Base dimensions
// These neeed to be gathered from
// kicad pcb file
//
pcb_min_thick = 0.4;                   // fiberglass thickness
pcb_thick     = 4.0;                    // pcb clearance  (for sweeping space)
pcb_clearance = 0.004*25.4;             // edge clearance (routing error)
pcb_len       = 13.5 + pcb_clearance*2;
pcb_width     = 9 + pcb_clearance*2;
    // battery support lip, length is arbitrary since this is just
    // to punch a hole
lip_len   = 8;                    
lip_width = 4 + pcb_clearance*2;
lip_rad   = 0.5 + pcb_clearance;

cutclear = 1.5; // horizontal space around cutout under pcb (/2 on each edge) 
pogo_center = [4.385, 0.0];   // offset from board center of pogo pins

//
// dimensions of base
//

base_width       = pcb_width + 7.0;
base_len         = pcb_len + 8.0;
base_height      = pogo_pin_height + 2;//1.5;  // base holds 1.5mm board
cut_depth        = 1.5;                    // clearance under board
post_rad         = 3.0;                    // radius of post support
post_hole_insert = 1.65;                   // clearance for 2-56 insert
post_hole_screw  = 1.3;                    // clearance for 2-56 screw
post_off         = -5.0;                   // offset from pogo pin
post_dist        = 20.0;                   // distance between posts
align_pin_off    = [-5.0,6.5];             // alignment pin offset
align_pin_rad    = 0.75;                   // radius of alignment pin

// generate the PCB outline

module makePCB() {
  difference(){
      // positive
     union(){
           // main body
	 centerCube(pcb_len,pcb_width,pcb_thick);
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
        translate([0,0,-pogo_pin_height])
	        union(){
                centerCube(base_len,base_width,base_height);
 	            makeCrossBar(base_height,base_height,post_hole_insert);
            };
	    // negative -- pcb cutout, pogopin cutout,
	    //             slots for harness thread,
	    //             alignment pins
        union(){
   	       translate([0,0,-cut_depth])
               union(){
	                 // pcb clearance
		
	                centerCube(pcb_len-cutclear, pcb_width-cutclear,
		                       pcb_thick+cut_depth);

                    // harness slots
		
  	                translate([-pcb_len/2.0+cutclear/2+1.25,0,0])
		                centerCube(2.5,base_width,pcb_thick+cut_depth);
                    translate([pcb_len/2.0-cutclear/2-1.25,0,0])
	                    centerCube(2.5,base_width,pcb_thick+cut_depth);
		   
  	                // cleanup
		
 	                translate([pcb_len/2.0-cutclear/2-1.25,base_width/2,0])
		                cylinder(r=1.25,h=pcb_thick+cut_depth);
   	                translate([pcb_len/2.0-cutclear/2-1.25,-base_width/2,0])
		                cylinder(r=1.25,h=pcb_thick+cut_depth);
  	             };

             // pogo pin cutout
	     
	        translate([pogo_center[0],pogo_center[1],-pogo_pin_height-0.1])
	            centerCube(pogo_box_len,pogo_box_width,pogo_pin_height+4.0);

             // alignment pins

            translate([pogo_center[0]+align_pin_off[0],
			           pogo_center[1]+align_pin_off[1],
			           -pogo_pin_height-0.1])
	                        cylinder(r=align_pin_rad,h=pcb_thick+pogo_pin_height);
  	        translate([pogo_center[0]+align_pin_off[0],
	                   pogo_center[1]-align_pin_off[1],
		               -pogo_pin_height-0.1])
  	                        cylinder(r=align_pin_rad,h=pcb_thick+pogo_pin_height);
             
             // text

            translate([-4,0,-pogo_pin_height+0.4])
                 rotate(a=[0,180,-90])linear_extrude(0.5)	
                 text(text=version_string,size=2,halign="center");
           translate([8,4.5,pcb_thick/2-0.4])
                 linear_extrude(0.5)       
                 text(text="*",size=3,halign="center");

             // thin battery end
  // battery
         translate([pcb_len/2.0+6.5,0,pcb_min_thick])cylinder(r=6.5,h=pcb_thick);
	        translate([pcb_len/2 - 2,-7/2,pcb_min_thick])
	         cube([5,7,pcb_thick]);
	               
        };
     };
}

module makeLid(){
	translate([0,0,10])difference(){
        union(){
         end_height = 4;
         end_length = 2;
         end_width = pcb_width - 1;
	     makeCrossBar(3,4,post_hole_screw);
	     // end pieces -- inset from pcb edge by 1.1
	     translate([-pcb_len/2+1.1,0,-end_height/2])
	         centerCube(end_length,end_width,end_height);
             // centered over the pogo pins
	     translate([pogo_center[0],pogo_center[1],-end_height/2])
	         centerCube(end_length,end_width,end_height);
             // body
	     translate([-pcb_len/2+0.1,-pcb_width/2,0])
	         cube([pcb_len/2-0.7 + pogo_box_len/2 + pogo_center[0],
		           pcb_width,3]);
        };
	     // text
  	     translate([-1,0,2.5])
	     rotate(a=[0,0,-90])linear_extrude(0.6)
             text(text=version_string,size=2,halign="center",valign="center");
         translate([3.5,1,2.5])
                 linear_extrude(0.6)       
                 text(text="*",size=3,halign="center");
    }; // difference end
}

if (makebase) 
   difference() {
     makeBase();
     makePCB();
   };

if (makelid) makeLid();
