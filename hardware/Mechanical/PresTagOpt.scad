$fn=120;


version_string = "PresTagV0"; // imprinted in base and lid


// measurements from pcb_new of pcb

front_hole_offset = [0.25,0,0];
rear_hole_offset  = [-11.75,0,0];

      // main rectangle
clearance_size = [13,9];
clearance_offset = [-15/2+1.75,0,-1.5];
      // large features
battery_offset=[-0.75,0,0];
lightpipe_offset = [-13.03,0,0];

//
// Parameters for test pins, screw locations
// 

pogo_pin_height = 6.27-0.9;  // Pin height at pcb
pogo_box = [3.2,7];
block_height = pogo_pin_height+2;

// screw post

screw_diameter=2.4;
post_diameter=6.0;
insert_diameter = 1.65*2;

// from footprint of pogopins

post_offset=[-5,10,0];
post_offset2=[-5,-10,0];
pin_offset=[-5,6.5,0];
pin_offset2=[-5,-6.5,0];
align_pin_diameter = 1.5;

// PCB outline dxf -- export dxf from kicad plot, dimensions = mm, 
//                    contours = no

filename = "test.dxf";

// PCB thickness for sweeping base

pcb_thick = 2.5;

// edge clearance

edge_clearance = 0.006*25.4;

// offset to center of pins

board_offset = [-151.03,108.441,0];

// pcb board 

module render_pcb(filename,board_offset) {
   // extruded, expanded dxf board outline
   linear_extrude(height=pcb_thick) {           
       offset(delta=edge_clearance)
       translate(board_offset)
       import(file = filename, layer = 2);
   };
}
 
module battery(){
    // dimensions for ms920 battery
    battery_diameter=9.5;
    conn_size=2.5;
    conn_offset=[2.21+conn_size/2,0,0];
    // height scaled to clear lid
    battery_height=3;
    // expand footprint to create clearance
    // pull battery closer for more clearance
    size_delta=0.2;
    disk_offset=[11.6-battery_diameter/2,0,battery_height/2];
    translate(battery_offset)linear_extrude(height=battery_height)
      offset(delta=size_delta) union(){
        translate(disk_offset)
        circle(d=battery_diameter);
        translate(conn_offset)square(size=[conn_size,conn_size],center=true);}
}

module lightpipe(){
    // dimensions based upon bivar vlp
    // https://www.bivar.com/parts_content/Datasheets/VLP-XXX-X.pdf
    // use 0.4 for minimum board thickness to get correct offset 
    // for lid
    // pipe and base dimensions expanded for clearance
    pipe_height=10;
    base_height=6;
    translate(lightpipe_offset)
    union(){
        translate([0,0,pipe_height/2])
        cylinder(h=pipe_height,d=3.5,center= true);
        linear_extrude(height=base_height)square([5.2,5.2],center=true);
       translate([-2.05,2.05,-0.84])cylinder(h=1.7,d=1,center=true);
       translate([2.05,-2.05,-0.84])cylinder(h=1.7,d=1,center=true);
        
    }
}

module pcb_sweep() {
  translate([0,0,pogo_pin_height])union() {
    render_pcb(filename,board_offset);
    battery();
    lightpipe();
    }
}   
    


module base_block(){
    block_height = pogo_pin_height+2;
    union(){
        translate(clearance_offset + [0,0,1.5]) linear_extrude(height=block_height) 
        offset(delta=3)
        square(size=clearance_size,center=true);
     translate([post_offset[0],0,0])linear_extrude(height=block_height)
        square(size=[post_diameter,20],center=true);
     translate(post_offset)cylinder(h=block_height,d=post_diameter);
     translate(post_offset2)cylinder(h=block_height,d=post_diameter);
     
   };  
}

module base(){
cavity_offset     = [0,0,pogo_pin_height-1];
difference(){
   base_block();
   union(){
    %pcb_sweep();
    // screw inserts
    translate(post_offset-[0,0,1])cylinder(h=block_height+2,d=insert_diameter);
    translate(post_offset2-[0,0,1])cylinder(h=block_height+2,d=insert_diameter);
    // pins
    translate(pin_offset-[0,0,1])cylinder(h=block_height+2,d=align_pin_diameter);
    translate(pin_offset2-[0,0,1])cylinder(h=block_height+2,d=align_pin_diameter);
    // pogo_cuttout
    translate([0,0,-1])linear_extrude(height=block_height+2)square(size=pogo_box,center=true);
    // harness clearance
     translate(front_hole_offset+cavity_offset)linear_extrude(height=4)square(size=[2,20],center=true);
    translate(rear_hole_offset+cavity_offset)linear_extrude(height=4)square(size=[2,20],center=true);
    // clearance
   translate(clearance_offset+cavity_offset+[0,0,1.5]) 
        linear_extrude(height=4) 
        offset(delta=-0.5)
   // text
        square(size=clearance_size,center=true);
   translate([-8,0,0.4])rotate(a=[0,180,-90])linear_extrude(0.6)
             text(text=version_string,size=2,halign="center",valign="center");
   translate([3,4.3,pogo_pin_height+1.6])
                 linear_extrude(0.5)       
                 text(text="\u2B24",font="DejaVu Sans",size=0.75,halign="center");

   }
       
}}

module lid(){
  lid_size = clearance_size-[1,1];
  endcap_width=lid_size[1];
  difference(){
  union(){
  translate(clearance_offset+[0,0,16.5])
    linear_extrude(height=2)
    square(size=lid_size,center=true);
      // endcaps
    translate(front_hole_offset+[0,0,12.5])linear_extrude(height=4.5)square(         size=[2,endcap_width],center=true);
    translate(rear_hole_offset+[0,0,12.5])linear_extrude(height=4.5)square(         size=[2,endcap_width],center=true);
      // cross bar
    translate([post_offset[0],0,15])linear_extrude(height=2)square(size=[post_diameter,20],center=true);
      // posts
    translate(post_offset+[0,0,15])linear_extrude(height=3)circle(d=post_diameter);
    translate(post_offset2+[0,0,15])linear_extrude(height=3)circle(d=post_diameter);
  }
  union(){
      // create voids
      translate([0,0,11.95])battery();
      translate([0,0,11.95])lightpipe();
      translate(post_offset+[0,0,12])cylinder(h=10,d=screw_diameter);
      translate(post_offset2+[0,0,12])cylinder(h=10,d=screw_diameter);
      translate([-5,0,16.5])rotate(a=[0,0,-90])linear_extrude(0.6)
             text(text=version_string,size=2,halign="center",valign="center");
       translate([-0.5,4,16.5])rotate(a=[0,0,-60])linear_extrude(0.6)
         text("\u2B06", font="DejaVu Sans",size=2,halign="left",valign="center");


  }
  }
    
}

union(){
    base();
    lid();
}







