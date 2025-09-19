
include  <roundedcube.scad>
$fn = 120; // steps in generating circles

// select parts

makelid = true;
makebase = false;

version_string = "BTCase"; // imprinted in base and lid

                // fiberglass thickness
pcb_thick     = 2.0;                    // pcb clearance  (for sweeping space)
pcb_clearance = 0.002*25.4;             // edge clearance (routing error)
pcb_len       = 14 + pcb_clearance*2;
pcb_width     = 9 + pcb_clearance*2;
   
lid_height = 3.5;

//
// dimensions of base
//

base_width       = pcb_width + 5.0;
base_len         = pcb_len + 14;
base_height      = 4;//1.5;  // base holds 1.5mm board
post_rad         = 2.0;                    // radius of post support
post_hole_insert = 0.75;                   // clearance for 2-56 insert
post_dist        = 17.0;                   // distance between posts


// helper function
// create a cube centered [x/2,y/2,0]

module centerCube(x,y,z) {
   translate([0,0,z/2]) cube([x,y,z],center=true);
}


// generate the PCB 




module makePCB(height) {
   centerCube(pcb_len,pcb_width,height);
    // battery
   translate([pcb_len/2.0+3,0,0])cylinder(r=5,h=height+2);
}


// make a crossbar with endholes -- we need two of these
// one for the base, one for the lid

module makeCrossBar(height,endheight,hole) {
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
                    
                    // base cube
                    roundedcube([base_len,base_width,base_height],true,0.8,"z");
                    
                    // snap bar
                    
                    translate([0,0,0.85])centerCube(6,base_width+1.1,1);

                    // cross bars
                    
                    translate([-9,0,-2])makeCrossBar(base_height/2,base_height/2,post_hole_insert);
                    translate([ 10,0,-2])makeCrossBar(base_height/2,base_height/2,post_hole_insert);
                
                };
                
                // alignment pins
                 translate([base_len/2-2,base_width/2-2,0])cylinder(h=0.8,r=0.7);
                 translate([2-base_len/2,2-base_width/2,0])cylinder(h=0.8,r=0.7);
            }
        {
            // text
        
               translate([1,0,-base_height+.3])
                 rotate(a=[0,180,-90])linear_extrude(0.5)	
                 text(text=version_string,size=2,halign="center");
        };
    }
}


module snap(length){
     rotate([0,90,0]) linear_extrude(length)polygon(points=[[0,0],[4,0],[4,0.5],[3,1.25],[2.8,1.25],[2.8,0.8],[0,0.8]]);
    //linear_extrude(length)polygon(points=[[0,0],[4,0],[4,0.5],[2.5,2],[2.5,1],[0,1]]);
    
    
}

module makeLid(){
	translate([0,0,lid_height/2])
	        difference(){union(){
                roundedcube([base_len,base_width,lid_height],true,0.8,"z");
                translate([0,0,-lid_height/2 + 1])centerCube(5,base_width+2.9,lid_height-1);
                //translate([0,0,-lid_height/2-1])centerCube(base_len+2,8,lid_height+1);
               
                // snaps
                #translate([-2.5,-8.5,-0.3])snap(5);
                rotate([0,0,180])translate([-2.5,-8.5,-0.3])snap(5);
            };
             // text
                 translate([-1,0,lid_height/2-0.5])
                 rotate(a=[0,0,-90])linear_extrude(0.6)	
                 text(text=version_string,size=2,halign="center");
            // alignment pin holes
                 translate([base_len/2-2,base_width/2-2,-1.8])cylinder(h=1,r=0.8);
                 translate([2-base_len/2,2-base_width/2,-1.8])cylinder(h=1,r=0.8);
            
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
    
     color("green")rotate(makebase?[0,0,0]:[180,0,0])translate(makebase?[0,0,0]:[0,0,-lid_height])difference() {
     makeLid();
     translate([-3,0,-base_height+1.25])makePCB(3);
   }
   

